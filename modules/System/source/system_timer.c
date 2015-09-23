#include "system_timer.h"
#include "stm8l15x.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_gpio.h"

static uint32_t system_tick;

#define LED_BLUE_PORT               GPIOC
#define LED_BLUE_PIN                GPIO_Pin_7

uint8_t SYSTEM_timer_register(void)
{
}

uint8_t SYSTEM_timer_init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE);

    TIM4_TimeBaseInit(TIM4_Prescaler_128,124);
    TIM4_ClearFlag(TIM4_FLAG_Update);
    TIM4_ITConfig(TIM4_IT_Update,ENABLE);
    enableInterrupts();
    TIM4_Cmd(ENABLE);

    GPIO_Init(LED_BLUE_PORT,LED_BLUE_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_ToggleBits(LED_BLUE_PORT,LED_BLUE_PIN);
}

@interrupt void SYSTEM_timer_tick(void)
{
    system_tick++;

    TIM4_ClearFlag(TIM4_FLAG_Update);

    if((system_tick %1000) ==0)
    {
        GPIO_ToggleBits(LED_BLUE_PORT,LED_BLUE_PIN);
    }
}
