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

/*!
 * \brief Maximal amount of drivers which can be configured
 */
#define MAX_HANDLES         2

/*!
 * \brief Hardware configuration structure
 */
typedef struct
{
    GPIO_TypeDef *port;     /*!< GPIO port for PWM */
    GPIO_Pin_TypeDef pin;   /*!< GPIO pin for PWM */
} PWM_hardware_config_t;

/*!
 * \brief Handle structure containing all information about driver
 */
typedef struct PWM_handle_t
{
    uint8_t is_configured;  /*!< indicates is driver configured */
    uint8_t is_attached;    /*!< indicates is driver attached */
    PWM_config_t config;    /*!< holds driver's configuration data */
} PWM_handle_t;

/*!
 * \brief Holds duty cycle for timer No.2 and channel No.1
 *
 * \note
 * In case channel_duty_cycle1 in PWM_config_t
 * is equal to NULL no value is assigned
 *
 * \see PWM_config_t
 */
static uint16_t timer2_channel1_duty_cycle;

/*!
 * \brief Holds duty cycle for timer No.2 and channel No.2
 *
 * \note
 * In case channel_duty_cycle2 in PWM_config_t
 * is equal to NULL no value is assigned
 *
 * \see PWM_config_t
 */
static uint16_t timer2_channel2_duty_cycle;

/*!
 * \brief Holds duty cycle for timer No.3 and channel No.1
 *
 * \note
 * In case channel_duty_cycle1 in PWM_config_t
 * is equal to NULL no value is assigned
 *
 * \see PWM_config_t
 */
static uint16_t timer3_channel1_duty_cycle;

/*!
 * \brief Holds duty cycle for timer No.3 and channel No.2
 *
 * \note
 * In case channel_duty_cycle2 in PWM_config_t
 * is equal to NULL no value is assigned
 *
 * \see PWM_config_t
 */
static uint16_t timer3_channel2_duty_cycle;

/*!
 * \brief Holds handles to drivers
 */
static PWM_handle_t handles[MAX_HANDLES];

/*!
 * \brief Checks is pointer to handle NULL
 *
 * \param handle handle to be check
 *
 * \retval 0 handle is NULL
 * \retval -1 handle isn't NULL
 */
static @inline int8_t is_handle_null(PWM_handle_t *handle)
{
    if(NULL != handle)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*!
 * \brief Checks is pointer to handle pointer NULL,
 *
 * \param handle pointer to handle pointer to be checked
 *
 * \retval 0 handle is NULL
 * \retval -1 handle isn't NULL
 */
static @inline int8_t is_ptr_to_handle_ptr_null(PWM_handle_t **handle)
{
    if(NULL != handle)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*!
 * \brief Checks is driver identified by handle configured
 *
 * \param handle driver handle as input for checking
 *
 * \retval 0 driver with handle is configured
 * \retval -1 driver with handle isn't configured
 */
static @inline int8_t is_configured(PWM_handle_t *handle)
{
    if(0 == handle->is_configured)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*!
 * \brief Checks is driver identified by handle attached
 *
 * \param handle driver handle as input for checking
 *
 * \retval 0 driver with handle is attached
 * \retval -1 driver with handle isn't attached
 */
static @inline int8_t is_attached(PWM_handle_t *handle)
{
    if(0 == handle->is_attached)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*!
 * \brief Checks is configuration structure NULL
 *
 * \param config configuration structure to be checked
 *
 * \retval 0 configuration structure is NULL
 * \retval -1 configuration structure isn't NULL
 */
static @inline int8_t is_config_null(const PWM_config_t *config)
{
    if(NULL != config)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*!
 * \brief Checks is pointer to uint16_t is NULL
 *
 * \param ptr pointer to be checked
 *
 * \retval 0 pointer is NULL
 * \retval -1 pointer isn't NULL
 */
static @inline int8_t is_uint16_t_ptr_null(const uint16_t *ptr)
{
    if(NULL != ptr)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*!
 * \brief Validates configuration structure
 *
 * \param config configuration structure to be validated
 *
 * \retval 0 configuration structure is valid
 * \retval -1 configuration structures for both channels are NULL
 * \retval -2 unsupported timer group has been passed
 * \retval -3 unsupported prescaler value has been passed
 */
static @inline int8_t is_config_valid(const PWM_config_t *config)
{
    if(0 == is_uint16_t_ptr_null(config->channel_duty_cycle1) &&
        0 == is_uint16_t_ptr_null(config->channel_duty_cycle2))
    {
        return -1;
    }

    if(PWM_TIMER2_GROUP != config->group &&
            PWM_TIMER3_GROUP != config->group)
    {
        return -2;
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

    return -3;
}

/*!
 * \brief Converts PWM_prescaler_t value to TIM2_Prescaler_TypeDef value
 *
 * \param prescaler input prescaler value from generic enum PWM_prescaler_t
 *
 * \return output prescaler value from enum TIM2_Prescaler_TypeDef
 */
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

/*!
 * \brief Converts PWM_prescaler_t value to TIM3_Prescaler_TypeDef value
 *
 * \param prescaler input prescaler value from generic enum PWM_prescaler_t
 *
 * \return output prescaler value from enum TIM3_Prescaler_TypeDef
 */
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
 * \brief Sets timer No.2 and channel No.1 specific values
 * in order to attach
 *
 * \param duty_cycle value of duty_cycle to be set
 *
 * \todo check why changing parameter from const PWM_config_t *config
 * to uint16_t duty_cycle causes growing of .text section
 */
static @inline void attach_timer2_channel_1(uint16_t duty_cycle)
{
    GPIO_Init(PWM_TIMER2_CHANNEL1_PORT,PWM_TIMER2_CHANNEL1_PIN,GPIO_Mode_Out_PP_High_Fast);

    TIM2_OC1Init(TIM2_OCMode_PWM2,
        TIM2_OutputState_Enable,
        duty_cycle,
        TIM2_OCPolarity_Low,
        TIM2_OCIdleState_Set);
    TIM2_OC1PreloadConfig(ENABLE);
}

/*!
 * \brief Sets timer No.2 and channel No.1 specific values
 * in order to detach
 */
static @inline void detach_timer2_channel_1(void)
{
    TIM2_OC1PreloadConfig(DISABLE);
    GPIO_Init(PWM_TIMER2_CHANNEL1_PORT,PWM_TIMER2_CHANNEL1_PIN,GPIO_Mode_In_FL_No_IT);
}

/*!
 * \brief Sets timer No.2 and channel No.2 specific values
 * in order to attach
 *
 * \param duty_cycle value of duty_cycle to be set
 *
 * \todo check why changing parameter from const PWM_config_t *config
 * to uint16_t duty_cycle causes growing of .text section
 */
static @inline void attach_timer2_channel_2(uint16_t duty_cycle)
{
    GPIO_Init(PWM_TIMER2_CHANNEL2_PORT,PWM_TIMER2_CHANNEL2_PIN,GPIO_Mode_Out_PP_High_Fast);

    TIM2_OC2Init(TIM2_OCMode_PWM2,
        TIM2_OutputState_Enable,
        duty_cycle,
        TIM2_OCPolarity_Low,
        TIM2_OCIdleState_Set);
    TIM2_OC2PreloadConfig(ENABLE);
}

/*!
 * \brief Sets timer No.2 and channel No.2 specific values
 * in order to detach
 */
static @inline void detach_timer2_channel_2(void)
{
    TIM2_OC2PreloadConfig(DISABLE);
    GPIO_Init(PWM_TIMER2_CHANNEL2_PORT,PWM_TIMER2_CHANNEL2_PIN,GPIO_Mode_In_FL_No_IT);
}

/*!
 * \brief Sets timer No.2 specific values in order to attach
 *
 * \param config configuration structure with parameters
 */
static @inline void attach_timer2_group(const PWM_config_t *config)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    TIM2_DeInit();

    TIM2_TimeBaseInit(get_timer2_prescaler(config->prescaler),
            TIM2_CounterMode_Down,config->counter);

    if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle1))
    {
        attach_timer2_channel_1(*(config->channel_duty_cycle1));
    }

    if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle2))
    {
        attach_timer2_channel_2(*(config->channel_duty_cycle2));
    }

    TIM2_ARRPreloadConfig(ENABLE);
    TIM2_CtrlPWMOutputs(ENABLE);
    TIM2_Cmd(ENABLE);
}

/*!
 * \brief Sets timer No.2 specific values in order to detach
 *
 * \param config configuration structure with parameters
 */
static @inline void detach_timer2_group(const PWM_config_t *config)
{
    TIM2_Cmd(DISABLE);
    TIM2_CtrlPWMOutputs(DISABLE);
    TIM2_ARRPreloadConfig(DISABLE);

    /*
     * checking for NULL cannot be omitted, because can cause reconfiguring
     * GPIO used by other modules
     */
    if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle1))
    {
        detach_timer2_channel_1();
    }

    /*
     * checking for NULL cannot be omitted, because can cause reconfiguring
     * GPIO used by other modules
     */
    if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle2))
    {
        detach_timer2_channel_2();
    }

    TIM2_DeInit();
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, DISABLE);
}

/*!
 * \brief Sets timer No.3 and channel No.1 specific values
 * in order to attach
 *
 * \param duty_cycle value of duty_cycle to be set
 *
 * \todo check why changing parameter from const PWM_config_t *config
 * to uint16_t duty_cycle causes growing of .text section
 */
static @inline void attach_timer3_channel_1(uint16_t duty_cycle)
{
    GPIO_Init(PWM_TIMER3_CHANNEL1_PORT,PWM_TIMER3_CHANNEL1_PIN,GPIO_Mode_Out_PP_High_Fast);

    TIM3_OC1Init(TIM3_OCMode_PWM2,
                TIM3_OutputState_Enable,
                duty_cycle,
                TIM3_OCPolarity_Low,
                TIM3_OCIdleState_Set);
    TIM3_OC1PreloadConfig(ENABLE);
}

/*!
 * \brief Sets timer No.3 and channel No.1 specific values
 * in order to detach
 */
static @inline void detach_timer3_channel_1(void)
{
    TIM3_OC1PreloadConfig(DISABLE);
    GPIO_Init(PWM_TIMER3_CHANNEL1_PORT,PWM_TIMER3_CHANNEL1_PIN,GPIO_Mode_In_FL_No_IT);
}

/*!
 * \brief Sets timer No.3 and channel No.2 specific values
 * in order to attach
 *
 * \param duty_cycle value of duty_cycle to be set
 *
 * \todo check why changing parameter from const PWM_config_t *config
 * to uint16_t duty_cycle causes growing of .text section
 */
static @inline void attach_timer3_channel_2(uint16_t duty_cycle)
{
    GPIO_Init(PWM_TIMER3_CHANNEL2_PORT,PWM_TIMER3_CHANNEL2_PIN,GPIO_Mode_Out_PP_High_Fast);

    TIM3_OC2Init(TIM3_OCMode_PWM2,
                TIM3_OutputState_Enable,
                duty_cycle,
                TIM3_OCPolarity_Low,
                TIM3_OCIdleState_Set);
    TIM3_OC2PreloadConfig(ENABLE);
}

/*!
 * \brief Sets timer No.3 and channel No.2 specific values
 * in order to detach
 */
static @inline void detach_timer3_channel_2(void)
{
    TIM3_OC2PreloadConfig(DISABLE);
    GPIO_Init(PWM_TIMER3_CHANNEL2_PORT,PWM_TIMER3_CHANNEL2_PIN,GPIO_Mode_In_FL_No_IT);
}

/*!
 * \brief Sets timer No.3 specific values in order to attach
 *
 * \param config configuration structure with parameters
 */
static @inline void attach_timer3_group(const PWM_config_t *config)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
    TIM3_DeInit();
    TIM3_TimeBaseInit(get_timer3_prescaler(config->prescaler),
            TIM3_CounterMode_Down,config->counter);

    if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle1))
    {
        attach_timer3_channel_1(*(config->channel_duty_cycle1));
    }

    if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle2))
    {
        attach_timer3_channel_2(*(config->channel_duty_cycle2));
    }

    TIM3_ARRPreloadConfig(ENABLE);
    TIM3_CtrlPWMOutputs(ENABLE);
    TIM3_Cmd(ENABLE);
}

/*!
 * \brief Sets timer No.3 specific values in order to detach
 *
 * \param config configuration structure with parameters
 */
static @inline void detach_timer3_group(const PWM_config_t *config)
{
    TIM3_Cmd(DISABLE);
    TIM3_CtrlPWMOutputs(DISABLE);
    TIM3_ARRPreloadConfig(DISABLE);

    /*
     * checking for NULL cannot be omitted, because can cause reconfiguring
     * GPIO used by other modules
     */
    if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle1))
    {
        detach_timer3_channel_1();
    }

    /*
     * checking for NULL cannot be omitted, because can cause reconfiguring
     * GPIO used by other modules
     */
    if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle2))
    {
        detach_timer3_channel_2();
    }

    TIM3_DeInit();
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, DISABLE);
}

/*!
 * \brief Acquires handle to driver regarding to configuration
 * structure
 *
 * \param config configuration structure according to which
 * handle will be set
 *
 * \return pointer to driver handle
 */
static @inline PWM_handle_t* acquire_handle(const PWM_config_t *config)
{
    PWM_handle_t *handle = NULL;

    if(PWM_TIMER2_GROUP == config->group)
    {
        handle = &handles[0];

        if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle1))
        {
            timer2_channel1_duty_cycle = *(config->channel_duty_cycle1);
            handle->config.channel_duty_cycle1 = &timer2_channel1_duty_cycle;
        }

        if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle2))
        {
            timer2_channel2_duty_cycle = *(config->channel_duty_cycle2);
            handle->config.channel_duty_cycle2 = &timer2_channel2_duty_cycle;
        }
    }
    else
    {
        handle = &handles[1];

        if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle1))
        {
            timer3_channel1_duty_cycle = *(config->channel_duty_cycle1);
            handle->config.channel_duty_cycle1 = &timer3_channel1_duty_cycle;
        }

        if(0 != is_uint16_t_ptr_null(config->channel_duty_cycle2))
        {
            timer3_channel2_duty_cycle = *(config->channel_duty_cycle2);
            handle->config.channel_duty_cycle2 = &timer3_channel2_duty_cycle;
        }
    }

    handle->config.prescaler = config->prescaler;
    handle->config.group = config->group;
    handle->config.counter = config->counter;

    return handle;
}

/*!
 * \brief Attaches internally driver with passed handle
 *
 * \param handle handle acquired with acquire_handle function
 * or passed by user
 */
static void pwm_internal_attach(PWM_handle_t *handle)
{
    if(PWM_TIMER2_GROUP == handle->config.group)
    {
        attach_timer2_group(&handle->config);
    }
    else
    {
        attach_timer3_group(&handle->config);
    }

    handle->is_attached = 1U;
}

/*!
 * \brief Detaches internally driver with passed handle
 *
 * \param handle handle acquired with acquire_handle function
 * or passed by user
 */
static void pwm_internal_detach(PWM_handle_t *handle)
{
    if(PWM_TIMER2_GROUP == handle->config.group)
    {
        detach_timer2_group(&handle->config);
    }
    else
    {
        detach_timer3_group(&handle->config);
    }

    handle->is_attached = 0U;
}

int8_t PWM_set_counter(struct PWM_handle_t *handle,uint16_t counter,PWM_prescaler_t prescaler)
{
    if(0 == is_handle_null(handle))
    {
        return -1;
    }
    else if(0 != is_configured(handle))
    {
        return -2;
    }
    else
    {
        handle->config.counter = counter;

        if(0 == is_attached(handle))
        {
            if(PWM_TIMER2_GROUP == handle->config.group)
            {
                TIM2_SetAutoreload(counter);
                TIM2_PrescalerConfig(get_timer2_prescaler(prescaler),
                        TIM2_PSCReloadMode_Immediate);
            }
            else
            {
                TIM3_SetAutoreload(counter);
                TIM3_PrescalerConfig(get_timer3_prescaler(prescaler),
                        TIM3_PSCReloadMode_Immediate);
            }
        }

        return 0;
    }
}

int8_t PWM_get_counter(uint16_t *counter,PWM_prescaler_t *prescaler,struct PWM_handle_t *handle)
{
    if(0 == is_handle_null(handle))
    {
        return -1;
    }
    else if(0 != is_configured(handle))
    {
        return -2;
    }
    else if(0 == is_uint16_t_ptr_null(counter))
    {
        return -3;
    }
    else if(NULL == prescaler)
    {
        return -4;
    }
    else
    {
        *counter = handle->config.counter;
        *prescaler = handle->config.prescaler;
        return 0;
    }
}

int8_t PWM_attach(struct PWM_handle_t *handle)
{
    if(0 == is_handle_null(handle))
    {
        return -1;
    }
    else if(0 != is_configured(handle))
    {
        return -2;
    }
    else if(0 == is_attached(handle))
    {
        return -3;
    }
    else
    {
        pwm_internal_attach(handle);
        return 0;
    }
}

int8_t PWM_detach(struct PWM_handle_t *handle)
{
    if(0 == is_handle_null(handle))
    {
        return -1;
    }
    else if(0 != is_configured(handle))
    {
        return -2;
    }
    else if(0 != is_attached(handle))
    {
        return -3;
    }
    else
    {
        pwm_internal_detach(handle);
        return 0;
    }
}

int8_t PWM_configure(struct PWM_handle_t **handle,
        const PWM_config_t *config,uint8_t is_attached)
{
    PWM_handle_t *tmp = NULL;
    PWM_hardware_config_t hardware = {0};

    if(0 == is_ptr_to_handle_ptr_null(handle))
    {
        return -1;
    }

    if(0 == is_config_null(config))
    {
        return -2;
    }

    if(0 != is_config_valid(config))
    {
        return -3;
    }

    tmp = acquire_handle(config);

    if(0 != is_attached)
    {
        pwm_internal_attach(tmp);
    }
    else
    {
        pwm_internal_detach(tmp);
    }

    tmp->is_configured = 1U;

    *handle = tmp;

    return 0;
}
