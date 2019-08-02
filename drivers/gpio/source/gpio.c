/*!
 * \file
 * \brief GPIO implementation file
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

#include "gpio.h"
#include <stddef.h>
#include <stdbool.h>
#include <avr/io.h>

#define REG_BY_PORT(base_reg, port_no, port_offset)  \
    *((volatile uint8_t *)((volatile uint8_t *)&base_reg + port_no * port_offset))

#define DDR(port_no)    REG_BY_PORT(DDRD, port_no, 0x03)
#define PORT(port_no)   REG_BY_PORT(PORTD, port_no, 0x03)
#define PIN(port_no)    REG_BY_PORT(PIND, port_no, 0x03)

static inline bool is_port_valid(GPIO_port_t port)
{
    switch(port)
    {
        case GPIO_PORTB:
        case GPIO_PORTC:
        case GPIO_PORTD:
            return true;
        default:
            return false;
    }
}

static inline bool is_pin_valid(const GPIO_data_t *data)
{
    switch(data->port)
    {
        case GPIO_PORTB:
            /* no break */
        case GPIO_PORTD:
            if(data->pin > 7U)
            {
                return false;
            }
            break;
        case GPIO_PORTC:
            if(data->pin > 6U)
            {
                return false;
            }
            break;
        default:
            return false;
    }

    return true;
}

static inline bool is_mode_valid(GPIO_mode_t mode)
{
    switch(mode)
    {
        case GPIO_OUTPUT_PUSH_PULL:
            /* no break */
        case GPIO_INPUT_FLOATING:
            /* no break */
        case GPIO_INPUT_PULL_UP:
            return true;
        default:
            return false;
    }
}

int8_t GPIO_read_pin(uint8_t config, bool *is_high)
{
#if GPIO_DYNAMIC_CHECK == 1U
    if(data == NULL)
    {
        return -1;
    }

    if(is_high == NULL)
    {
        return -1;
    }

    if(!is_port_valid(data->port))
    {
        return -1;
    }

    if(!is_pin_valid(data))
    {
        return -1;
    }
#endif

    const uint8_t port = config & GPIO_PORT_MASK;
    const uint8_t pin = ((config & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT);

    *is_high = ((PIN(port) & ( 1 << pin)) != 0);

    return 0;
}

int8_t GPIO_write_pin(uint8_t config, bool is_high)
{
#if GPIO_DYNAMIC_CHECK == 1U
    if(data == NULL)
    {
        return -1;
    }

    if(!is_port_valid(data->port))
    {
        return -1;
    }

    if(!is_pin_valid(data))
    {
        return -1;
    }
#endif
    const uint8_t port = config & GPIO_PORT_MASK;
    const uint8_t pin = ((config & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT);

    if(is_high)
    {
        PORT(port) |= (1 << pin);
    }
    else
    {
        PORT(port)  &= ~(1 << pin);
    }

    return 0;
}

int8_t GPIO_config_pin(uint8_t config)
{
#if GPIO_DYNAMIC_CHECK == 1U
    if(data == NULL)
    {
        return -1;
    }

    if(!is_mode_valid(mode))
    {
        return -1;
    }

    if(!is_port_valid(data->port))
    {
        return -1;
    }

    if(!is_pin_valid(data))
    {
        return -1;
    }
#endif
    const uint8_t port = config & GPIO_PORT_MASK;
    const uint8_t pin = ((config & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT);
    const uint8_t mode = ((config & GPIO_MODE_MASK) >> GPIO_MODE_SHIFT);

    DDR(port) &= ~(1 << pin);
    PORT(port)  &= ~(1 << pin);

    switch(mode)
    {
        case GPIO_OUTPUT_PUSH_PULL:
            DDR(port) |= (1 << pin);
            break;
        case GPIO_INPUT_FLOATING:
            break;
        case GPIO_INPUT_PULL_UP:
            PORT(port) |= (1 << pin);
            break;
    }

    return 0;
}

void GPIO_configure(const uint8_t *config, uint8_t size, bool is_global_pullup)
{
    if(is_global_pullup)
    {
        SFIOR |= (1 << PUD);
    }
    else
    {
        SFIOR &= ~(1 << PUD);
    }

    for(uint8_t i = 0; i < size; i++, config++)
    {
        GPIO_config_pin(*config);
    }
}
