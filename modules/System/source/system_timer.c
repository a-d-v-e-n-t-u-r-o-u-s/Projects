#include "system_common.h"
#include "system_timer.h"
#include "stm8l15x.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_gpio.h"

static volatile uint32_t system_tick;
static void (*timer_callback)(void);

@interrupt void SYSTEM_timer_tick(void)
{
    system_tick++;

    if(timer_callback != NULL)
    {
        timer_callback();
    }

    TIM4_ClearFlag(TIM4_FLAG_Update);
}

int8_t SYSTEM_timer_register(void (*callback)(void))
{
    if(callback != NULL)
    {
        timer_callback = callback;
        return 0;
    }
    else
    {
        return -1;
    }
}

uint32_t SYSTEM_timer_get_tick(void)
{
    uint32_t tick=0;
    disableInterrupts();
    tick = system_tick;
    enableInterrupts();
    return tick;
}

uint32_t SYSTEM_timer_tick_difference(uint32_t prev,uint32_t next)
{
    if(next >= prev)
    {
        return next - prev;
    }
    else
    {
        return (U32_MAX - prev) + next + 1;
    }
}

void SYSTEM_timer_delay(uint8_t val)
{
    uint32_t tick = SYSTEM_timer_get_tick();
    while(SYSTEM_timer_tick_difference(tick,SYSTEM_timer_get_tick()) <= val)
    {
    }
}

uint8_t SYSTEM_timer_init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE);

    TIM4_TimeBaseInit(TIM4_Prescaler_128,124);
    TIM4_ClearFlag(TIM4_FLAG_Update);
    TIM4_ITConfig(TIM4_IT_Update,ENABLE);
    enableInterrupts();
    TIM4_Cmd(ENABLE);

}
