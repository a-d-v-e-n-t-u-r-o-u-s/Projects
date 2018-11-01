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

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

static inline void drivers_init(void)
{
}

static inline void modules_init(void)
{
}

int main(void)
{
    drivers_init();
    modules_init();

    DDRD |= (1 << PD6);

    while(1)
    {
        PORTD ^= (1 << PB6);
        _delay_ms(1000);
    }
}
