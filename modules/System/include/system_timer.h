#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

#include "stm8l15x.h"

int8_t SYSTEM_timer_register(void (timer_callback)(void));
uint32_t SYSTEM_timer_get_tick(void);
uint32_t SYSTEM_timer_tick_difference(uint32_t prev,uint32_t next);
uint8_t SYSTEM_timer_init(void);

#endif // SYSTEM_TIMER_H
