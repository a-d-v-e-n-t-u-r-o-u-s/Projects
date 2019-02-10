/*!
 * \file
 * \brief Seven segment display driver manager implementation file
 * \author Dawid Babula
 * \email dbabula@adventurous.pl
 *
 * \par Copyright (C) Dawid Babula, 2019
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
#include "ssd_mgr.h"
#include <stddef.h>
#include <avr/io.h>
#include "system.h"
#include "system_timer.h"
#include "ssd_display.h"
#include "debug.h"

static SSD_MGR_config_t module_config;

static void ssd_mgr_main(void)
{
    static uint8_t counter;
    uint32_t tick = SYSTEM_timer_get_tick();

    for(uint8_t i = 0u; i< module_config.size; i++)
    {
        GPIO_write_pin(&module_config.config[i], false);
    }

    while(SYSTEM_timer_tick_difference(tick, SYSTEM_timer_get_tick()) < 2);

    SSD_light(0);

    switch(counter)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        {
            GPIO_write_pin(&module_config.config[counter], true);
        }
            break;
        default:
            DEBUG_output("Shouldn't happen\n");
            break;
    }

    counter++;
    counter %=4;

    while(SYSTEM_timer_tick_difference(tick, SYSTEM_timer_get_tick()) < 5);
}

int8_t SSD_MGR_initialize(const SSD_MGR_config_t *config)
{
    if( config == NULL)
    {
        return -1;
    }

    if(config->size > SSD_MGR_MAX_MULTIPLEXED_DISPLAYS)
    {
        return -1;
    }

    for(uint8_t i = 0u; i < config->size; i++)
    {
        const GPIO_data_t pin = config->config[i];

        if(GPIO_config_pin(GPIO_OUTPUT_PUSH_PULL, &pin) != 0)
        {
            return -1;
        }
    }

    if(SYSTEM_register_task(ssd_mgr_main) != 0)
    {
        return -1;
    }

    module_config = *config;

    return 0;
}
