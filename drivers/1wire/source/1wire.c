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

static WIRE_config_t driver_config;

static bool reset(void)
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

static bool flag;

static bool read_bit(void)
{
    bool ret = false;
    cli();
    DDRD |= (1 << PB7);
    //PORTD &= ~(1 << PB7);
    _delay_us(2);
    DDRD &= ~(1 << PB7);
    _delay_us(15);

    if(flag)
    {
        PORTD |= (1 << PD5);
        flag = false;
    }
    else
    {
        PORTD &= ~(1 << PD5);
        flag = true;
    }

    ret = (PIND & (1 << PB7)) != 0u;
    sei();
    return ret;
}

static void send_byte(uint8_t byte)
{
    //DEBUG_output("Send:");
    for(uint8_t i = 0U; i < 8U; i++)
    {
        const bool bit = ((byte & 0x01U) == 0x01);
        //DEBUG_output("%d",bit);
        send_bit(bit);
        byte >>= 1U;
    }
    _delay_us(100U);
    //DEBUG_output("\n");
}

static uint8_t read_byte(void)
{
    uint8_t ret = 0U;
    //DEBUG_output("Read:");
    for(uint8_t i = 0U; i < 8U ; i++)
    {
        const bool bit = read_bit();
        //DEBUG_output("%d",bit);
        if(bit)
        {
            ret |= (1U << i);
        }
        _delay_us(15);
    }
    //DEBUG_output("\n");

    return ret;
}

static inline bool is_mode_valid(WIRE_mode_t mode)
{
    switch(mode)
    {
        case WIRE_9BIT_MODE:
            /* no break */
        case WIRE_10BIT_MODE:
            /* no break */
        case WIRE_11BIT_MODE:
            /* no break */
        case WIRE_12BIT_MODE:
            return true;
        default:
            return false;
    }
}

int8_t WIRE_configure(const WIRE_config_t *config)
{
    if(config == NULL)
    {
        return -1;
    }

    if(!is_mode_valid(config->mode))
    {
        return -1;
    }

    driver_config = *config;

    return 0;
}
