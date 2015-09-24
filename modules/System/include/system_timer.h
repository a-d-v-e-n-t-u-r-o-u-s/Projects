#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

#include "stm8l15x.h"

uint8_t SYSTEM_timer_register(void);
uint8_t SYSTEM_timer_init(void);
uint32_t SYSTEM_timer_get_tick(void);

#endif // SYSTEM_TIMER_H
