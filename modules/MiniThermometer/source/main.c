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
#include "system_timer.h"

static inline void drivers_init(void)
{
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
        .a = { .port = SSD_PORTC, .pin = 0U, },
        .b = { .port = SSD_PORTC, .pin = 1U, },
        .c = { .port = SSD_PORTC, .pin = 2U, },
        .d = { .port = SSD_PORTC, .pin = 3U, },
        .e = { .port = SSD_PORTC, .pin = 4U, },
        .f = { .port = SSD_PORTC, .pin = 5U, },
        .g = { .port = SSD_PORTD, .pin = 6U, },
        .dp = { .port = SSD_PORTD, .pin = 7U, },
    };

    if(SSD_configure(&ssd_config) != 0u)
    {
        DEBUG_output("SSD [fail]\n");
    }
}

static inline void modules_init(void)
{
}

static void callback(void)
{
    static uint32_t tick;
    uint32_t new_tick = SYSTEM_timer_get_tick();
    if(SYSTEM_timer_tick_difference(tick, new_tick) > 1000)
    {
        //PORTD ^= (1 << PD6);
        static uint8_t counter;
        DEBUG_output("Lights %d\n",tick);
        SSD_light(counter);
        tick = new_tick;
        counter++;
        counter %= 10u;
    }
}

int main(void)
{
    drivers_init();
    modules_init();

    //DDRD |= (1 << PD6);

    SYSTEM_init();
    SYSTEM_timer_register(callback);

    DEBUG_output("********************************\n");
    DEBUG_output("******* Mini Thermometer *******\n");
    DEBUG_output("********************************\n");

    while(1)
    {
    }
}
