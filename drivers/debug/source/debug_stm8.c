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
#include "common.h"
#include "hardware.h"
#include "fifo.h"

#define FIFO_SIZE           1024

static uint8_t fifo_buffer[FIFO_SIZE];
static FIFO_t fifo;

INTERRUPT_HANDLER(DEBUG_usart2_tx, 19)
{
    uint8_t data;

    USART_ClearFlag(USART2, USART_IT_TC);

    if(FIFO_dequeue(&fifo, &data) == 0)
    {
        USART_SendData8(USART2, data);
    }
    else
    {
        USART_Cmd(USART2, DISABLE);
    }
}

int putchar(int c)
{
    uint8_t tmp = c;
    DEBUG_write(&tmp, 1);
    return c;
}

int8_t DEBUG_write(const uint8_t *data, uint8_t length)
{
    FIFO_elements_no_t len = FIFO_get_free(&fifo);
    uint8_t tmp = 0;

    if(len < length)
    {
        return -1;
    }

    for(uint8_t i = 0; i < length; i++)
    {
        if(FIFO_enqueue(&fifo, &data[i]) != 0)
        {
            return -1;
        }
    }

    disableInterrupts();
    tmp = (USART2->CR1 & USART_CR1_USARTD);
    enableInterrupts();

    if(tmp != 0)
    {
        USART_Cmd(USART2, ENABLE);
    }

    return 0;
}

void DEBUG_configure(const DEBUG_config_t *config)
{
    const FIFO_config_t fifo_config =
    {
        .elements_no = sizeof(fifo_buffer)/sizeof(fifo_buffer[0]),
        .elements_size = 1,
        .buffer = &fifo_buffer[0],
    };

    FIFO_configure(&fifo, &fifo_config);

    disableInterrupts();
    USART_DeInit(config->unit);
    CLK_PeripheralClockConfig(config->clk, ENABLE);

    USART_Init(config->unit, config->baudrate, config->word_length,
            config->stop_bits, config->parity, config->mode);
    USART_ClearFlag(USART2, USART_IT_TC);
    USART_ITConfig(config->unit, USART_IT_TC, ENABLE);
    enableInterrupts();
    USART_Cmd(USART2, DISABLE);
}
