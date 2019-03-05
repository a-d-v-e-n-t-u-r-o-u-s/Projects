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


static volatile uint8_t *get_port_config_register(GPIO_port_t port)
{
    volatile uint8_t *reg = NULL;

    switch(port)
    {
        case GPIO_PORTB:
            reg = &DDRB;
            break;
        case GPIO_PORTC:
            reg = &DDRC;
            break;
        case GPIO_PORTD:
            reg = &DDRD;
            break;
    }

    return reg;
}

static volatile uint8_t *get_port_status_register(GPIO_port_t port)
{
    volatile uint8_t *reg = NULL;

    switch(port)
    {
        case GPIO_PORTB:
            reg = &PORTB;
            break;
        case GPIO_PORTC:
            reg = &PORTC;
            break;
        case GPIO_PORTD:
            reg = &PORTD;
            break;
    }

    return reg;
}

static volatile uint8_t *get_input_pin_register(GPIO_port_t port)
{
    volatile uint8_t *reg = NULL;

    switch(port)
    {
        case GPIO_PORTB:
            reg = &PINB;
            break;
        case GPIO_PORTC:
            reg = &PINC;
            break;
        case GPIO_PORTD:
            reg = &PIND;
            break;
    }

    return reg;
}

int8_t GPIO_read_pin(const GPIO_data_t *data, bool *is_high)
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

    volatile uint8_t *input_pin_reg = get_input_pin_register(data->port);

    *is_high = (((*input_pin_reg) & ( 1 << data->pin)) != 0);

    return 0;
}

int8_t GPIO_write_pin(const GPIO_data_t *data, bool is_high)
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

    volatile uint8_t *port_status_reg = get_port_status_register(data->port);

    if(is_high)
    {
        *port_status_reg |= (1 << data->pin);
    }
    else
    {
        *port_status_reg &= ~(1 << data->pin);
    }

    return 0;
}

int8_t GPIO_config_pin(GPIO_mode_t mode, const GPIO_data_t *data)
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

    volatile uint8_t *port_cfg_reg = get_port_config_register(data->port);
    volatile uint8_t *port_status_reg = get_port_status_register(data->port);

    /*! \todo think about intermediate steps */
    *port_cfg_reg &= ~(1 << data->pin);
    *port_status_reg &= ~(1 << data->pin);

    switch(mode)
    {
        case GPIO_OUTPUT_PUSH_PULL:
            *port_cfg_reg |= (1 << data->pin);
            break;
        case GPIO_INPUT_FLOATING:
            break;
        case GPIO_INPUT_PULL_UP:
            *port_status_reg |= (1 << data->pin);
            break;
    }

    return 0;
}

void GPIO_configure(bool is_global_pullup)
{
    if(is_global_pullup)
    {
        SFIOR |= (1 << PUD);
    }
    else
    {
        SFIOR &= ~(1 << PUD);
    }
}
