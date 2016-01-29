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
#include "stm8l15x_clk.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_tim3.h"


#define DISPLAY_BACKLIGHT_PORT      GPIOD
#define DISPLAY_BACKLIGHT_PIN       GPIO_Pin_0

void PWM_configure(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);

    GPIO_Init(DISPLAY_BACKLIGHT_PORT,DISPLAY_BACKLIGHT_PIN,GPIO_Mode_Out_PP_High_Fast);

    TIM3_DeInit();
    TIM3_TimeBaseInit(TIM3_Prescaler_1,TIM3_CounterMode_Down,0x1);

    TIM3_OC2Init(TIM3_OCMode_PWM2,
                  TIM3_OutputState_Enable,
                  (uint16_t)500,
                  TIM3_OCPolarity_Low,
                  TIM3_OCIdleState_Set);

    TIM3_OC2PreloadConfig(ENABLE);
    TIM3_ARRPreloadConfig(ENABLE);

    TIM3_CtrlPWMOutputs(ENABLE);
    TIM3_Cmd(ENABLE);
}
