/*!
 * \file
 * \brief Windcom Tech WC1602A display driver
 * \author Dawid Babula
 * \email dbabula@adventurous.pl
 *
 * \par Copyright (C) Dawid Babula, 2016
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
#ifndef DISPLAY_H
#define DISPLAY_H

#include "stm8l15x.h"

typedef enum
{
    DISP_MODE_4_BIT = 0x00,
    DISP_FONTS5x7 = 0x00,
    DISP_ONE_LINE = 0x00,
    DISP_MODE_8_BIT = 0x01,
    DISP_FONTS5x10 = 0x04,
    DISP_TWO_LINE = 0x08
} DISP_function_t;

typedef enum
{
    DISPLAY_OFF = 0x00,
    CURSOR_OFF = 0x00,
    CURSOR_NOBLINK = 0x00,
    CURSOR_BLINK = 0x01,
    CURSOR_ON = 0x02,
    DISPLAY_ON = 0x04
} DISP_control_t;

typedef enum
{
    EM_SHIFT_CURSOR = 0x00,
    EM_DECREMENT = 0x00,
    EM_SHIFT_DISPLAY = 0x01,
    EM_INCREMENT = 0x02
} DISP_mode_t;

typedef struct
{
    uint8_t function;
    uint8_t control;
    uint8_t mode;
    uint8_t cols;
    uint8_t rows;
} DISP_config_t;

void DISP_clear(void);
void DISP_home(void);
void DISP_display_on(void);
void DISP_display_off(void);
void DISP_blink_on(void);
void DISP_blink_off(void);
void DISP_cursor_on(void);
void DISP_cursor_off(void);
void DISP_scroll_left(void);
void DISP_scroll_right(void);
void DISP_create_custom_char(uint8_t location,const uint8_t *data);
void DISP_send_custom_char(uint8_t x, uint8_t y, uint8_t location);
void DISP_send_text(uint8_t x, uint8_t y,const char *str);
void DISP_configure(const DISP_config_t *config);

#endif // DISPLAY_H
