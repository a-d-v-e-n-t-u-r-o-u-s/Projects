/*!
 * \file
 * \brief DS1302 header file
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

#ifndef DS1302_H
#define DS1302_H

/*!
 *
 * \addtogroup ds1302
 * \ingroup drivers
 * \brief DS1302 implementation
 */

/*@{*/
#include <stdint.h>

typedef struct
{
    uint8_t pins[3][2];
} DS1302_config_t;

int8_t DS1302_configure(const DS1302_config_t *config);

/*@}*/
#endif
