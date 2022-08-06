/*
 * Copyright (C) 2020 Banyan Water
 * usb.h
 * HAL for the USB Interface
 */

#ifndef MSP430_HAL_USB_H_
#define MSP430_HAL_USB_H_

#include "stdbool.h"

/******************************************************************************
 * hal_usb__init
 * Initializes the USB Interface
 *****************************************************************************/
void hal_usb__init();

/******************************************************************************
 * hal_usb__read
 * Reads any pending data from the USB Interface. The buffer will not be null
 *      terminated as null may be valid serial data.
 * volatile char* buffer - the buffer for the read data
 * uint16_t buffer_size - the size of the receiving buffer
 * return uint16_t - The number of characters read
 *****************************************************************************/
uint16_t hal_usb__read(volatile char *buffer, uint16_t buffer_size);

/******************************************************************************
 * hal_usb__write
 * Writes the buffer to the USB Interface. The buffer must be null terminated
 * Attempts to write data to the USB Interface
 * char *buffer - the buffer containing the data to write
 * uint16_t buffer_size - the number of bytes to write to the interface
 * return uint16_t - the number of bytes successfully written to the interface
 *****************************************************************************/
uint16_t hal_usb__write(volatile const char *buffer, uint16_t buffer_size);

/******************************************************************************
 * hal_usb__get_connected
 * Returns whether the device is connected to a usb host
 * return bool - whether the device is connected or not
 *****************************************************************************/
bool hal_usb__get_connected();

#endif /* MSP430_HAL_USB_H_ */
