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
#include "hardware.h"

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

typedef struct
{
    GPIO_TypeDef *port;
    GPIO_Pin_TypeDef pin;
} disp_hardware_t;

static disp_driver_t driver;
static disp_hardware_t hardware[4] =
{
    {DISP_D4_PORT,DISP_D4_PIN},
    {DISP_D5_PORT,DISP_D5_PIN},
    {DISP_D6_PORT,DISP_D6_PIN},
    {DISP_D7_PORT,DISP_D7_PIN}
};

static uint8_t disp_read_nibble(void)
{
    uint8_t ret = 0;
    uint8_t i = 0;

    GPIO_SetBits(DISP_E_PORT,DISP_E_PIN);

    for(i=0;i<ARRAY_SIZE(hardware);i++)
    {
        if(GPIO_ReadInputDataBit(hardware[i].port,hardware[i].pin))
        {
            ret |= (uint8_t) (1<<i);
        }
    }

    GPIO_ResetBits(DISP_E_PORT,DISP_E_PIN);

    return ret;
}

static void disp_send_nibble(uint8_t cmd)
{
    uint8_t i = 0;

    GPIO_SetBits(DISP_E_PORT,DISP_E_PIN);

    GPIO_ResetBits(DISP_D4_PORT,DISP_D4_PIN);
    GPIO_ResetBits(DISP_D5_PORT,DISP_D5_PIN);
    GPIO_ResetBits(DISP_D6_PORT,DISP_D6_PIN);
    GPIO_ResetBits(DISP_D7_PORT,DISP_D7_PIN);

    for(i=0;i<ARRAY_SIZE(hardware);i++)
    {
        if(cmd & (1 << i))
        {
            GPIO_SetBits(hardware[i].port,hardware[i].pin);
        }
    }

    GPIO_ResetBits(DISP_E_PORT,DISP_E_PIN);
}

static uint8_t disp_read(void)
{
    uint8_t ret=0;

    GPIO_Init(DISP_D4_PORT,DISP_D4_PIN,GPIO_Mode_In_PU_No_IT);
    GPIO_Init(DISP_D5_PORT,DISP_D5_PIN,GPIO_Mode_In_PU_No_IT);
    GPIO_Init(DISP_D6_PORT,DISP_D6_PIN,GPIO_Mode_In_PU_No_IT);
    GPIO_Init(DISP_D7_PORT,DISP_D7_PIN,GPIO_Mode_In_PU_No_IT);

    GPIO_SetBits(DISP_RW_PORT,DISP_RW_PIN);
    ret |= (uint8_t)(disp_read_nibble() << 4);
    ret |= disp_read_nibble();
    return ret;
}

static uint8_t disp_read_cmd(void)
{
    GPIO_ResetBits(DISP_RS_PORT,DISP_RS_PIN);
    return disp_read();
}

static void disp_write(uint8_t cmd)
{
    GPIO_Init(DISP_D4_PORT,DISP_D4_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(DISP_D5_PORT,DISP_D5_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(DISP_D6_PORT,DISP_D6_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(DISP_D7_PORT,DISP_D7_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_ResetBits(DISP_RW_PORT,DISP_RW_PIN);
    disp_send_nibble((uint8_t)(cmd>>4));
    disp_send_nibble(cmd);
    while(disp_read_cmd() & 0x80);
}

static void disp_send_cmd(uint8_t cmd)
{
    GPIO_ResetBits(DISP_RS_PORT,DISP_RS_PIN);
    disp_write(cmd);
}

static void disp_send_data(uint8_t data)
{
    GPIO_SetBits(DISP_RS_PORT,DISP_RS_PIN);
    disp_write(data);
}

static INLINE_SPECIFIER void disp_display_control_cmd(uint8_t cmd)
{
    disp_send_cmd((uint8_t)(DISPLAY_ONOFF|cmd));
}

static INLINE_SPECIFIER void disp_display_function_cmd(uint8_t cmd)
{
    disp_send_cmd((uint8_t)(FUNCTION_SET|cmd));
}

static INLINE_SPECIFIER void disp_display_mode_cmd(uint8_t cmd)
{
    disp_send_cmd((uint8_t)(ENTRY_MODE|cmd));
}

static INLINE_SPECIFIER void disp_cursor_shift_cmd(uint8_t cmd)
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
    driver.config.function = config->function;
    driver.config.control = config->control;
    driver.config.mode = config->mode;
    driver.config.cols = config->cols;
    driver.config.rows = config->rows;

    GPIO_Init(DISP_RS_PORT,DISP_RS_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(DISP_RW_PORT,DISP_RW_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(DISP_E_PORT,DISP_E_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(DISP_D4_PORT,DISP_D4_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(DISP_D5_PORT,DISP_D5_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(DISP_D6_PORT,DISP_D6_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(DISP_D7_PORT,DISP_D7_PIN,GPIO_Mode_Out_PP_High_Fast);

    SYSTEM_timer_delay(40);
    GPIO_ResetBits(DISP_RS_PORT,DISP_RS_PIN);
    GPIO_ResetBits(DISP_RW_PORT,DISP_RW_PIN);
    GPIO_ResetBits(DISP_E_PORT,DISP_E_PIN);

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
