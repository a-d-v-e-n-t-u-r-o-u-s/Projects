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

static volatile uint8_t *get_port_config_register(uint8_t port)
{
    switch(port)
    {
        case 1:
            return &DDRB;
        case 2:
            return &DDRC;
        case 3:
            return &DDRD;
        default:
            return NULL;
    }
}

static volatile uint8_t *get_port_status_register(uint8_t port)
{
    switch(port)
    {
        case 1:
            return &PORTB;
        case 2:
            return &PORTC;
        case 3:
            return &PORTD;
        default:
            return NULL;
    }
}

static int8_t pin_configure(const SSD_MGR_pin_t data)
{
    volatile uint8_t *port_cfg_reg = get_port_config_register(data.port);

    if(port_cfg_reg == NULL)
    {
        return -1;
    }

    *port_cfg_reg |= (1 << data.pin);
    return 0;
}

static void ssd_mgr_main(void)
{
    static uint8_t counter;
    uint32_t tick = SYSTEM_timer_get_tick();

    for(uint8_t i = 0u; i< module_config.size; i++)
    {
        volatile uint8_t *port_status_reg =
            get_port_status_register(module_config.config[i].port);
            *port_status_reg &= ~(1 << module_config.config[i].pin);
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
            volatile uint8_t *port_status_reg =
                get_port_status_register(module_config.config[counter].port);
            *port_status_reg |= (1 << module_config.config[counter].pin);
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
        const SSD_MGR_pin_t pin = config->config[i];

        if(pin_configure(pin) != 0)
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
