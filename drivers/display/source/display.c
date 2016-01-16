#include "display.h"
#include "system_timer.h"
#include "stm8l15x_gpio.h"
#include "common.h"

#define DISP_POWER_UP_TIME      40
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

#define FUNCTION_SET            0x20
#define MODE_4_BIT              0x00
#define FONT5x7                 0x00
#define FONT5x10                0x04
#define ONE_LINE                0x00
#define TWO_LINE                0x08

#define DISPLAY_ONOFF           0x08
#define DISPLAY_OFF             0x00
#define DISPLAY_ON              0x04
#define CURSOR_OFF              0x00
#define CURSOR_ON               0x02
#define CURSOR_NOBLINK          0x00
#define CURSOR_BLINK            0x01

#define CLEAR                   0x01
#define SETCGRAMADDR            0x40
#define SETDDRAMADDR            0x80

#define DISPLAY_CURSOR_SHIFT    0x10
#define SHIFT_CURSOR            0x00
#define SHIFT_DISPLAY           0x08
#define SHIFT_LEFT              0x00
#define SHIFT_RIGHT             0x04

#define ENTRY_MODE              0x04
#define EM_SHIFT_CURSOR         0x00
#define EM_SHIFT_DISPLAY        0x01
#define EM_DECREMENT            0x00
#define EM_INCREMENT            0x02

typedef enum disp_state
{
    DISP_INIT,
    DISP_POWER_UP,
    DISP_INIT_CMDS1,
    DISP_INIT_CMDS2,
    DISP_INIT_CMDS3,
    DISP_END
} disp_state_t;

typedef struct
{
    uint8_t columns;
    uint8_t rows;
    uint8_t current_x;
    uint8_t current_y;
} disp_driver_t;

static disp_driver_t driver;

static disp_state_t state;

static uint8_t disp_read_nibble(void)
{
    uint8_t ret = 0;

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

    return ret;
}

static void disp_send_nibble(uint8_t cmd)
{
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
}

static uint8_t disp_read(void)
{
    uint8_t ret=0;
    GPIO_Init(DISP_PORT,DISP_DATA_PINS,GPIO_Mode_In_PU_No_IT);
    GPIO_SetBits(DISP_PORT,DISP_RW);
    GPIO_SetBits(DISP_PORT,DISP_E);
    ret |= (uint8_t)(disp_read_nibble() << 4);
    GPIO_ResetBits(DISP_PORT,DISP_E);
    GPIO_SetBits(DISP_PORT,DISP_E);
    ret |= (disp_read_nibble());
    GPIO_ResetBits(DISP_PORT,DISP_E);
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
    GPIO_SetBits(DISP_PORT,DISP_E);
    disp_send_nibble((uint8_t)(cmd>>4));
    GPIO_ResetBits(DISP_PORT,DISP_E);
    GPIO_SetBits(DISP_PORT,DISP_E);
    disp_send_nibble(cmd);
    GPIO_ResetBits(DISP_PORT,DISP_E);
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

static void disp_cursor_set(uint8_t column,uint8_t row)
{
    uint8_t row_offset[] = {0x00,0x40,0x14,0x54};
    uint8_t cmd = SETDDRAMADDR;

    if(column >= driver.columns)
    {
        column = 0;
    }

    if(row >= driver.rows ||
            row >= ARRAY_SIZE(row_offset))
    {
        row = 0;
    }

    driver.current_x = column;
    driver.current_y = row;

    cmd |= (uint8_t)(column + row_offset[row]);

    disp_send_cmd(cmd);
}


/*! \TODO 1ms delay might be needed here */
void DISP_command(DISP_command_t cmd)
{
    uint8_t tmp = 0;

    switch(cmd)
    {
        case DISP_HOME:
            tmp = 0x02;
            break;
        case DISP_CLEAR:
            tmp = 0x01;
            break;
    }

    disp_send_cmd(tmp);
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

void DISP_send_text(uint8_t x, uint8_t y,const char *str)
{
    disp_cursor_set(x,y);

    while(*str)
    {
        if(driver.current_x >= driver.columns)
        {
            driver.current_x = 0;
            driver.current_y++;
            driver.current_y %= driver.rows;

            disp_cursor_set(x,y);
        }

        if('\n' == *str)
        {
            driver.current_y++;
            driver.current_y %= driver.rows;

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

void DISP_configure(uint8_t columns,uint8_t rows)
{
    uint8_t i = 0;

    driver.columns = columns;
    driver.rows = rows;

    GPIO_Init(DISP_PORT,DISP_PINS_ALL,GPIO_Mode_Out_PP_High_Fast);
    SYSTEM_timer_delay(40);
    GPIO_ResetBits(DISP_PORT,DISP_RS);
    GPIO_ResetBits(DISP_PORT,DISP_RW);
    GPIO_ResetBits(DISP_PORT,DISP_E);

    for(i=0;i<3;i++)
    {
        GPIO_SetBits(DISP_PORT,DISP_E);
        disp_send_nibble(0x03);
        GPIO_ResetBits(DISP_PORT,DISP_E);
        SYSTEM_timer_delay(5);
    }

    GPIO_SetBits(DISP_PORT,DISP_E);
    disp_send_nibble(0x02);
    GPIO_ResetBits(DISP_PORT,DISP_E);

    SYSTEM_timer_delay(1);

    disp_send_cmd(FUNCTION_SET|FONT5x7|TWO_LINE|MODE_4_BIT);
    disp_send_cmd(DISPLAY_ONOFF|DISPLAY_OFF);
    disp_send_cmd(0x01);
    disp_send_cmd(ENTRY_MODE|EM_SHIFT_CURSOR|EM_INCREMENT);
    disp_send_cmd(DISPLAY_ONOFF|DISPLAY_ON|CURSOR_OFF|CURSOR_NOBLINK);

    DISP_command(DISP_CLEAR);
    DISP_command(DISP_HOME);
}
