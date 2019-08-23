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
#include "gpio.h"

void SSD_light(uint8_t value);
void SSD_set_segment(uint8_t segment, bool value);
int8_t SSD_configure(const uint8_t config[8], bool inverted_logic);

#endif
