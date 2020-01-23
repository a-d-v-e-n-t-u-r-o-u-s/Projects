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
#include "system_timer.h"
#include "system_common.h"
#include <stdbool.h>
#include "debug.h"

#define SYSTEM_MAX_TASKS    5

typedef struct
{
    SYSTEM_task_t task;
    uint32_t last_tick;
    uint16_t interval;
} task_data_t;

static task_data_t tasks[SYSTEM_MAX_TASKS];
static uint8_t task_counter;

int8_t SYSTEM_register_task(SYSTEM_task_t task, uint16_t interval)
{
    if(task == NULL || task_counter == SYSTEM_MAX_TASKS)
    {
        return -1;
    }

    tasks[task_counter].task = task;
    tasks[task_counter].interval = interval;
    task_counter++;
    return 0;
}

void SYSTEM_main(void)
{
    uint8_t i = 0;

    for(i=0;i<task_counter;i++)
    {
        uint32_t curr_tick = SYSTEM_timer_get_tick();

        if(SYSTEM_timer_tick_difference(tasks[i].last_tick, curr_tick) >= tasks[i].interval)
        {
            tasks[i].task();
            tasks[i].last_tick = curr_tick;
        }
    }
}

void SYSTEM_init(void)
{
    SYSTEM_timer_init();
}
