/*!
 * \file
 * \brief 7 segment display driver implementation file
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

#include "ssd_display.h"
#include <stddef.h>
#include <avr/io.h>

static SSD_config_t driver_config;

static volatile uint8_t *get_port_config_register(uint8_t port)
{
    switch(port)
    {
        case SSD_PORTB:
            return &DDRB;
        case SSD_PORTC:
            return &DDRC;
        case SSD_PORTD:
            return &DDRD;
        default:
            return NULL;
    }
}

static volatile uint8_t *get_port_status_register(uint8_t port)
{
    switch(port)
    {
        case SSD_PORTB:
            return &PORTB;
        case SSD_PORTC:
            return &PORTC;
        case SSD_PORTD:
            return &PORTD;
        default:
            return NULL;
    }
}

static int8_t pin_configure(const SSD_pin_t data)
{
    volatile uint8_t *port_cfg_reg = get_port_config_register(data.port);

    if(port_cfg_reg == NULL)
    {
        return -1;
    }

    *port_cfg_reg |= (1 << data.pin);
    return 0;
}

static inline void clear(void)
{
    volatile uint8_t *port_status_reg = get_port_status_register(driver_config.a.port);
    *port_status_reg &= ~(1 << driver_config.a.pin);
    port_status_reg = get_port_status_register(driver_config.b.port);
    *port_status_reg &= ~(1 << driver_config.b.pin);
    port_status_reg = get_port_status_register(driver_config.c.port);
    *port_status_reg &= ~(1 << driver_config.c.pin);
    port_status_reg = get_port_status_register(driver_config.d.port);
    *port_status_reg &= ~(1 << driver_config.d.pin);
    port_status_reg = get_port_status_register(driver_config.e.port);
    *port_status_reg &= ~(1 << driver_config.e.pin);
    port_status_reg = get_port_status_register(driver_config.f.port);
    *port_status_reg &= ~(1 << driver_config.f.pin);
    port_status_reg = get_port_status_register(driver_config.g.port);
    *port_status_reg &= ~(1 << driver_config.g.pin);
    port_status_reg = get_port_status_register(driver_config.dp.port);
    *port_status_reg &= ~(1 << driver_config.dp.pin);
}

static inline void set0(void)
{
    volatile uint8_t *port_status_reg = get_port_status_register(driver_config.a.port);
    *port_status_reg |= (1 << driver_config.a.pin);
    port_status_reg = get_port_status_register(driver_config.b.port);
    *port_status_reg |= (1 << driver_config.b.pin);
    port_status_reg = get_port_status_register(driver_config.c.port);
    *port_status_reg |= (1 << driver_config.c.pin);
    port_status_reg = get_port_status_register(driver_config.d.port);
    *port_status_reg |= (1 << driver_config.d.pin);
    port_status_reg = get_port_status_register(driver_config.e.port);
    *port_status_reg |= (1 << driver_config.e.pin);
    port_status_reg = get_port_status_register(driver_config.f.port);
    *port_status_reg |= (1 << driver_config.f.pin);
}

static inline void set1(void)
{
    volatile uint8_t *port_status_reg = get_port_status_register(driver_config.b.port);
    *port_status_reg |= (1 << driver_config.b.pin);
    port_status_reg = get_port_status_register(driver_config.c.port);
    *port_status_reg |= (1 << driver_config.c.pin);
}

static inline void set2(void)
{
    volatile uint8_t *port_status_reg = get_port_status_register(driver_config.a.port);
    *port_status_reg |= (1 << driver_config.a.pin);
    port_status_reg = get_port_status_register(driver_config.b.port);
    *port_status_reg |= (1 << driver_config.b.pin);
    port_status_reg = get_port_status_register(driver_config.d.port);
    *port_status_reg |= (1 << driver_config.d.pin);
    port_status_reg = get_port_status_register(driver_config.e.port);
    *port_status_reg |= (1 << driver_config.e.pin);
    port_status_reg = get_port_status_register(driver_config.g.port);
    *port_status_reg |= (1 << driver_config.g.pin);
}

static inline void set3(void)
{
    volatile uint8_t *port_status_reg = get_port_status_register(driver_config.a.port);
    *port_status_reg |= (1 << driver_config.a.pin);
    port_status_reg = get_port_status_register(driver_config.b.port);
    *port_status_reg |= (1 << driver_config.b.pin);
    port_status_reg = get_port_status_register(driver_config.c.port);
    *port_status_reg |= (1 << driver_config.c.pin);
    port_status_reg = get_port_status_register(driver_config.d.port);
    *port_status_reg |= (1 << driver_config.d.pin);
    port_status_reg = get_port_status_register(driver_config.g.port);
    *port_status_reg |= (1 << driver_config.g.pin);
}

static inline void set4(void)
{
    volatile uint8_t *port_status_reg = get_port_status_register(driver_config.b.port);
    *port_status_reg |= (1 << driver_config.b.pin);
    port_status_reg = get_port_status_register(driver_config.c.port);
    *port_status_reg |= (1 << driver_config.c.pin);
    port_status_reg = get_port_status_register(driver_config.f.port);
    *port_status_reg |= (1 << driver_config.f.pin);
    port_status_reg = get_port_status_register(driver_config.g.port);
    *port_status_reg |= (1 << driver_config.g.pin);
}

static inline void set5(void)
{
    volatile uint8_t *port_status_reg = get_port_status_register(driver_config.a.port);
    *port_status_reg |= (1 << driver_config.a.pin);
    port_status_reg = get_port_status_register(driver_config.c.port);
    *port_status_reg |= (1 << driver_config.c.pin);
    port_status_reg = get_port_status_register(driver_config.d.port);
    *port_status_reg |= (1 << driver_config.d.pin);
    port_status_reg = get_port_status_register(driver_config.f.port);
    *port_status_reg |= (1 << driver_config.f.pin);
    port_status_reg = get_port_status_register(driver_config.g.port);
    *port_status_reg |= (1 << driver_config.g.pin);
}

static inline void set6(void)
{
    volatile uint8_t *port_status_reg = get_port_status_register(driver_config.a.port);
    *port_status_reg |= (1 << driver_config.a.pin);
    port_status_reg = get_port_status_register(driver_config.c.port);
    *port_status_reg |= (1 << driver_config.c.pin);
    port_status_reg = get_port_status_register(driver_config.d.port);
    *port_status_reg |= (1 << driver_config.d.pin);
    port_status_reg = get_port_status_register(driver_config.e.port);
    *port_status_reg |= (1 << driver_config.e.pin);
    port_status_reg = get_port_status_register(driver_config.f.port);
    *port_status_reg |= (1 << driver_config.f.pin);
    port_status_reg = get_port_status_register(driver_config.g.port);
    *port_status_reg |= (1 << driver_config.g.pin);
}

static inline void set7(void)
{
    volatile uint8_t *port_status_reg = get_port_status_register(driver_config.a.port);
    *port_status_reg |= (1 << driver_config.a.pin);
    port_status_reg = get_port_status_register(driver_config.b.port);
    *port_status_reg |= (1 << driver_config.b.pin);
    port_status_reg = get_port_status_register(driver_config.c.port);
    *port_status_reg |= (1 << driver_config.c.pin);
}

static inline void set8(void)
{
    volatile uint8_t *port_status_reg = get_port_status_register(driver_config.a.port);
    *port_status_reg |= (1 << driver_config.a.pin);
    port_status_reg = get_port_status_register(driver_config.b.port);
    *port_status_reg |= (1 << driver_config.b.pin);
    port_status_reg = get_port_status_register(driver_config.c.port);
    *port_status_reg |= (1 << driver_config.c.pin);
    port_status_reg = get_port_status_register(driver_config.d.port);
    *port_status_reg |= (1 << driver_config.d.pin);
    port_status_reg = get_port_status_register(driver_config.e.port);
    *port_status_reg |= (1 << driver_config.e.pin);
    port_status_reg = get_port_status_register(driver_config.f.port);
    *port_status_reg |= (1 << driver_config.f.pin);
    port_status_reg = get_port_status_register(driver_config.g.port);
    *port_status_reg |= (1 << driver_config.g.pin);
}

static inline void set9(void)
{
    volatile uint8_t *port_status_reg = get_port_status_register(driver_config.a.port);
    *port_status_reg |= (1 << driver_config.a.pin);
    port_status_reg = get_port_status_register(driver_config.b.port);
    *port_status_reg |= (1 << driver_config.b.pin);
    port_status_reg = get_port_status_register(driver_config.c.port);
    *port_status_reg |= (1 << driver_config.c.pin);
    port_status_reg = get_port_status_register(driver_config.d.port);
    *port_status_reg |= (1 << driver_config.d.pin);
    port_status_reg = get_port_status_register(driver_config.f.port);
    *port_status_reg |= (1 << driver_config.f.pin);
    port_status_reg = get_port_status_register(driver_config.g.port);
    *port_status_reg |= (1 << driver_config.g.pin);
}

void SSD_light(uint8_t value)
{
    clear();

    switch(value)
    {
        case 0:
            set0();
            break;
        case 1:
            set1();
            break;
        case 2:
            set2();
            break;
        case 3:
            set3();
            break;
        case 4:
            set4();
            break;
        case 5:
            set5();
            break;
        case 6:
            set6();
            break;
        case 7:
            set7();
            break;
        case 8:
            set8();
            break;
        case 9:
            set9();
            break;
        case UINT8_MAX:
            break;
        default:
            break;
    }
}

int8_t SSD_configure(const SSD_config_t *config)
{
    if(config == NULL)
    {
        return -1;
    }

    const SSD_pin_t *pin_config = (const SSD_pin_t *) config;

    for(uint8_t i = 0; i<8u; i++)
    {
        if(pin_configure(*pin_config) != 0)
        {
            return -1;
        }
        pin_config++;
    }

    driver_config = *config;

    return 0;
}
