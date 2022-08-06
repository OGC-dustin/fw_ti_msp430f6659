/*
 * TI MSP430F6659 HAL
 * Function availability and configuration based on hardware description header
 */

#include <msp430.h>

#include "driverlib.h"
#include "timer.h"
#include "hal_msp430f6659.h"
#include "hw_description.h"

void hal_clock_xt1__init( void )
{
    /* XT1 32.768kHz RTC is used for ACLK */
    UCS_initClockSignal(
        UCS_ACLK,
        UCS_XT1CLK_SELECT,
        UCS_CLOCK_DIVIDER_1);
}

void hal_clock_xt2__init( void )
{
    /* Configure the XT2 pins for their special function as crystal drivers */
    P7DIR |= BIT2 | BIT3;
    P7SEL |= BIT2 | BIT3;

    /* Turn on XT2 */
    UCS_turnOnXT2(XT2DRIVE_0);
    //UCS_turnOnLFXT1()
    //UCS_turnOnXT2WithTimeout(XT2DRIVE_0, 50000);

    /* Set the core voltage to the highest setting so that the device can run on 16 MHz */
    PMM_setVCore(PMM_CORE_LEVEL_3);

    /* Use the 4 MHz XT2 to drive the FLL output up to 20 MHz */
    UCS_initClockSignal(
        UCS_FLLREF,
        UCS_XT2CLK_SELECT,
        UCS_CLOCK_DIVIDER_2);

    UCS_initFLLSettle(
        (20000000 / 1000),
        (20000000 / 2000000));

    /* SMCLK to 20 MHZ */
    UCS_initClockSignal(
        UCS_SMCLK,
        UCS_DCOCLK_SELECT,
        UCS_CLOCK_DIVIDER_1);

    /* MCLK to 20 MHz */
    UCS_initClockSignal(
        UCS_MCLK,
        UCS_DCOCLK_SELECT,
        UCS_CLOCK_DIVIDER_1);
}

uint64_t tick_system = 0U;
uint64_t hal_get_sys_tick( void )
{
    return ( tick_system );
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR( void )
{
    tick_system++;                      /* increment tick timestamp */
}

void hal_led_heartbeat( int8_t state )
{
    switch ( state )
    {
        case 1:
            __no_operation();
            break;
        case 0:
            __no_operation();
            break;
        default:
            __no_operation();
    };
    return;
}

void hal_init( void )
{
    WDTCTL = WDTPW | WDTHOLD;       /* stop watchdog timer */
    hal_clock_xt1__init();
    hal_clock_xt2__init();

    hal_timer__setup_tick_timer( 32768 / 1000 ); /* TODO: verify this is ~1 millisecond per tick as a test */

    _enable_interrupt();

    return;
}
