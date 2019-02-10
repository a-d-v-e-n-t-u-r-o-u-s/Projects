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

static inline void clear(void)
{
    GPIO_write_pin(&driver_config.a, false);
    GPIO_write_pin(&driver_config.b, false);
    GPIO_write_pin(&driver_config.c, false);
    GPIO_write_pin(&driver_config.d, false);
    GPIO_write_pin(&driver_config.e, false);
    GPIO_write_pin(&driver_config.f, false);
    GPIO_write_pin(&driver_config.g, false);
    GPIO_write_pin(&driver_config.dp, false);
}

static inline void set0(void)
{
    GPIO_write_pin(&driver_config.a, true);
    GPIO_write_pin(&driver_config.b, true);
    GPIO_write_pin(&driver_config.c, true);
    GPIO_write_pin(&driver_config.d, true);
    GPIO_write_pin(&driver_config.e, true);
    GPIO_write_pin(&driver_config.f, true);
}

static inline void set1(void)
{
    GPIO_write_pin(&driver_config.b, true);
    GPIO_write_pin(&driver_config.c, true);
}

static inline void set2(void)
{
    GPIO_write_pin(&driver_config.a, true);
    GPIO_write_pin(&driver_config.b, true);
    GPIO_write_pin(&driver_config.d, true);
    GPIO_write_pin(&driver_config.e, true);
    GPIO_write_pin(&driver_config.g, true);
}

static inline void set3(void)
{
    GPIO_write_pin(&driver_config.a, true);
    GPIO_write_pin(&driver_config.b, true);
    GPIO_write_pin(&driver_config.c, true);
    GPIO_write_pin(&driver_config.d, true);
    GPIO_write_pin(&driver_config.g, true);
}

static inline void set4(void)
{
    GPIO_write_pin(&driver_config.b, true);
    GPIO_write_pin(&driver_config.c, true);
    GPIO_write_pin(&driver_config.f, true);
    GPIO_write_pin(&driver_config.g, true);
}

static inline void set5(void)
{
    GPIO_write_pin(&driver_config.a, true);
    GPIO_write_pin(&driver_config.c, true);
    GPIO_write_pin(&driver_config.d, true);
    GPIO_write_pin(&driver_config.f, true);
    GPIO_write_pin(&driver_config.g, true);
}

static inline void set6(void)
{
    GPIO_write_pin(&driver_config.a, true);
    GPIO_write_pin(&driver_config.c, true);
    GPIO_write_pin(&driver_config.d, true);
    GPIO_write_pin(&driver_config.e, true);
    GPIO_write_pin(&driver_config.f, true);
    GPIO_write_pin(&driver_config.g, true);
}

static inline void set7(void)
{
    GPIO_write_pin(&driver_config.a, true);
    GPIO_write_pin(&driver_config.b, true);
    GPIO_write_pin(&driver_config.c, true);
}

static inline void set8(void)
{
    GPIO_write_pin(&driver_config.a, true);
    GPIO_write_pin(&driver_config.b, true);
    GPIO_write_pin(&driver_config.c, true);
    GPIO_write_pin(&driver_config.d, true);
    GPIO_write_pin(&driver_config.e, true);
    GPIO_write_pin(&driver_config.f, true);
    GPIO_write_pin(&driver_config.g, true);
}

static inline void set9(void)
{
    GPIO_write_pin(&driver_config.a, true);
    GPIO_write_pin(&driver_config.b, true);
    GPIO_write_pin(&driver_config.c, true);
    GPIO_write_pin(&driver_config.d, true);
    GPIO_write_pin(&driver_config.f, true);
    GPIO_write_pin(&driver_config.g, true);
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

    const GPIO_data_t *pin_config = (const GPIO_data_t *) config;

    for(uint8_t i = 0; i < 8u; i++)
    {
        if(GPIO_config_pin(GPIO_OUTPUT_PUSH_PULL, pin_config) != 0)
        {
            return -1;
        }
        pin_config++;
    }

    driver_config = *config;

    return 0;
}
