/*!
 * \file
 * \brief 7 segment display driver header file
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

#ifndef SSD_DISPLAY_H
#define SSD_DISPLAY_H

#include <stdint.h>

typedef struct
{
    uint8_t port;
    uint8_t pin;
} SSD_pin_t;

typedef struct
{
    SSD_pin_t a;
    SSD_pin_t b;
    SSD_pin_t c;
    SSD_pin_t d;
    SSD_pin_t e;
    SSD_pin_t f;
    SSD_pin_t g;
    SSD_pin_t dp;
} SSD_config_t;

int8_t SSD_configure(const SSD_config_t *config);

#endif
