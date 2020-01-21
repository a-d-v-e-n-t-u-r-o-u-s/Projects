/*!
 * \file
 * \brief Seven segment display driver manager implementation file
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
#include "ssd_mgr.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <assert.h>
#include "system.h"
#include "debug.h"

typedef struct
{
    SSD_MGR_config_t config;
    uint16_t value;
} SSD_MGR_module_t;

static SSD_MGR_module_t ssd_mgr;

/*! \todo make it really in FLASH memory */
static const bool digits_data[10][7] =
{
    [0] = {  true, true, true, true, true, true, false},
    [1] = {  false, true, true, false, false, false, false},
    [2] = {  true, true, false, true, true, false, true},
    [3] = {  true, true, true, true, false, false, true},
    [4] = {  false, true, true, false, false, true, true},
    [5] = {  true, false, true, true, false, true, true},
    [6] = {  true, false, true, true, true, true, true},
    [7] = {  true, true, true, false, false, false, false},
    [8] = {  true, true, true, true, true, true, true},
    [9] = {  true, true, true, true, false, true, true},
};

static inline void clear(const uint8_t *gpio, uint8_t size,
        bool is_inverted_logic)
{
    bool val = is_inverted_logic ? true : false;

    for(uint8_t i = 0u; i < size;  i++)
    {
        GPIO_write_pin(gpio[i], val);
    }
}

static void light_digit(const uint8_t *gpio, const bool *data, uint8_t size,
        bool is_inverted_logic)
{
    //clear(gpio, size, is_inverted_logic);

    for(uint8_t i = 0u; i < size; i++)
    {
        bool val = is_inverted_logic ? !data[i]: data[i];
        GPIO_write_pin(gpio[i], val);
    }
}

static inline uint8_t get_digit(uint16_t value, uint8_t position)
{
     switch(position)
     {
         case 0:
             return (value/1000u)%10u;
         case 1:
             return (value/100u)%10u;
         case 2:
             return (value/10u)%10u;
         case 3:
             return value%10u;
     }

     return 0U;
}

/*! \todo fix this mode after receiving logic analyzer probes */
/*
 *static void multiplex_in_segment_mode(uint16_t value)
 *{
 *    static uint8_t display_no;
 *    static uint8_t old_display_no = UINT8_MAX;
 *    static uint8_t segment_no;
 *
 *    static uint8_t digit;
 *
 *    if(display_no != old_display_no)
 *    {
 *        digit = get_digit(value, display_no);
 *        old_display_no = display_no;
 *    }
 *
 *    GPIO_write_pin(module_config[display_no], true);
 *
 *    const uint8_t real_seg = segment_no/2;
 *
 *    if((segment_no % 2u) == 0)
 *    {
 *        SSD_set_segment(real_seg, !data[digit][real_seg]);
 *    }
 *    else
 *    {
 *        SSD_set_segment(real_seg, true);
 *    }
 *
 *    segment_no++;
 *    segment_no %= 14u;
 *
 *    if(segment_no == 0u)
 *    {
 *        GPIO_write_pin(module_config[display_no], false);
 *        display_no++;
 *        display_no %= 4u;
 *    }
 *}
 */
/*! \todo find out later what is more efficient */
#if 0
static inline void clear2(void)
{
    GPIO_write_pin(ssd_mgr.config.seg_config[0], ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[1], ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[2], ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[3], ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[4], ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[5], ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[6], ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[7], ssd_mgr.config.is_segment_inverted_logic);
}

static inline void set0(void)
{
    GPIO_write_pin(ssd_mgr.config.seg_config[0], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[1], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[2], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[3], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[4], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[5], !ssd_mgr.config.is_segment_inverted_logic);
}

static inline void set1(void)
{
    GPIO_write_pin(ssd_mgr.config.seg_config[1], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[2], !ssd_mgr.config.is_segment_inverted_logic);
}

static inline void set2(void)
{
    GPIO_write_pin(ssd_mgr.config.seg_config[0], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[1], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[3], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[4], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[6], !ssd_mgr.config.is_segment_inverted_logic);
}

static inline void set3(void)
{
    GPIO_write_pin(ssd_mgr.config.seg_config[0], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[1], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[2], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[3], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[6], !ssd_mgr.config.is_segment_inverted_logic);
}

static inline void set4(void)
{
    GPIO_write_pin(ssd_mgr.config.seg_config[1], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[2], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[5], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[6], !ssd_mgr.config.is_segment_inverted_logic);
}

static inline void set5(void)
{
    GPIO_write_pin(ssd_mgr.config.seg_config[0], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[2], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[3], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[5], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[6], !ssd_mgr.config.is_segment_inverted_logic);
}

static inline void set6(void)
{
    GPIO_write_pin(ssd_mgr.config.seg_config[0], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[2], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[3], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[4], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[5], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[6], !ssd_mgr.config.is_segment_inverted_logic);
}

static inline void set7(void)
{
    GPIO_write_pin(ssd_mgr.config.seg_config[0], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[1], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[2], !ssd_mgr.config.is_segment_inverted_logic);
}

static inline void set8(void)
{
    GPIO_write_pin(ssd_mgr.config.seg_config[0], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[1], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[2], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[3], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[4], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[5], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[6], !ssd_mgr.config.is_segment_inverted_logic);
}

static inline void set9(void)
{
    GPIO_write_pin(ssd_mgr.config.seg_config[0], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[1], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[2], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[3], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[5], !ssd_mgr.config.is_segment_inverted_logic);
    GPIO_write_pin(ssd_mgr.config.seg_config[6], !ssd_mgr.config.is_segment_inverted_logic);
}

static void light(uint8_t value)
{
    clear2();

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
#endif

static void multiplex_in_digit_mode(uint16_t value)
{
    static uint8_t display_no;
    const uint8_t *disp_config = ssd_mgr.config.disp_config;
    const uint8_t disp_config_size = sizeof(ssd_mgr.config.disp_config);
    const bool is_disp_inverted_logic = ssd_mgr.config.is_disp_inverted_logic;

    clear(disp_config, disp_config_size, is_disp_inverted_logic);

    uint8_t digit = get_digit(value, display_no);

    light_digit(ssd_mgr.config.seg_config, &digits_data[digit][0], 7U,
            ssd_mgr.config.is_segment_inverted_logic);
    //light(digit);

    bool val = is_disp_inverted_logic ? false : true;
    GPIO_write_pin(disp_config[display_no], val);

    display_no++;
    display_no %= 4u;
}

static void ssd_mgr_main(void)
{
    uint16_t value = ssd_mgr.value;

    if(ssd_mgr.config.is_segment_mode)
    {
        //multiplex_in_segment_mode(value);
    }
    else
    {
        multiplex_in_digit_mode(value);
    }
}

int8_t SSD_MGR_set(uint16_t value)
{
    if( value > 9999U)
    {
        return -1;
    }

    ssd_mgr.value = value;

    return 0;
}

int8_t SSD_MGR_initialize(const SSD_MGR_config_t *config)
{
    if(config == NULL)
    {
        return -1;
    }

    for(uint8_t i = 0; i < 8u; i++)
    {
        GPIO_config_pin(config->seg_config[i]);
    }

    for(uint8_t i = 0; i < 4u; i++)
    {
        GPIO_config_pin(config->disp_config[i]);
    }

    clear(config->disp_config, sizeof(config->disp_config),
                config->is_disp_inverted_logic);

    uint8_t interval = config->is_segment_mode ? 0u : 5u;

    if(SYSTEM_register_task(ssd_mgr_main, interval) != 0)
    {
        return -1;
    }

    ssd_mgr.config = *config;

    return 0;
}
