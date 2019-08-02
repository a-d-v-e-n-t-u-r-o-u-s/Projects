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
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "system.h"
#include "ssd_display.h"
#include "debug.h"

static uint8_t module_config[4];
static bool module_mode;
static uint16_t display_value;

static const bool data[10][7] =
{
    [0] = {  true, true, true, true, true, true, false},
    [1] = {  false, true, true, false, false, false, false},
    [2] = {  true, true, false, true, true, false, true},
    [3] = {  true, true, true, true, false, false, true},
    [4] = {  false, true, true, false, false, true, true},
    [5] = {  true, false, true, true, false, true, true},
    [6] = {  true, false, true, true, true, true, true},
    [7] = {  true, true, true, false, false, false, false},
    [8] = {  true, true, true, true, true, true, true},
    [9] = {  true, true, true, true, false, true, true},
};

static inline uint8_t get_digit(uint16_t value, uint8_t position)
{
     switch(position)
     {
         case 0:
             return (value/1000u)%10u;
         case 1:
             return (value/100u)%10u;
         case 2:
             return (value/10u)%10u;
         case 3:
             return value%10u;
     }

     return 0U;
}

static void multiplex_in_segment_mode(uint16_t value)
{
    for(uint8_t i = 0u; i < 4U ; i++)
    {
        uint8_t digit = get_digit(value, i);
        GPIO_write_pin(module_config[i], true);
        for(uint8_t j = 0u; j < 7u ; j++)
        {
            SSD_set_segment(j, data[digit][j]);
            _delay_us(500);
            SSD_set_segment(j, false);
        }
        GPIO_write_pin(module_config[i], false);
    }
}

static void multiplex_in_digit_mode(uint16_t value)
{
    for(uint8_t i = 0U; i < 4U; i++)
    {
        for(uint8_t j = 0U; j< 4U; j++)
        {
            GPIO_write_pin(module_config[j], false);
        }
        _delay_us(250);
        SSD_light(get_digit(value, i));
        GPIO_write_pin(module_config[i], true);
        _delay_us(250);
    }
}

static void ssd_mgr_main(void)
{
    uint16_t value = display_value;

    if(module_mode)
    {
        multiplex_in_segment_mode(value);
    }
    else
    {
        multiplex_in_digit_mode(value);
    }
}

int8_t SSD_MGR_set(uint16_t value)
{
    if( value > 9999U)
    {
        return -1;
    }

    display_value = value;

    return 0;
}

int8_t SSD_MGR_initialize(const uint8_t config[4],
        bool is_segment_mode)
{
    if( config == NULL)
    {
        return -1;
    }

    /*
     *if(config->size > SSD_MGR_MAX_MULTIPLEXED_DISPLAYS)
     *{
     *    return -1;
     *}
     */

    for(uint8_t i = 0u; i < 4u; i++)
    {
        GPIO_write_pin(config[i], false);
    }

    if(SYSTEM_register_task(ssd_mgr_main) != 0)
    {
        return -1;
    }

    memcpy(module_config, config, 4);
    module_mode = is_segment_mode;

    return 0;
}
