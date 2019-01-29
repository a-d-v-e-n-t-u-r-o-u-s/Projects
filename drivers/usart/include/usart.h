/*!
 * \file
 * \brief Usart driver
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

#ifndef USART_H
#define USART_H

#include <stdint.h>

typedef enum
{
    USART_NO_PARITY,
    USART_EVEN_PARITY,
    USART_ODD_PARITY,
} USART_parity_t;

typedef enum
{
    USART_1_STOP_BITS,
    USART_2_STOP_BITS
} USART_stop_bits_t;

typedef enum
{
    USART_5_DATA_BITS,
    USART_6_DATA_BITS,
    USART_7_DATA_BITS,
    USART_8_DATA_BITS,
    USART_9_DATA_BITS
} USART_data_bits_t;

typedef struct
{
    uint32_t baudrate;
    USART_parity_t parity;
    USART_stop_bits_t stop_bits;
    USART_data_bits_t data_bits;
} USART_config_t;

void USART_transmit(uint8_t data);
int8_t USART_configure(const USART_config_t *config);


#endif
