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
    DISP_configure(16,2);
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

    DISP_text("Hello world !!!");

    while(1)
    {
        SYSTEM_main();
        SYSTEM_timer_delay(100);
        GPIO_ToggleBits(LED_BLUE_PORT,LED_BLUE_PIN);
        GPIO_ToggleBits(LED_GREEN_PORT,LED_GREEN_PIN);
        SYSTEM_timer_delay(100);
    }
}
