#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#include "system.h"
#include "system_timer.h"
#include "display.h"
#include "debug.h"
#include <stdio.h>

#define LED_GREEN_PORT              GPIOA
#define LED_GREEN_PIN               GPIO_Pin_7

INTERRUPT_HANDLER(SYSTEM_clock_switch, 17);
INTERRUPT_HANDLER(DEBUG_usart2_tx, 19);
INTERRUPT_HANDLER(SYSTEM_timer_tick, 25);

static INLINE_SPECIFIER void drivers_init(void)
{
    DEBUG_config_t config =
    {
        .unit = USART2,
        .clk = CLK_Peripheral_USART2,
        .pullup_port = GPIOE,
        .pullup_pin = 4U,
        .baudrate = 115200U,
        .word_length = USART_WordLength_8b,
        .stop_bits = USART_StopBits_1,
        .parity = USART_Parity_No,
        .mode = USART_Mode_Tx,
    };

    DEBUG_configure(&config);
}

static INLINE_SPECIFIER void modules_init(void)
{
}

static void callback(void)
{
}

int main(void)
{
    GPIO_Init(LED_GREEN_PORT,LED_GREEN_PIN,GPIO_Mode_Out_PP_High_Fast);

    SYSTEM_init();
    SYSTEM_timer_register(callback);

    drivers_init();
    modules_init();

    for(uint8_t i = 0; i < 10; i++)
    {
        SYSTEM_timer_delay(1);
        GPIO_ToggleBits(LED_GREEN_PORT,LED_GREEN_PIN);
    }

    SYSTEM_timer_delay(100);
    GPIO_ToggleBits(LED_GREEN_PORT,LED_GREEN_PIN);

    printf("********************************\n");
    printf("******* Remote Control *********\n");
    printf("********************************\n");

    while(1)
    {
        SYSTEM_main();

        GPIO_ToggleBits(LED_GREEN_PORT,LED_GREEN_PIN);
        SYSTEM_timer_delay(100);
    }
}
