/*!
 * \file
 * \brief 1-WIRE implementation file
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

#include "1wire.h"
#include <stddef.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static WIRE_config_t driver_config;

static void send_bit(bool bit)
{
    cli();
    DDRD |= (1 << PB7);
    PORTD &= ~(1 << PB7);
    _delay_us(5);

    if(bit)
    {
        DDRD &= ~(1 << PB7);
    }

    _delay_us(80);
    DDRD &= ~(1 << PB7);
    sei();
}

static bool read_bit(void)
{
    bool ret = false;
    cli();
    DDRD |= (1 << PB7);
    _delay_us(2);
    DDRD &= ~(1 << PB7);
    _delay_us(15);

    ret = (PIND & (1 << PB7)) != 0u;
    sei();
    return ret;
}

bool WIRE_reset(void)
{
    bool ret = false;

    DDRD |= (1 << PB7);
    PORTD &= ~(1 << PB7);
    _delay_us(500);

    DDRD &= ~(1 << PB7);
    _delay_us(45);

    ret = ((PIND & (1 << PB7)) == 0U);

    _delay_us(470);

    return ret;
}

void WIRE_send_byte(uint8_t byte)
{
    for(uint8_t i = 0U; i < 8U; i++)
    {
        const bool bit = ((byte & 0x01U) == 0x01);
        send_bit(bit);
        byte >>= 1U;
    }
    _delay_us(100U);
}

uint8_t WIRE_read_byte(void)
{
    uint8_t ret = 0U;
    for(uint8_t i = 0U; i < 8U ; i++)
    {
        const bool bit = read_bit();
        if(bit)
        {
            ret |= (1U << i);
        }
        _delay_us(15);
    }
    return ret;
}

int8_t WIRE_configure(const WIRE_config_t *config)
{
    if(config == NULL)
    {
        return -1;
    }

    driver_config = *config;

    return 0;
}
