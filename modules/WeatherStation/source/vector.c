/*  INTERRUPT VECTORS TABLE FOR STM8
 *  Copyright (c) 2007 by COSMIC Software
 */
/*  should be the name of a dummy interrupt routine
 *  in order to catch properly unexpected interrupts
 */
#define NULL 0

#include "stm8l15x.h"

extern void _stext();       /* startup routine */

extern void SYSTEM_timer_tick(void);
extern void RTC_timer_tick(void);
extern void SYSTEM_clock_switch(void);

#pragma section const {vector}

void (* const @vector _vectab[32])() =
{
    _stext,             /* RESET       */
    NULL,               /* TRAP        */
    NULL,               /* TLI         */
    NULL,               /* AWU         */
    NULL,               /* CLK         */
    NULL,               /* EXTI PORTA  */
    RTC_timer_tick,     /* EXTI PORTB  */
    NULL,               /* EXTI PORTC  */
    NULL,               /* EXTI PORTD  */
    NULL,               /* EXTI PORTE  */
    NULL,               /* CAN RX      */
    NULL,               /* CAN TX      */
    NULL,               /* SPI         */
    NULL,               /* TIMER 1 OVF */
    NULL,               /* TIMER 1 CAP */
    NULL,               /* TIMER 2 OVF */
    NULL,               /* TIMER 2 CAP */
    NULL,               /* TIMER 3 OVF */
    NULL,               /* TIMER 3 CAP */
    SYSTEM_clock_switch,/* USART TX    */
    NULL,               /* USART RX    */
    NULL,               /* I2C         */
    NULL,               /* LINUART TX  */
    NULL,               /* LINUART RX  */
    NULL,               /* ADC         */
    NULL,               /* TIMER 4 OVF */
    NULL,               /* EEPROM ECC  */
    SYSTEM_timer_tick,  /* Reserved    */
    NULL,               /* Reserved    */
    NULL,               /* Reserved    */
    NULL,               /* Reserved    */
    NULL,               /* Reserved    */
};
