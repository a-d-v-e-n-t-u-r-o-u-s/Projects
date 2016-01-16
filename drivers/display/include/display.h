#ifndef DISPLAY_H
#define DISPLAY_H

#include "stm8l15x.h"

typedef enum
{
    DISP_HOME,
    DISP_CLEAR
} DISP_command_t;

void DISP_command(DISP_command_t cmd);
void DISP_text(const char *str);
void DISP_configure(uint8_t columns,uint8_t rows);

#endif // DISPLAY_H
