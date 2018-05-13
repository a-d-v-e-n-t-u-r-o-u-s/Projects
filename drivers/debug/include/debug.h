/*!
 * \file
 * \brief Debug driver
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
#ifndef DEBUG_H
#define DEBUG_H

/*!
 *
 * \defgroup drivers
 * \addtogroup debug
 * \ingroup drivers
 * \brief Driver for debug
 */

/*@{*/

#include "stm8l15x_usart.h"

typedef struct
{
    USART_TypeDef *unit;
    CLK_Peripheral_TypeDef clk;
    GPIO_TypeDef* pullup_port;
    uint8_t pullup_pin;
    uint32_t baudrate;
    USART_WordLength_TypeDef word_length;
    USART_StopBits_TypeDef stop_bits;
    USART_Parity_TypeDef parity;
    USART_Mode_TypeDef mode;
} DEBUG_config_t;

int putchar(int c);
int8_t DEBUG_write(const uint8_t *data, uint8_t length);
void DEBUG_configure(const DEBUG_config_t *config);

/*@}*/
#endif
