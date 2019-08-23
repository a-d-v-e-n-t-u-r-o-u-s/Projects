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
#include <avr/interrupt.h>
#include <util/delay.h>
#include "system.h"
#include "usart.h"
#include "debug.h"
#include "ssd_display.h"
#include "ssd_mgr.h"
#include "system_timer.h"
#include "gpio.h"
#include "1wire.h"

static inline void drivers_init(void)
{
    uint8_t gpio_config[] =
    {
        GPIO_CONFIG(GPIO_PORTB, 0U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTB, 1U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTB, 2U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTB, 3U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTC, 0U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTC, 1U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTC, 2U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTC, 3U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTC, 4U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTC, 5U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTD, 5U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTD, 6U, GPIO_OUTPUT_PUSH_PULL),
        GPIO_CONFIG(GPIO_PORTD, 7U, GPIO_OUTPUT_PUSH_PULL),
    };

    GPIO_configure(gpio_config, sizeof(gpio_config), true);

    USART_config_t config =
    {
        .baudrate = 250000U,
        .parity = USART_NO_PARITY,
        .stop_bits = USART_1_STOP_BITS,
        .data_bits = USART_8_DATA_BITS,
    };

    (void) USART_configure(&config);

    DEBUG_configure(NULL);

    uint8_t ssd_config[8] =
    {
        GPIO_PORT_PIN_CONFIG(GPIO_PORTC, 0U),
        GPIO_PORT_PIN_CONFIG(GPIO_PORTC, 1U),
        GPIO_PORT_PIN_CONFIG(GPIO_PORTC, 2U),
        GPIO_PORT_PIN_CONFIG(GPIO_PORTC, 3U),
        GPIO_PORT_PIN_CONFIG(GPIO_PORTC, 4U),
        GPIO_PORT_PIN_CONFIG(GPIO_PORTC, 5U),
        GPIO_PORT_PIN_CONFIG(GPIO_PORTD, 6U),
        GPIO_PORT_PIN_CONFIG(GPIO_PORTD, 5U),
    };

    if(SSD_configure(ssd_config, true) != 0)
    {
        DEBUG_output("SSD [fail]\n");
    }

    WIRE_config_t wire_config =
    {
        .data_pin = { .port = GPIO_PORTD, .pin = 7U },
    };

    if(WIRE_configure(&wire_config) != 0)
    {
        DEBUG_output("WIRE [fail]\n");
    }
}

static inline void modules_init(void)
{
    uint8_t ssd_mgr_config[4] =
    {
        GPIO_PORT_PIN_CONFIG(GPIO_PORTB, 0U),
        GPIO_PORT_PIN_CONFIG(GPIO_PORTB, 1U),
        GPIO_PORT_PIN_CONFIG(GPIO_PORTB, 2U),
        GPIO_PORT_PIN_CONFIG(GPIO_PORTB, 3U),
    };

    if(SSD_MGR_initialize(ssd_mgr_config, false) != 0)
    {
        DEBUG_output("SSD MGR [fail]\n");
    }
    DEBUG_output("sizeof void* [%d]\n", sizeof(void*));
}

static uint32_t tick;
static uint16_t counter;
static uint16_t old_counter = UINT16_MAX;

static void callback(void)
{
    /*
     *if(SYSTEM_timer_tick_difference(tick, SYSTEM_timer_get_tick()) > 1000)
     *{
     *    uint8_t hours = (counter/3600u) % 24u;
     *    uint8_t minutes = (counter/60u) % 60u;
     *    uint8_t seconds = counter % 60u;
     *    SSD_MGR_set(minutes*100 + seconds);
     *    tick = SYSTEM_timer_get_tick();
     *    counter++;
     *}
     */
}

/*
 *static bool reset(void)
 *{
 *    bool ret = false;
 *
 *    DDRD |= (1 << PB7);
 *    PORTD &= ~(1 << PB7);
 *    _delay_us(500);
 *
 *    DDRD &= ~(1 << PB7);
 *    _delay_us(45);
 *
 *    ret = ((PIND & (1 << PB7)) == 0U);
 *
 *    _delay_us(470);
 *
 *    return ret;
 *}
 */

/*
 *static void send_bit(bool bit)
 *{
 *    cli();
 *    DDRD |= (1 << PB7);
 *    PORTD &= ~(1 << PB7);
 *    _delay_us(5);
 *
 *    if(bit)
 *    {
 *        DDRD &= ~(1 << PB7);
 *    }
 *
 *    _delay_us(80);
 *    DDRD &= ~(1 << PB7);
 *    sei();
 *}
 */

static bool flag;

static bool read_bit(void)
{
    bool ret = false;
    cli();
    DDRD |= (1 << PB7);
    //PORTD &= ~(1 << PB7);
    _delay_us(2);
    DDRD &= ~(1 << PB7);
    _delay_us(15);

    if(flag)
    {
        PORTD |= (1 << PD5);
        flag = false;
    }
    else
    {
        PORTD &= ~(1 << PD5);
        flag = true;
    }

    ret = (PIND & (1 << PB7)) != 0u;
    sei();
    return ret;
}

#if 0
static void send_byte(uint8_t byte)
{
    //DEBUG_output("Send:");
    for(uint8_t i = 0U; i < 8U; i++)
    {
        const bool bit = ((byte & 0x01U) == 0x01);
        //DEBUG_output("%d",bit);
        send_bit(bit);
        byte >>= 1U;
    }
    _delay_us(100U);
    //DEBUG_output("\n");
}
#endif

static uint8_t read_byte(void)
{
    uint8_t ret = 0U;
    //DEBUG_output("Read:");
    for(uint8_t i = 0U; i < 8U ; i++)
    {
        const bool bit = read_bit();
        //DEBUG_output("%d",bit);
        if(bit)
        {
            ret |= (1U << i);
        }
        _delay_us(15);
    }
    //DEBUG_output("\n");

    return ret;
}

static uint16_t get_converted_fraction(uint16_t value)
{
    uint16_t ret = 0;
    if((value & (1 << 3)) != 0)
    {
        ret += 50;
    }
    if((value & (1 << 2)) != 0)
    {
        ret += 25;
    }

    if((value & (1 << 1)) != 0)
    {
        ret += 13;
    }

    if((value & (1 << 0)) != 0)
    {
        ret += 7;
    }

    return ret;
}

typedef enum
{
    START_CONVERSION,
    WAIT_FOR_CONVERTION,
    READ_CONVERSION_RESULT,
} state_t;

static state_t status;

int main(void)
{
    drivers_init();
    modules_init();

    SYSTEM_init();
    tick = SYSTEM_timer_get_tick();
    SYSTEM_timer_register(callback);

    DEBUG_output("********************************\n");
    DEBUG_output("******* Mini Thermometer *******\n");
    DEBUG_output("********************************\n");

    status = START_CONVERSION;

    uint32_t time = 0;
    while(1)
    {
        SYSTEM_main();

        //DEBUG_output("Status %d\n", status);
        switch(status)
        {
            case START_CONVERSION:
                if(!WIRE_reset())
                {
                    DEBUG_output("No Presence\n");
                }

                WIRE_send_byte(0xCC);
                WIRE_send_byte(0x44);
                status = WAIT_FOR_CONVERTION;
                time = SYSTEM_timer_get_tick();
                break;
            case WAIT_FOR_CONVERTION:
                if(SYSTEM_timer_tick_difference(time,
                            SYSTEM_timer_get_tick()) > 750)
                {
                    status = READ_CONVERSION_RESULT;
                }
                break;
            case READ_CONVERSION_RESULT:
                {
                    WIRE_reset();
                    WIRE_send_byte(0xCC);
                    WIRE_send_byte(0xBE);

                    uint8_t lsb = WIRE_read_byte();
                    uint8_t msb = WIRE_read_byte();
                    uint16_t value = (msb << 8U)|lsb;
                    uint16_t fraction = get_converted_fraction(value % 16);
                    uint16_t integer = (value / 16);
                    uint16_t display = integer * 100;
                    display += fraction;
                    //DEBUG_output("Temp 0x%x%x \n", msb, lsb);
                    DEBUG_output("Temp %d,%d \n", integer, fraction);
                    SSD_MGR_set(display);
                    status = START_CONVERSION;
                }
                break;
        }
    }
}
