#ifndef DISPLAY_H
#define DISPLAY_H

typedef enum
{
    DISP_HOME,
    DISP_CLEAR
} DISP_command_t;

void DISP_command(DISP_command_t cmd);
void DISP_text(const char *str);
void DISP_configure(void);

#endif // DISPLAY_H
