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
#include "debug.h"
#include "usart.h"

int8_t DEBUG_write(const uint8_t *data, uint8_t length)
{
    for(uint8_t i = 0; i < length; i++)
    {
        USART_transmit(*data++);
    }

    return 0;
}

void DEBUG_configure(const DEBUG_config_t *config)
{
    (void) config;
}
