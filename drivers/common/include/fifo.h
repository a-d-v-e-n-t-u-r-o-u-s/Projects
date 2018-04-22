/*!
 * \file
 * \brief FIFO ADT header file
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

#ifndef FIFO_H
#define FIFO_H

/*!
 *
 * \addtogroup fifo
 * \ingroup drivers
 * \brief FIFO ADT implementation
 */

/*@{*/

#include <stdint.h>

typedef struct
{
    uint8_t elements_no;
    uint8_t elements_size;
    uint8_t *buffer;
} FIFO_config_t;

typedef struct
{
    uint8_t head;
    uint8_t tail;
    FIFO_config_t config;
} FIFO_t;

int8_t FIFO_enqueue(FIFO_t *fifo, const void *data);
int8_t FIFO_dequeue(FIFO_t *fifo, const void *data);
uint8_t FIFO_get_free(FIFO_t *fifo);
uint8_t FIFO_get_occupied(FIFO_t *fifo);
void FIFO_configure(FIFO_t *fifo, const FIFO_config_t *config);

/*@}*/
#endif
