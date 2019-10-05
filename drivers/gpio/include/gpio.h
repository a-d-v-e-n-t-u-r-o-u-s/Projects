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

#define GPIO_DYNAMIC_CHECK 0U

typedef enum
{
    GPIO_PORTD,
    GPIO_PORTC,
    GPIO_PORTB,
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

#define GPIO_PIN_SHIFT          (2U)
#define GPIO_MODE_SHIFT         (5U)

#define GPIO_PORT_MASK          (0x03U)
#define GPIO_PIN_MASK           (0x1CU)
#define GPIO_MODE_MASK          (0x60U)

#define GPIO_PORTD              (0U)
#define GPIO_PORTC              (1U)
#define GPIO_PORTB              (2U)

#define GPIO_OUTPUT_PUSH_PULL   (0U)
#define GPIO_INPUT_FLOATING     (1U)
#define GPIO_INPUT_PULL_UP      (2U)

#define GPIO_PORT_PIN_CONFIG(port, pin) \
    ((pin << GPIO_PIN_SHIFT) | (port))

#define GPIO_CONFIG(port, pin, mode) \
    ((mode << GPIO_MODE_SHIFT) | GPIO_PORT_PIN_CONFIG(port, pin))

int8_t GPIO_read_pin(uint8_t config, bool *is_high);
int8_t GPIO_write_pin(uint8_t config, bool is_high);
int8_t GPIO_config_pin(uint8_t config);
void GPIO_configure(bool is_global_pullup);

/*@}*/
#endif
