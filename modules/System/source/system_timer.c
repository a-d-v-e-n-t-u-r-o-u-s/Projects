/*!
 * \file
 * \brief Hardware configuration file
 * \author Dawid Babula
 * \email dbabula@adventurous.pl
 *
 * \par Copyright (C) Dawid Babula, 2018
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
#include "system_common.h"
#include "system_timer.h"
#include "stm8l15x.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_gpio.h"

static volatile uint32_t system_tick;
static void (*timer_callback)(void);

INTERRUPT_HANDLER(SYSTEM_timer_tick, 25)
{
    system_tick++;

    if(timer_callback != NULL)
    {
        timer_callback();
    }

    TIM4_ClearFlag(TIM4_FLAG_Update);
}

int8_t SYSTEM_timer_register(void (*callback)(void))
{
    if(callback != NULL)
    {
        timer_callback = callback;
        return 0;
    }
    else
    {
        return -1;
    }
}

uint32_t SYSTEM_timer_get_tick(void)
{
    uint32_t tick=0;
    disableInterrupts();
    tick = system_tick;
    enableInterrupts();
    return tick;
}

uint32_t SYSTEM_timer_tick_difference(uint32_t prev,uint32_t next)
{
    if(next >= prev)
    {
        return next - prev;
    }
    else
    {
        return (U32_MAX - prev) + next + 1;
    }
}

void SYSTEM_timer_delay(uint8_t val)
{
    uint32_t tick = SYSTEM_timer_get_tick();
    while(SYSTEM_timer_tick_difference(tick,SYSTEM_timer_get_tick()) <= val)
    {
    }
}

uint8_t SYSTEM_timer_init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE);

#if 0
    /*!
     * \todo Prescaler set for the RemoteControl project,
     * for WeatherStation should be changed as follows
     */
    TIM4_TimeBaseInit(TIM4_Prescaler_128, 124);
#else
    TIM4_TimeBaseInit(TIM4_Prescaler_64, 124);
#endif
    TIM4_ClearFlag(TIM4_FLAG_Update);
    TIM4_ITConfig(TIM4_IT_Update,ENABLE);
    enableInterrupts();
    TIM4_Cmd(ENABLE);
    return 0;
}
