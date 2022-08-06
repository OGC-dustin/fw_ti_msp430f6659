/*
 * Copyright (C) 2020 Banyan Water
 * timer.c
 * HAL for manipulating the MSP430's Timers
 */

#include "timer.h"

#include <msp430.h>

#include "adc.h"

uint32_t hal_timer__millisecond_value = 0;

void hal_timer__setup_tick_timer(uint16_t clock_cycles_in_tick)
{
    /* Ensure the timer is stopped. */
    TA0CTL = 0;

    /* Run the timer from the ACLK. */
    TA0CTL = TASSEL_1;

    /* Clear everything to start with. */
    TA0CTL |= TACLR;

    /* Set the compare match value according to the tick rate we want. */
    TA0CCR0 = clock_cycles_in_tick - 1;

    /* Enable the interrupts. */
    TA0CCTL0 = CCIE;

    /* Start up clean. */
    TA0CTL |= TACLR;

    /* Up mode. */
    TA0CTL |= MC_1;
}

void hal_timer__setup_millisecond_timer(uint16_t cycles_per_millisecond)
{
    hal_timer__millisecond_value = 0;

    /* Ensure the timer is stopped. */
    TA2CTL = 0;

    /* Run the timer from the MCLK. */
    TA2CTL = TASSEL_2;

    /* Clear everything to start with. */
    TA2CTL |= TACLR;

    /* Set the compare match value according to the tick rate we want. */
    TA2CCR0 = cycles_per_millisecond - 1;

    /* Enable the interrupts. */
    TA2CCTL0 = CCIE;

    /* Start up clean. */
    TA2CTL |= TACLR;

    /* Up mode. */
    TA2CTL |= MC_1;
}

inline uint32_t hal_timer__get_millisecond()
{
    return hal_timer__millisecond_value;
}

/******************************************************************************
 * USCI_A2_ISR
 * Modem UART ISR
 *****************************************************************************/
uint16_t adc_divider = 0;
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER2_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER2_A0_VECTOR)))
#endif
    void
    TIMER2_A0_ISR(void)
{
    hal_timer__millisecond_value++;

    /* TODO: Schedule the ADC read value might belong elsewhere */
    if ((++adc_divider) > 100)
    {
        adc_divider = 0;
        hal_adc__trigger_conversion();
    }
}
