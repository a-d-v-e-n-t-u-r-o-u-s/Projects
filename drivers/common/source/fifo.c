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

#include "fifo.h"
#include <string.h>
#include "stm8l15x.h"

static INLINE_SPECIFIER int8_t is_fifo_full(FIFO_t *fifo)
{
    if((fifo->head - fifo->tail) == fifo->config.elements_no)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t FIFO_enqueue(FIFO_t *fifo, const void *data)
{
    uint8_t offset = 0U;

    if(is_fifo_full(fifo) == 0)
    {
        return -1;
    }

    offset = (fifo->head & (fifo->config.elements_no - 1)) * fifo->config.elements_size;
    memcpy(&fifo->config.buffer[offset], data, fifo->config.elements_size);
    fifo->head++;
    return 0;
}

static INLINE_SPECIFIER int8_t is_fifo_empty(FIFO_t *fifo)
{
    if(fifo->head == fifo->tail)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t FIFO_dequeue(FIFO_t *fifo, void *data)
{
    uint8_t offset = 0U;

    if(is_fifo_empty(fifo) == 0)
    {
        return -1;
    }

    offset = (fifo->tail & (fifo->config.elements_no - 1)) * fifo->config.elements_size;
    memcpy(data, &fifo->config.buffer[offset], fifo->config.elements_size);
    fifo->tail++;
    return 0;
}

uint8_t FIFO_get_occupied(FIFO_t *fifo)
{
    uint8_t head = fifo->head;
    uint8_t tail = fifo->tail;

    if(head >= tail)
    {
        return (head - tail);
    }
    else
    {
        return ((UINT8_MAX - tail) + head + 1);
    }
}

uint8_t FIFO_get_free(FIFO_t *fifo)
{
    return fifo->config.elements_no - FIFO_get_occupied(fifo);
}

void FIFO_configure(FIFO_t *fifo, const FIFO_config_t *config)
{
    memset(config->buffer, 0, config->elements_no * config->elements_size);
    fifo->config.elements_no = config->elements_no;
    fifo->config.elements_size = config->elements_size;
    fifo->config.buffer = config->buffer;
}
