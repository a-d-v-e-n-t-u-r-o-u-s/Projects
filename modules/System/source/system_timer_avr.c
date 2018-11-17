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
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint32_t system_tick;
static void (*timer_callback)(void);

ISR(TIMER1_COMPB_vect)
{
    system_tick++;

    if(timer_callback != NULL)
    {
        timer_callback();
    }
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
    cli();
    tick = system_tick;
    sei();
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
        return (UINT32_MAX - prev) + next + 1;
    }
}

void SYSTEM_timer_delay(uint8_t val)
{
    (void) val;
}

uint8_t SYSTEM_timer_init(void)
{
    OCR1A = 7999U;
    TCCR1B |= (1<<WGM12)|(1<<CS10);
    TIMSK |= (1<<OCF1B);
    sei();

    return 0;
}
