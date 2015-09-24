#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_tim4.h"

#define LED_GREEN_PORT              GPIOE
#define LED_GREEN_PIN               GPIO_Pin_7
#define LED_BLUE_PORT               GPIOC
#define LED_BLUE_PIN                GPIO_Pin_7

static uint32_t system_tick;

static @inline system_clock_init(void)
{
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

    CLK_PeripheralClockConfig(CLK_Peripheral_BOOTROM,DISABLE);
}

static @inline system_tick_init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE);

    TIM4_TimeBaseInit(TIM4_Prescaler_128,124);
    TIM4_ClearFlag(TIM4_FLAG_Update);
    TIM4_ITConfig(TIM4_IT_Update,ENABLE);
    enableInterrupts();
    TIM4_Cmd(ENABLE);
}

int main(void)
{
    GPIO_Init(LED_BLUE_PORT,LED_BLUE_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_ToggleBits(LED_BLUE_PORT,LED_BLUE_PIN);
    GPIO_Init(LED_GREEN_PORT,LED_GREEN_PIN,GPIO_Mode_Out_PP_High_Fast);
    GPIO_ToggleBits(LED_GREEN_PORT,LED_GREEN_PIN);

    system_clock_init();
    system_tick_init();


    while(1)
    {
        GPIO_ToggleBits(LED_GREEN_PORT,LED_GREEN_PIN);
    }
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
