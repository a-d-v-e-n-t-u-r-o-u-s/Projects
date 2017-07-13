#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#include "system.h"
#include "system_timer.h"
#include "display.h"

#define LED_GREEN_PORT              GPIOE
#define LED_GREEN_PIN               GPIO_Pin_7
#define LED_BLUE_PORT               GPIOC
#define LED_BLUE_PIN                GPIO_Pin_7

static @inline void drivers_init(void)
{
    DISP_config_t config =
    {
        .function = (uint8_t) (DISP_MODE_4_BIT|DISP_FONTS5x7|DISP_TWO_LINE),
        .control = (uint8_t)( DISPLAY_ON| CURSOR_ON | CURSOR_BLINK),
        .mode = (uint8_t)( EM_SHIFT_DISPLAY | EM_INCREMENT),
        .cols = 16,
        .rows = 2
    };

    DISP_configure(&config);
}

static @inline void modules_init(void)
{
}

int main(void)
{
    GPIO_Init(LED_BLUE_PORT,LED_BLUE_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(LED_GREEN_PORT,LED_GREEN_PIN,GPIO_Mode_Out_PP_High_Fast);

    SYSTEM_init();

    drivers_init();
    modules_init();

    DISP_send_text(0,0,"Hello world !!!");

    while(1)
    {
        SYSTEM_main();
        SYSTEM_timer_delay(100);
        GPIO_ToggleBits(LED_BLUE_PORT,LED_BLUE_PIN);
        GPIO_ToggleBits(LED_GREEN_PORT,LED_GREEN_PIN);
        SYSTEM_timer_delay(100);
    }
}
