/*!
 * \file
 * \brief Application implementation file
 * \author Dawid Babula
 * \email dbabula@adventurous.pl
 *
 * \par Copyright (C) Dawid Babula, 2020
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
#include "app.h"
#include "system.h"
#include "system_timer.h"
#include "1wire_mgr.h"
#include "ssd_mgr.h"
#include "debug.h"
#include <stdbool.h>

static uint32_t tick;
static uint16_t counter;
static uint8_t hours = 17;
static uint8_t minutes = 48;

static uint16_t get_converted_fraction(uint16_t value)
{
    uint16_t ret = 0;
    if((value & (1 << 3)) != 0)
    {
        ret += 50;
    }
    if((value & (1 << 2)) != 0)
    {
        ret += 25;
    }

    if((value & (1 << 1)) != 0)
    {
        ret += 13;
    }

    if((value & (1 << 0)) != 0)
    {
        ret += 7;
    }

    return ret;
}

static void callback(void)
{
    if(SYSTEM_timer_tick_difference(tick, SYSTEM_timer_get_tick()) > 1000)
    {
        tick = SYSTEM_timer_get_tick();
        counter++;
    }
}

static void app_main(void)
{
    static bool flag;

    if(!flag)
    {
        hours += (counter/3600u) % 24u;
        minutes += (counter/60u) % 60u;
        SSD_MGR_set(hours*100 + minutes);
        flag = true;
    }
    else
    {
        uint16_t temperature = WIRE_MGR_get_temperature();
        uint16_t fraction = get_converted_fraction(temperature % 16);
        uint16_t integer = (temperature / 16);
        uint16_t display = integer * 100;
        display += fraction;
        DEBUG_output("Temp %d,%d \n", integer, fraction);
        SSD_MGR_set(display);
        flag = false;
    }
}

int8_t APP_initialize(void)
{
    if(SYSTEM_register_task(app_main, 10000u) != 0)
    {
        return -1;
    }

    SYSTEM_timer_register(callback);

    return 0;
}
