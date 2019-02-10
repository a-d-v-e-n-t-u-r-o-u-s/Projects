/*!
 * \file
 * \brief GPIO header file
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

#ifndef GPIO_H
#define GPIO_H

/*!
 *
 * \addtogroup gpio
 * \ingroup drivers
 * \brief GPIO implementation
 */

/*@{*/

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    GPIO_PORTB = 1U,
    GPIO_PORTC = 2U,
    GPIO_PORTD = 3U,
} GPIO_port_t;

typedef enum
{
    GPIO_OUTPUT_PUSH_PULL,
    GPIO_INPUT_FLOATING,
    GPIO_INPUT_PULL_UP,
} GPIO_mode_t;

typedef struct
{
    GPIO_port_t port;
    uint8_t pin;
} GPIO_data_t;

int8_t GPIO_write_pin(const GPIO_data_t *data, bool is_high);
int8_t GPIO_config_pin(GPIO_mode_t mode, const GPIO_data_t *data);
void GPIO_configure(bool is_global_pullup);

/*@}*/
#endif
