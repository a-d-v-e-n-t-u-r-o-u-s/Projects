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

/*!
 *
 * \defgroup drivers
 * \addtogroup display
 * \ingroup drivers
 * \brief Driver for Windcom Tech WC1602A-STBLWNC06 display
 *
 * LCD format: 16 characters x 2 lines
 * LCD controller: KS0066+KS0065 (aka Samsung S6A0069)
 * LCD colour: Blue/Gray/Yellow-Green
 *
 * Pin assignment:
 * 1        VSS     GND
 * 2        VDD     +5V
 * 3        V0      Contrast adjust (GND for full contrast)
 * 4        RS      Register select signal
 * 5        R/W     Data read/write
 * 6        E       Enable signal
 * 7-14     DB0-DB7 Data bus
 * 15       A       LED B/L(+) +5V
 * 16       K       LED B/L(-) GND
 *
 */

/*@{*/

#include "stm8l15x.h"

/*!
 * \brief Type represents diplay functional configuration
 *
 * Functional configuration must be done as combination
 * of those enums e.g.
 *
 * \code (uint8_t)( DISP_MODE_4_BIT| DISP_FONTS5x7 | DISP_TWO_LINE) \endcode
 * or
 * \code (uint8_t)( DISP_MODE_8_BIT| DISP_FONTS5x7 | DISP_ONE_LINE) \endcode
 *
 * \note Some settings are mutually exclusive, that's why setting following
 * functional configuration is just wrong:
 * \code (uint8_t)( DISP_MODE_8_BIT| DISP_MODE_4_BIT | DISP_ONE_LINE) \endcode
 */
typedef enum
{
    DISP_MODE_4_BIT = 0x00, /*!< turn on 4-bit display mode uses DB4-DB7 only */
    DISP_FONTS5x7 = 0x00, /*!< turn on fonts with 5x7 pixels size */
    DISP_ONE_LINE = 0x00, /*!< turn on one line display support */
    DISP_MODE_8_BIT = 0x01, /*!< turn on  8-bit display mode uses DB0-DB7 */
    DISP_FONTS5x10 = 0x04, /*!< turn on fonts with 5x10 pixels size */
    DISP_TWO_LINE = 0x08 /*!< turn on two lines display support */
} DISP_function_t;

/*!
 * \brief Type represents diplay control configuration
 *
 * Control configuration must be done as combination
 * of those enums e.g.
 * \code (uint8_t)( DISPLAY_ON| CURSOR_OFF | CURSOR_NOBLINK) \endcode
 * or
 * \code (uint8_t)( DISPLAY_ON| CURSOR_ON | CURSOR_BLINK) \endcode
 *
 * \note Some settings are mutually exclusive, that's why setting following
 * functional configuration is just wrong:
 * \code (uint8_t)( DISPLAY_ON| DISPLAY_OFF| CURSOR_BLINK) \endcode
 */
typedef enum
{
    DISPLAY_OFF = 0x00, /*!< turn off display */
    CURSOR_OFF = 0x00, /*! < turn off cursor */
    CURSOR_NOBLINK = 0x00, /*! < turn off cursor blinking */
    CURSOR_BLINK = 0x01, /*!< turn on cursor blinking */
    CURSOR_ON = 0x02, /*!< turn on cursor */
    DISPLAY_ON = 0x04 /*!< turn on display */
} DISP_control_t;

/*!
 * \brief Type represents diplay mode configuration
 *
 * Control configuration must be done as combination
 * of those enums e.g.
 * \code (uint8_t)( EM_SHIFT_DISPLAY | EM_INCREMENT) \endcode
 *
 * \note Some settings are mutually exclusive, that's why setting following
 * functional configuration is just wrong:
 * \code (uint8_t)( EM_SHIFT_CURSOR| EM_SHIFT_DISPLAY) \endcode
 */
typedef enum
{
    EM_SHIFT_CURSOR = 0x00,
    EM_DECREMENT = 0x00,
    EM_SHIFT_DISPLAY = 0x01,
    EM_INCREMENT = 0x02
} DISP_mode_t;

/*!
 * \brief Type represents configuration structure
 */
typedef struct
{
    uint8_t function; /*!< display functional configuration \see DISP_function_t */
    uint8_t control; /*!< display control configuration \see DISP_control_t */
    uint8_t mode; /*!< display mode configuration \see DISP_mode_t */
    uint8_t cols; /*!< amount of display columns */
    uint8_t rows; /*!< amount of display rows */
} DISP_config_t;

/*!
 * \brief Clears display screen
 */
void DISP_clear(void);

/*!
 * \brief Takes cursor to the position 0,0
 */
void DISP_home(void);

/*!
 * \brief Turns on display
 */
void DISP_display_on(void);

/*!
 * \brief Turns off display
 */
void DISP_display_off(void);

/*!
 * \brief Turns on cursor blinking
 */
void DISP_blink_on(void);

/*!
 * \brief Turns off cursor blinking
 */
void DISP_blink_off(void);

/*!
 * \brief Turns on cursor
 */
void DISP_cursor_on(void);

/*!
 * \brief Turns off cursor
 */
void DISP_cursor_off(void);

/*!
 * \brief Scrolls whole display on the left
 */
void DISP_scroll_left(void);

/*!
 * \brief Scrolls whole display on the right
 */
void DISP_scroll_right(void);

/*!
 * \brief Create custom character in CGRAM
 *
 * \param location slot at which character will be stored valid values 0-7
 * \param data pointer to 8-byte array which holds character data
 */
void DISP_create_custom_char(uint8_t location,const uint8_t *data);

/*!
 * \brief Displays custom character on screen
 *
 * \param x column value
 * \param y row value
 * \param location slot from which character will be taken valid values 0-7
 */
void DISP_send_custom_char(uint8_t x, uint8_t y, uint8_t location);

/*!
 * \brief Displays string on screen
 *
 * \param x column value
 * \param y row value
 * \param str string to be displayed
 */
void DISP_send_text(uint8_t x, uint8_t y,const char *str);

/*!
 * \brief Configuration function for display driver
 *
 * \param config pointer to configuration structure
 */
void DISP_configure(const DISP_config_t *config);

/*@}*/
#endif // DISPLAY_H
