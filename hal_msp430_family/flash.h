/*
 * Copyright (C) 2020 Banyan Water
 * flash.h
 * HAL for writing to the msp430's flash memory
 */

#ifndef MSP430_HAL_FLASH_H_
#define MSP430_HAL_FLASH_H_

#include <stdint.h>


/******************************************************************************
 * Flash Bank Definitions
 *****************************************************************************/
#define HAL_FLASH__BANK_0_ADDRESS 0x008000
#define HAL_FLASH__BANK_1_ADDRESS 0x028000
#define HAL_FLASH__BANK_2_ADDRESS 0x048000
#define HAL_FLASH__BANK_3_ADDRESS 0x068000

#define HAL_FLASH__BANK_SIZE 0x020000

/******************************************************************************
 * hal_flash__erase_bank
 * Erases a flash segment. Must be a ram function since it may erase itself
 * uint32_t bank_address - an address in the bank to erase
 *****************************************************************************/
__attribute__((ramfunc)) void hal_flash__erase_bank(uint32_t bank_address);

/******************************************************************************
 * hal_flash__copy
 * Copies the contents of one block of flash to another
 * uint8_t * dest - the start address of the destination flash segment
 * uint8_t * src - the start address of the source flash segment
 * uint16_t count - the number of bytes to copy
 *****************************************************************************/
__attribute__((ramfunc)) void hal_flash__copy(uint8_t *dest, uint8_t *src, uint32_t count);

/******************************************************************************
 * hal_flash__ram_reboot
 * Reboots the device. This function is specifically a ram function
 *****************************************************************************/
__attribute__((ramfunc)) void hal_flash__ram_reboot();

#endif /* MSP430_HAL_FLASH_H_ */
