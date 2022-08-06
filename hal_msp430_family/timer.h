/*
 * Copyright (C) 2020 Banyan Water
 * timer.h
 * HAL for manipulating the MSP430's Timers
 */

#ifndef MSP430_HAL_TIMER_H_
#define MSP430_HAL_TIMER_H_

#include <stdint.h>

/******************************************************************************
 * hal_timer__setup_tick_timer
 * Configues the tick timer
 * uint16_t clock_cycles_in_tick - the number of clock cycles in each tick
 *****************************************************************************/
void hal_timer__setup_tick_timer(uint16_t clock_cycles_in_tick);

/******************************************************************************
 * hal_timer__setup_millisecond_timer
 * Configues the millisecond timer
 * uint16_t cycles_per_millisecond - the number of clock cycles in each
 *      millisecond
 *****************************************************************************/
void hal_timer__setup_millisecond_timer(uint16_t cycles_per_millisecond);

/******************************************************************************
 * hal_timer__get_millisecond
 * Gets the current millisecond value
 * return uint32_t - the current millisecond value
 *****************************************************************************/
uint32_t hal_timer__get_millisecond();

#endif /* MSP430_HAL_TIMER_H_ */
