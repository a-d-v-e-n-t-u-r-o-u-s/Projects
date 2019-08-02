/*!
 * \file
 * \brief Seven segment display driver manager header file
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

#ifndef SSD_MGR_H
#define SSD_MGR_H

#include <stdint.h>
#include "gpio.h"

#define SSD_MGR_MAX_MULTIPLEXED_DISPLAYS        4U

int8_t SSD_MGR_set(uint16_t value);
int8_t SSD_MGR_initialize(const uint8_t config[4],
        bool is_segment_mode);
#endif
