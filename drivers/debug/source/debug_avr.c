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
#include <stdarg.h>
#include <stdio.h>

#define DEBUG_BUFFER_SIZE  UINT8_MAX

void DEBUG_output(const char *format, ...)
{
    char debug_buffer[DEBUG_BUFFER_SIZE] = {0};

    va_list args;
    va_start( args, format);
    int written = vsnprintf(debug_buffer, DEBUG_BUFFER_SIZE, format, args);
    va_end(args);

    for(uint8_t i = 0; i < written ; i++)
    {
        USART_transmit((uint8_t)debug_buffer[i]);
    }
}


void DEBUG_configure(const DEBUG_config_t *config)
{
    (void) config;
}
