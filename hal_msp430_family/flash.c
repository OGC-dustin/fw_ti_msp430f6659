/*
 * Copyright (C) 2020 Banyan Water
 * flash.c
 * HAL for writing to the msp430's flash memory
 */

#include "flash.h"

#include <msp430.h>
#include <stdint.h>

__attribute__((ramfunc)) void hal_flash__erase_bank(uint32_t bank_address)
{
    /* The driverlib erase function is not a ramfunction */

    uint16_t interruptState;

    /* Clear Lock bit */
    FCTL3 = FWKEY;

    while (FCTL3 & BUSY)
        ;

    /* FLASH34 errata: no concurrent access to flash bank while erasing */
    interruptState = __get_interrupt_state();
    __disable_interrupt();
    __no_operation();

    /* Set MERAS bit */
    FCTL1 = FWKEY + MERAS;

    /* Dummy write to erase Flash seg */
    *(uint8_t *)bank_address = 0;

    /* test busy */
    while (FCTL3 & BUSY)
        ;

    /* FLASH34 errata */
    /* Re-enable interrupt state to whatever it was before */
    if (interruptState & GIE)
    {
        __enable_interrupt();
    }

    /* Clear MERAS bit */
    FCTL1 = FWKEY;

    /* Set LOCK bit */
    FCTL3 = FWKEY + LOCK;
}

__attribute__((ramfunc)) void hal_flash__copy(uint8_t *dest, uint8_t *src, uint32_t count)
{
    /* It appears that if interrupts occur while the flash is being written to, teh PC will be reset to 0. */
    /* Disabling interrupts for each byte seems to resolve this issue */
    /* Interrupts are only disabled for a short time period and shouldn't impact most systems */
    /* Any time sensitive interfaces (e.g. The hunter interface) should probably not be utilized during updates */

    uint16_t interruptState;

    /*Clear Lock bit */
    FCTL3 = FWKEY;

    interruptState = __get_interrupt_state();
    __disable_interrupt();
    __no_operation();

    /* Enable byte/word write mode */
    FCTL1 = FWKEY + WRT;

    while (count > 0)
    {
        /* test busy */
        while (FCTL3 & BUSY)
            ;

        /* Write to Flash */
        *dest++ = *src++;
        count--;
    }

    if (interruptState & GIE)
    {
        __enable_interrupt();
    }

    /* Clear WRT bit */
    FCTL1 = FWKEY;

    /* Set LOCK bit */
    FCTL3 = FWKEY + LOCK;
}

__attribute__((ramfunc)) void hal_flash__ram_reboot()
{
    /* Initiate a BOR */
    PMMCTL0 = PMMPW | PMMSWBOR;
}
