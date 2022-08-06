/*
 * Copyright (C) 2020 Banyan Water
 * system.c
 * HAL for system functions like rebooting and watchdog timer manipulation
 */

#include <msp430.h>
#include "driverlib.h"

#include "system.h"

void hal_system__watchdog_stop()
{
    WDT_A_hold(WDT_A_BASE);
}
void hal_system__watchdog_start()
{
    WDT_A_start(WDT_A_BASE);
}

void hal_system__watchdog_configure()
{
    WDT_A_hold(WDT_A_BASE);
    WDT_A_initWatchdogTimer(WDT_A_BASE, WDT_A_CLOCKSOURCE_VLOCLK, WDT_A_CLOCKDIVIDER_32K);
    WDT_A_start(WDT_A_BASE);
}

void hal_system__watchdog_reset()
{
    WDT_A_resetTimer(WDT_A_BASE);
}

void hal_system__reboot()
{
    /* Initiate a BOR */
    PMMCTL0 = PMMPW | PMMSWBOR;
}

const char *hal_system__boot_code()
{
    static bool sysrstiv_read = false;
    static uint16_t sysrstiv;

    /* Read and clear the sysrstiv the first time calling */
    if (!sysrstiv_read)
    {
        sysrstiv = SYSRSTIV;
        SYSRSTIV = 0;
    }

    switch (sysrstiv)
    {
    case SYSRSTIV_NONE: /* (0x0000u)    No Interrupt pending */
        return "NONE";
    case SYSRSTIV_BOR: /* (0x0002u)    SYSRSTIV : BOR */
        return "BOR";
    case SYSRSTIV_RSTNMI: /* (0x0004u)    SYSRSTIV : RST/NMI */
        return "RST_NMI";
    case SYSRSTIV_DOBOR: /* (0x0006u)    SYSRSTIV : Do BOR */
        return "Do_BOR";
    case SYSRSTIV_LPM5WU: /* (0x0008u)    SYSRSTIV : Port LPM5 Wake Up */
        return "LP_Wake";
    case SYSRSTIV_SECYV: /* (0x000Au)    SYSRSTIV : Security violation ( access of protected areas in flash such as protected BSL ) */
        return "Security_Violation";
    case SYSRSTIV_SVSL: /* (0x000Cu)    SYSRSTIV : SVSL */
        return "SVSL";
    case SYSRSTIV_SVSH: /* (0x000Eu)    SYSRSTIV : SVSH */
        return "SVSH";
    case SYSRSTIV_SVML_OVP: /* (0x0010u)    SYSRSTIV : SVML_OVP */
        return "SVML_OV";
    case SYSRSTIV_SVMH_OVP: /* (0x0012u)    SYSRSTIV : SVMH_OVP */
        return "SVMH_OV";
    case SYSRSTIV_DOPOR: /* (0x0014u)    SYSRSTIV : Do POR */
        return "Do_POR";
    case SYSRSTIV_WDTTO: /* (0x0016u)    SYSRSTIV : WDT Time out */
        return "WDT_timeout";
    case SYSRSTIV_WDTKEY: /* (0x0018u)    SYSRSTIV : WDTKEY violation */
        return "WDT_key_violation";
    case SYSRSTIV_KEYV: /* (0x001Au)    SYSRSTIV : Flash Key violation */
        return "FLASH_key_violation";
#ifdef SYSRSTIV_FLLUL
    case SYSRSTIV_FLLUL: /* (0x001Cu)    SYSRSTIV : FLL unlock */
        return "FLL_unlock";
#endif
    case SYSRSTIV_PERF: /* (0x001Eu)    SYSRSTIV : peripheral/config area fetch */
        return "PERF";
    case SYSRSTIV_PMMKEY: /* (0x0020u)    SYSRSTIV : PMMKEY violation */
        return "PMM_KEY";
    default:
        return "UNKNOWN_SYSRSTIV";
    }
}

volatile uint8_t hal_system__sysnmi_count = 0;
volatile uint8_t hal_system__unmi_count = 0;

void hal_system__nmi_count(uint8_t *sysnmi_count, uint8_t *unmi_count)
{
    *sysnmi_count = hal_system__sysnmi_count;
    *unmi_count = hal_system__unmi_count;
}

/* SYSNMI_ISR defined to make sure it is handled correctly */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = SYSNMI_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(SYSNMI_VECTOR)))
#endif
    void
    SYSNMI_ISR(void)
{
    if (hal_system__sysnmi_count < UINT8_MAX)
    {
        hal_system__sysnmi_count++;
    }
}

/* UNMI defined to make sure it is handled correctly */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = UNMI_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(UNMI_VECTOR)))
#endif
    void
    UNMI_ISR(void)
{
    if (hal_system__unmi_count < UINT8_MAX)
    {
        hal_system__unmi_count++;
    }
}
