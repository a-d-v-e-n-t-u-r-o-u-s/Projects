/*!
 * \file
 * \brief Hardware configuration file
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
#include <stdint.h>
#include <stddef.h>
#include <avr/io.h>
#include <util/delay.h>
#include "system.h"
#include "usart.h"
#include "debug.h"
#include "ssd_display.h"
#include "ssd_mgr.h"
#include "system_timer.h"
#include "gpio.h"

static inline void drivers_init(void)
{
    GPIO_configure(true);

    USART_config_t config =
    {
        .baudrate = 250000U,
        .parity = USART_NO_PARITY,
        .stop_bits = USART_1_STOP_BITS,
        .data_bits = USART_8_DATA_BITS,
    };

    (void) USART_configure(&config);

    DEBUG_configure(NULL);

    SSD_config_t ssd_config =
    {
        .a = { .port = GPIO_PORTC, .pin = 0U, },
        .b = { .port = GPIO_PORTC, .pin = 1U, },
        .c = { .port = GPIO_PORTC, .pin = 2U, },
        .d = { .port = GPIO_PORTC, .pin = 3U, },
        .e = { .port = GPIO_PORTC, .pin = 4U, },
        .f = { .port = GPIO_PORTC, .pin = 5U, },
        .g = { .port = GPIO_PORTD, .pin = 6U, },
        .dp = { .port = GPIO_PORTD, .pin = 7U, },
    };

    if(SSD_configure(&ssd_config) != 0)
    {
        DEBUG_output("SSD [fail]\n");
    }
}

static inline void modules_init(void)
{
    SSD_MGR_config_t ssd_mgr_config =
    {
        .config =
        {
            [0] = { .port = GPIO_PORTB, .pin = 0u },
            [1] = { .port = GPIO_PORTB, .pin = 1u },
            [2] = { .port = GPIO_PORTB, .pin = 2u },
            [3] = { .port = GPIO_PORTB, .pin = 3u },
        },
        .size = 4u,
    };

    if(SSD_MGR_initialize(&ssd_mgr_config) != 0)
    {
        DEBUG_output("SSD MGR [fail]\n");
    }
}

static uint32_t tick;

static void callback(void)
{
    static uint16_t counter;

    if(SYSTEM_timer_tick_difference(tick, SYSTEM_timer_get_tick()) > 1000)
    {
        uint8_t hours = (counter/3600u) % 24u;
        uint8_t minutes = (counter/60u) % 60u;
        uint8_t seconds = counter % 60u;
        SSD_MGR_set(minutes*100 + seconds);
        tick = SYSTEM_timer_get_tick();
        counter++;
    }
}

int main(void)
{
    drivers_init();
    modules_init();

    //DDRB |= ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3));
    //DDRB |= (1 << PB0);
    //PORTB |= ((1 << PB0) | (1 << PB1));
    //PORTB |= (1 << PB0);

    SYSTEM_init();
    tick = SYSTEM_timer_get_tick();
    SYSTEM_timer_register(callback);

    DEBUG_output("********************************\n");
    DEBUG_output("******* Mini Thermometer *******\n");
    DEBUG_output("********************************\n");

    while(1)
    {
        SYSTEM_main();
    }
}
