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
#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>
#include "system.h"
#include "system_timer.h"

static inline void drivers_init(void)
{
}

static inline void modules_init(void)
{
}

static void callback(void)
{
    static uint32_t tick;
    uint32_t new_tick = SYSTEM_timer_get_tick();
    if(SYSTEM_timer_tick_difference(tick, new_tick) > 1000)
    {
        PORTD ^= (1 << PD6);
        tick = new_tick;
    }
}


int main(void)
{
    drivers_init();
    modules_init();

    DDRD |= (1 << PD6);

    SYSTEM_init();
    SYSTEM_timer_register(callback);

    while(1)
    {
    }
}
