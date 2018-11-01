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
#include "system.h"
#include "stm8l15x_clk.h"
#include "system_timer.h"
#include "system_common.h"

#define SYSTEM_MAX_TASKS    5

/*!
 * \todo HSE clock on pin might be necessary
 */

typedef struct
{
    SYSTEM_task_t task;
} task_data_t;

static task_data_t tasks[SYSTEM_MAX_TASKS];
static uint8_t task_counter;

INTERRUPT_HANDLER(SYSTEM_clock_switch, 17)
{
    CLK_ClearITPendingBit(CLK_IT_SWIF);
}

int8_t SYSTEM_register_task(SYSTEM_task_t task)
{
    uint8_t i=0;

    if(task == NULL || task_counter == SYSTEM_MAX_TASKS)
    {
        return -1;
    }

    tasks[task_counter].task = task;
    task_counter++;
    return 0;
}

void SYSTEM_main(void)
{
    uint8_t i = 0;

    for(i=0;i<task_counter;i++)
    {
        if(tasks[i].task != NULL)
        {
            tasks[i].task();
        }
    }
}

void SYSTEM_init(void)
{
    CLK_DeInit();

    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    CLK_ITConfig(CLK_IT_SWIF,ENABLE);
#if 0
    /*!
     * \todo HSI clock set for the RemoteControl project,
     * for WeatherStation should be change to HSE
     */
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
#else
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);
#endif
    CLK_PeripheralClockConfig(CLK_Peripheral_BOOTROM,DISABLE);

    SYSTEM_timer_init();
}
