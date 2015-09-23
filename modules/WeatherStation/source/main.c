#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#include "hd44780.h"
#include "system_kernel.h"
#include "system_timer.h"

#define LED_GREEN_PORT              GPIOE
#define LED_GREEN_PIN               GPIO_Pin_7

static void delay(volatile uint16_t counter)
{
    while(counter--)
    {
    }
}

static void drivers_init(void)
{
}

static void modules_init(void)
{
    SYSTEM_kernel_init();
    SYSTEM_timer_init();
}

int main(void)
{
    GPIO_Init(LED_GREEN_PORT,LED_GREEN_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_ToggleBits(LED_GREEN_PORT,LED_GREEN_PIN);

    drivers_init();
    modules_init();

    while(1)
    {
        GPIO_ToggleBits(LED_GREEN_PORT,LED_GREEN_PIN);
        delay(50000);
    }
}