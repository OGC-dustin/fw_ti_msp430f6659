/*
 * Copyright (C) 2020 Banyan Water
 * system.h
 * HAL for system functions like rebooting and watchdog timer manipulation
 */

#ifndef MSP430_HAL_SYSTEM_H_
#define MSP430_HAL_SYSTEM_H_

/******************************************************************************
 * hal_system__watchdog_stop
 * Stops the watchdog
 *****************************************************************************/
void hal_system__watchdog_stop();

/******************************************************************************
 * hal_system__watchdog_start
 * Starts the watchdog
 *****************************************************************************/
void hal_system__watchdog_start();

/******************************************************************************
 * hal_system__watchdog_configure
 * Stops and configures the watchdog
 *****************************************************************************/
void hal_system__watchdog_configure();

/******************************************************************************
 * hal_system__watchdog_reset
 * Resets the watchdog
 *****************************************************************************/
void hal_system__watchdog_reset();

/******************************************************************************
 * hal_system__reboot
 * Reboots the device
 *****************************************************************************/
void hal_system__reboot();

/******************************************************************************
 * hal_system__boot_code
 * Retrieves the boot code for the device
 * return const char * - a string with the boot code information
 *****************************************************************************/
const char *hal_system__boot_code();

/******************************************************************************
 * hal_system__nmi_count
 * Retrieves how many times non maskable ISRs have been called
 * uint8_t sysnmi_count - the number of system nmi occurrences
 * uint8_t unmi_count - the number of user nmi occurrences
 *****************************************************************************/
void hal_system__nmi_count(uint8_t *sysnmi_count, uint8_t *unmi_count);

#endif /* MSP430_HAL_SYSTEM_H_ */
