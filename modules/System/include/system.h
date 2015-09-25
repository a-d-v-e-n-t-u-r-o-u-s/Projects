#ifndef SYSTEM_H
#define SYSTEM_H

#include "stm8l15x.h"

typedef void (*SYSTEM_task_t)(void);

int8_t SYSTEM_register_task(SYSTEM_task_t task);
void SYSTEM_main(void);
void SYSTEM_init(void);

#endif // SYSTEM_KERNEL_H
