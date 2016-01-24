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
#include "display.h"
#include "system_timer.h"
#include "stm8l15x_gpio.h"
#include "common.h"

#define DISP_PORT               GPIOB

#define DISP_D4                 GPIO_Pin_3
#define DISP_D5                 GPIO_Pin_4
#define DISP_D6                 GPIO_Pin_5
#define DISP_D7                 GPIO_Pin_6

#define DISP_RS                 GPIO_Pin_0
#define DISP_RW                 GPIO_Pin_1
#define DISP_E                  GPIO_Pin_2

#define DISP_DATA_PINS          DISP_D4|DISP_D5|DISP_D6|DISP_D7
#define DISP_CTRL_PINS          DISP_RS|DISP_RW|DISP_E
#define DISP_PINS_ALL           DISP_CTRL_PINS|DISP_DATA_PINS

#define CLEAR                   0x01
#define HOME                    0x02
#define ENTRY_MODE              0x04
#define DISPLAY_ONOFF           0x08
#define CURSOR_SHIFT            0x10
#define FUNCTION_SET            0x20
#define SETCGRAMADDR            0x40
#define SETDDRAMADDR            0x80

#define CURSOR_MOVE             0x00
#define DISPLAY_MOVE            0x08
#define MOVE_LEFT               0x00
#define MOVE_RIGHT              0x04


typedef struct
{
    DISP_config_t config;
    uint8_t current_x;
    uint8_t current_y;
} disp_driver_t;

static disp_driver_t driver;

static uint8_t disp_read_nibble(void)
{
    uint8_t ret = 0;

    GPIO_SetBits(DISP_PORT,DISP_E);

    if(GPIO_ReadInputDataBit(DISP_PORT,DISP_D4))
    {
        ret |= (1<<0);
    }

    if(GPIO_ReadInputDataBit(DISP_PORT,DISP_D5))
    {
        ret |= (1<<1);
    }

    if(GPIO_ReadInputDataBit(DISP_PORT,DISP_D6))
    {
        ret |= (1<<2);
    }

    if(GPIO_ReadInputDataBit(DISP_PORT,DISP_D7))
    {
        ret |= (1<<3);
    }

    GPIO_ResetBits(DISP_PORT,DISP_E);

    return ret;
}

static void disp_send_nibble(uint8_t cmd)
{
    GPIO_SetBits(DISP_PORT,DISP_E);

    if(cmd&0x01)
    {
        GPIO_SetBits(DISP_PORT,DISP_D4);
    }
    else
    {
        GPIO_ResetBits(DISP_PORT,DISP_D4);
    }

    if(cmd&0x02)
    {
        GPIO_SetBits(DISP_PORT,DISP_D5);
    }
    else
    {
        GPIO_ResetBits(DISP_PORT,DISP_D5);
    }

    if(cmd&0x04)
    {
        GPIO_SetBits(DISP_PORT,DISP_D6);
    }
    else
    {
        GPIO_ResetBits(DISP_PORT,DISP_D6);
    }

    if(cmd&0x08)
    {
        GPIO_SetBits(DISP_PORT,DISP_D7);
    }
    else
    {
        GPIO_ResetBits(DISP_PORT,DISP_D7);
    }

    GPIO_ResetBits(DISP_PORT,DISP_E);
}

static uint8_t disp_read(void)
{
    uint8_t ret=0;
    GPIO_Init(DISP_PORT,DISP_DATA_PINS,GPIO_Mode_In_PU_No_IT);
    GPIO_SetBits(DISP_PORT,DISP_RW);
    ret |= (uint8_t)(disp_read_nibble() << 4);
    ret |= (disp_read_nibble());
    return ret;
}

static uint8_t disp_read_cmd(void)
{
    GPIO_ResetBits(DISP_PORT,DISP_RS);
    return disp_read();
}

static void disp_write(uint8_t cmd)
{
    GPIO_Init(DISP_PORT,DISP_DATA_PINS,GPIO_Mode_Out_PP_High_Fast);
    GPIO_ResetBits(DISP_PORT,DISP_RW);
    disp_send_nibble((uint8_t)(cmd>>4));
    disp_send_nibble(cmd);
    while(disp_read_cmd() & 0x80);
}

static void disp_send_cmd(uint8_t cmd)
{
    GPIO_ResetBits(DISP_PORT,DISP_RS);
    disp_write(cmd);
}

static void disp_send_data(uint8_t data)
{
    GPIO_SetBits(DISP_PORT,DISP_RS);
    disp_write(data);
}

static @inline void disp_display_control_cmd(uint8_t cmd)
{
    disp_send_cmd((uint8_t)(DISPLAY_ONOFF|cmd));
}

static @inline void disp_display_function_cmd(uint8_t cmd)
{
    disp_send_cmd((uint8_t)(FUNCTION_SET|cmd));
}

static @inline void disp_display_mode_cmd(uint8_t cmd)
{
    disp_send_cmd((uint8_t)(ENTRY_MODE|cmd));
}

static @inline void disp_cursor_shift_cmd(uint8_t cmd)
{
    disp_send_cmd((uint8_t)(CURSOR_SHIFT|DISPLAY_MOVE|cmd));
}


static void disp_cursor_set(uint8_t column,uint8_t row)
{
    uint8_t row_offset[] = {0x00,0x40,0x14,0x54};
    uint8_t cmd = SETDDRAMADDR;

    if(column >= driver.config.cols)
    {
        column = 0;
    }

    if(row >= driver.config.rows ||
            row >= ARRAY_SIZE(row_offset))
    {
        row = 0;
    }

    driver.current_x = column;
    driver.current_y = row;

    cmd |= (uint8_t)(column + row_offset[row]);

    disp_send_cmd(cmd);
}

void DISP_clear(void)
{
    disp_send_cmd(CLEAR);
}

void DISP_home(void)
{
    disp_send_cmd(HOME);
}

void DISP_display_on(void)
{
    driver.config.control |= (uint8_t) DISPLAY_ON;
    disp_display_control_cmd(driver.config.control);
}

void DISP_display_off(void)
{
    driver.config.control &= (uint8_t)(~DISPLAY_ON);
    disp_display_control_cmd(driver.config.control);
}

void DISP_blink_on(void)
{
    driver.config.control |= (uint8_t) CURSOR_BLINK;
    disp_display_control_cmd(driver.config.control);
}

void DISP_blink_off(void)
{
    driver.config.control &= (uint8_t)(~CURSOR_BLINK);
    disp_display_control_cmd(driver.config.control);
}

void DISP_cursor_on(void)
{
    driver.config.control |= (uint8_t) CURSOR_ON;
    disp_display_control_cmd(driver.config.control);
}

void DISP_cursor_off(void)
{
    driver.config.control &= (uint8_t)(~CURSOR_ON);
    disp_display_control_cmd(driver.config.control);
}

void DISP_scroll_left(void)
{
    disp_cursor_shift_cmd(MOVE_LEFT);
}

void DISP_scroll_right(void)
{
    disp_cursor_shift_cmd(MOVE_RIGHT);
}

void DISP_create_custom_char(uint8_t location,const uint8_t *data)
{
    uint8_t i = 0;
    uint8_t cmd = SETCGRAMADDR;
    location &= 0x07;
    cmd |= (uint8_t)(location << 3);

    disp_send_cmd(cmd);

    for(i=0;i<8;i++)
    {
        disp_send_data(data[i]);
    }

}

void DISP_send_custom_char(uint8_t x, uint8_t y, uint8_t location)
{
    disp_cursor_set(x,y);
    disp_send_data(location);
}

void DISP_send_text(uint8_t x, uint8_t y,const char *str)
{
    disp_cursor_set(x,y);

    while(*str)
    {
        if(driver.current_x >= driver.config.cols)
        {
            driver.current_x = 0;
            driver.current_y++;
            driver.current_y %= driver.config.rows;

            disp_cursor_set(x,y);
        }

        if('\n' == *str)
        {
            driver.current_y++;
            driver.current_y %= driver.config.rows;

            disp_cursor_set(x,y);
        }
        else if('\r' == *str)
        {
            disp_cursor_set(0,driver.current_y);
        }
        else
        {
            disp_send_data(*str++);
            driver.current_x++;
        }
    }
}

void DISP_configure(const DISP_config_t *config)
{
    uint8_t i = 0;

    /*! TODO add validation of config */
    driver.config = *config;

    GPIO_Init(DISP_PORT,DISP_PINS_ALL,GPIO_Mode_Out_PP_High_Fast);
    SYSTEM_timer_delay(40);
    GPIO_ResetBits(DISP_PORT,DISP_RS);
    GPIO_ResetBits(DISP_PORT,DISP_RW);
    GPIO_ResetBits(DISP_PORT,DISP_E);

    for(i=0;i<3;i++)
    {
        disp_send_nibble(0x03);
        SYSTEM_timer_delay(5);
    }

    disp_send_nibble(0x02);

    SYSTEM_timer_delay(1);

    disp_display_function_cmd(driver.config.function);
    disp_display_mode_cmd(driver.config.mode);
    disp_display_control_cmd(driver.config.control);

    DISP_clear();
    DISP_home();
}
