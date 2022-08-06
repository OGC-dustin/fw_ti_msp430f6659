#ifndef HAL_MSP430F6659_H
#define HAL_MSP430F6659_H

#include <stdint.h>

#include "hw_description.h"

void hal_init( void );

#ifdef HW_SYS_CLOCK_XT1 /* RTC 32.768 kHz */
void hal_clock_xt1__init( void );
#endif

#ifdef HW_SYS_CLOCK_XT2 /* 4 MHz */
void hal_clock_xt2__init( void );
#endif

#define HAL_SYS_TICKS_PER_SECOND 1024
uint64_t hal_get_sys_tick( void );

void hal_led_heartbeat( int8_t state ); /* for temporary heartbeat application */

#endif /* HAL_MSP430F6659_H */
