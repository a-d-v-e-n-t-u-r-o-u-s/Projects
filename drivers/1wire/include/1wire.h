/*!
 * \file
 * \brief 1-WIRE header file
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

#ifndef IWIRE_H
#define IWIRE_H

/*!
 *
 * \addtogroup 1wire
 * \ingroup drivers
 * \brief 1-WIRE implementation
 */

/*@{*/
#include "gpio.h"

bool WIRE_reset(void);
uint8_t WIRE_read_byte(void);
void WIRE_send_byte(uint8_t byte);
int8_t WIRE_configure(uint8_t port, uint8_t pin);

/*@}*/
#endif
