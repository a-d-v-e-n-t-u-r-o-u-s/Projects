/*!
 * \file
 * \brief PWM driver
 * \author Dawid Babula
 * \email dbabula@adventurous.pl
 *
 * \par Copyright (C) Dawid Babula, 2016
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "pwm.h"
#include "common.h"
#include "hardware.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_tim2.h"
#include "stm8l15x_tim3.h"

#define MAX_HANDLES         2
#define MAX_CHANNELS        4

typedef struct
{
    GPIO_TypeDef *port;
    GPIO_Pin_TypeDef pin;
} PWM_hardware_config_t;

typedef struct PWM_handle_t
{
    uint8_t is_configured;
    uint8_t is_attached;
    PWM_config_t *config;
} PWM_handle_t;

static PWM_handle_t handles[MAX_HANDLES];

static @inline int8_t is_handle_valid(PWM_handle_t *handle)
{
    if(NULL == handle)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static @inline int8_t is_config_null(const PWM_config_t *config)
{
    if(NULL == config)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static @inline int8_t is_channel_duty_cycle(const uint16_t *duty_cycle)
{
    if(NULL == duty_cycle)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static @inline int8_t is_config_valid(const PWM_config_t *config)
{
    if(0 != is_channel_duty_cycle(config->channel_duty_cycle1) &&
        0 != is_channel_duty_cycle(config->channel_duty_cycle2))
    {
        return -1;
    }

    /* not test if needed */
    if(0 == is_channel_duty_cycle(config->channel_duty_cycle1))
    {
        if(*(config->channel_duty_cycle1) > config->counter)
        {
            return -2;
        }
    }

    /* not test if needed */
    if(0 == is_channel_duty_cycle(config->channel_duty_cycle2))
    {
        if(*(config->channel_duty_cycle2) > config->counter)
        {
            return -3;
        }
    }

    if(PWM_TIMER2_GROUP != config->group &&
            PWM_TIMER3_GROUP != config->group)
    {
        return -4;
    }

    switch(config->prescaler)
    {
        case PWM_PRESCALER1:
            /* fall through */
        case PWM_PRESCALER2:
            /* fall through */
        case PWM_PRESCALER4:
            /* fall through */
        case PWM_PRESCALER8:
            /* fall through */
        case PWM_PRESCALER16:
            /* fall through */
        case PWM_PRESCALER32:
            /* fall through */
        case PWM_PRESCALER64:
            /* fall through */
        case PWM_PRESCALER128:
            return 0;
    }

    return -5;
}

static @inline TIM2_Prescaler_TypeDef get_timer2_prescaler(PWM_prescaler_t prescaler)
{
    switch(prescaler)
    {
        case PWM_PRESCALER1:
            return TIM2_Prescaler_1;
        case PWM_PRESCALER2:
            return TIM2_Prescaler_2;
        case PWM_PRESCALER4:
            return TIM2_Prescaler_4;
        case PWM_PRESCALER8:
            return TIM2_Prescaler_8;
        case PWM_PRESCALER16:
            return TIM2_Prescaler_16;
        case PWM_PRESCALER32:
            return TIM2_Prescaler_32;
        case PWM_PRESCALER64:
            return TIM2_Prescaler_64;
        case PWM_PRESCALER128:
            return TIM2_Prescaler_128;
    }

    return TIM2_Prescaler_1;
}

static @inline TIM3_Prescaler_TypeDef get_timer3_prescaler(PWM_prescaler_t prescaler)
{
    switch(prescaler)
    {
        case PWM_PRESCALER1:
            return TIM3_Prescaler_1;
        case PWM_PRESCALER2:
            return TIM3_Prescaler_2;
        case PWM_PRESCALER4:
            return TIM3_Prescaler_4;
        case PWM_PRESCALER8:
            return TIM3_Prescaler_8;
        case PWM_PRESCALER16:
            return TIM3_Prescaler_16;
        case PWM_PRESCALER32:
            return TIM3_Prescaler_32;
        case PWM_PRESCALER64:
            return TIM3_Prescaler_64;
        case PWM_PRESCALER128:
            return TIM3_Prescaler_128;
    }

    return TIM3_Prescaler_1;
}

/*!
 * \todo check why changing parameter from const PWM_config_t *config
 * to uint16_t duty_cycle causes growing of .text section
 */
static @inline void configure_timer2_channel_1(uint16_t duty_cycle)
{
    GPIO_Init(PWM_TIMER2_CHANNEL1_PORT,PWM_TIMER2_CHANNEL1_PIN,GPIO_Mode_Out_PP_High_Fast);

    TIM2_OC1Init(TIM2_OCMode_PWM2,
        TIM2_OutputState_Enable,
        duty_cycle,
        TIM2_OCPolarity_Low,
        TIM2_OCIdleState_Set);
    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_ARRPreloadConfig(ENABLE);
    TIM2_CtrlPWMOutputs(ENABLE);
    TIM2_Cmd(ENABLE);
}

/*!
 * \todo check why changing parameter from const PWM_config_t *config
 * to uint16_t duty_cycle causes growing of .text section
 */
static @inline void configure_timer2_channel_2(uint16_t duty_cycle)
{
    GPIO_Init(PWM_TIMER2_CHANNEL2_PORT,PWM_TIMER2_CHANNEL2_PIN,GPIO_Mode_Out_PP_High_Fast);

    TIM2_OC2Init(TIM2_OCMode_PWM2,
        TIM2_OutputState_Enable,
        duty_cycle,
        TIM2_OCPolarity_Low,
        TIM2_OCIdleState_Set);
    TIM2_OC2PreloadConfig(ENABLE);
    TIM2_ARRPreloadConfig(ENABLE);
    TIM2_CtrlPWMOutputs(ENABLE);
    TIM2_Cmd(ENABLE);
}

static @inline void configure_timer2_group(const PWM_config_t *config)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    TIM2_DeInit();

    TIM2_TimeBaseInit(get_timer2_prescaler(config->prescaler),
            TIM2_CounterMode_Down,config->counter);

    if(0 == is_channel_duty_cycle(config->channel_duty_cycle1))
    {
        configure_timer2_channel_1(*(config->channel_duty_cycle1));
    }

    if(0 == is_channel_duty_cycle(config->channel_duty_cycle2))
    {
        configure_timer2_channel_2(*(config->channel_duty_cycle2));
    }
}

/*!
 * \todo check why changing parameter from const PWM_config_t *config
 * to uint16_t duty_cycle causes growing of .text section
 */
static @inline void configure_timer3_channel_1(uint16_t duty_cycle)
{
    GPIO_Init(PWM_TIMER3_CHANNEL1_PORT,PWM_TIMER3_CHANNEL1_PIN,GPIO_Mode_Out_PP_High_Fast);

    TIM3_OC1Init(TIM3_OCMode_PWM2,
                TIM3_OutputState_Enable,
                duty_cycle,
                TIM3_OCPolarity_Low,
                TIM3_OCIdleState_Set);
    TIM3_OC1PreloadConfig(ENABLE);
    TIM3_ARRPreloadConfig(ENABLE);
    TIM3_CtrlPWMOutputs(ENABLE);
    TIM3_Cmd(ENABLE);
}

/*!
 * \todo check why changing parameter from const PWM_config_t *config
 * to uint16_t duty_cycle causes growing of .text section
 */
static @inline void configure_timer3_channel_2(uint16_t duty_cycle)
{
    GPIO_Init(PWM_TIMER3_CHANNEL2_PORT,PWM_TIMER3_CHANNEL2_PIN,GPIO_Mode_Out_PP_High_Fast);

    TIM3_OC2Init(TIM3_OCMode_PWM2,
                TIM3_OutputState_Enable,
                duty_cycle,
                TIM3_OCPolarity_Low,
                TIM3_OCIdleState_Set);
    TIM3_OC2PreloadConfig(ENABLE);

    TIM3_ARRPreloadConfig(ENABLE);
    TIM3_CtrlPWMOutputs(ENABLE);
    TIM3_Cmd(ENABLE);
}

static @inline void configure_timer3_group(const PWM_config_t *config)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
    TIM3_DeInit();
    TIM3_TimeBaseInit(get_timer3_prescaler(config->prescaler),
            TIM3_CounterMode_Down,config->counter);

    if(0 == is_channel_duty_cycle(config->channel_duty_cycle1))
    {
        configure_timer3_channel_1(*(config->channel_duty_cycle1));
    }

    if(0 == is_channel_duty_cycle(config->channel_duty_cycle2))
    {
        configure_timer3_channel_2(*(config->channel_duty_cycle2));
    }
}

int8_t PWM_configure(struct PWM_handle_t *handle,
        const PWM_config_t *config,uint8_t is_attached)
{
    PWM_hardware_config_t hardware = {0};

    if(0 != is_config_null(config))
    {
        return -1;
    }

    if(0 != is_config_valid(config))
    {
        return -2;
    }

    if(PWM_TIMER2_GROUP == config->group)
    {
        configure_timer2_group(config);
    }
    else
    {
        configure_timer3_group(config);
    }

    return 0;
}
