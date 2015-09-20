#ifndef __STM8L15x_CONF_H
#define __STM8L15x_CONF_H

#ifdef USE_FULL_ASSERT
/*
 *#define assert_param(expr)  ((expr) ? (void)0: assert_failed((uint8_t*)__FILE__,__LINE__))
 *void assert_failed(uint8_t* file,uint32_t line);
 */
#define assert_param(expr) ((void)0)
#else
#define assert_param(expr) ((void)0)
#endif
#endif
