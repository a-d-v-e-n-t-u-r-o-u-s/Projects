#ifndef PCB0000_H
#define PCB0000_H

#define DISP_PORT               GPIOB

#define DISP_D4                 GPIO_Pin_3
#define DISP_D5                 GPIO_Pin_4
#define DISP_D6                 GPIO_Pin_5
#define DISP_D7                 GPIO_Pin_6

#define DISP_RS                 GPIO_Pin_0
#define DISP_RW                 GPIO_Pin_1
#define DISP_E                  GPIO_Pin_2

#define DISP_DATA_PINS          DISP_D4|DISP_D5|DISP_D6|DISP_D7
#define DISP_CTRL_PINS          DISP_RS|DISP_RW|DISP_E
#define DISP_PINS_ALL           DISP_CTRL_PINS|DISP_DATA_PINS

#endif /* end of PCB0000_H */
