#include "system.h"
#include "stm8l15x_clk.h"

uint8_t SYSTEM_init(void)
{
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    CLK_PeripheralClockConfig(CLK_Peripheral_BOOTROM,DISABLE);
}
