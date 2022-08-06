/*
 * Copyright (C) 2020 Banyan Water
 * usb.c
 * HAL for the USB Interface
 */

#include <stdbool.h>

#include <msp430.h>
#include "usb/descriptors.h"
#include "usb/USB_APP/usbConstructs.h"
#include "usb/USB_API/USB_CDC_API/UsbCdc.h"

#include "usb.h"

#define HAL_USB__WRITE_MAX_SIZE 64

void hal_usb__init()
{
    USB_setup(TRUE, TRUE);
}

uint16_t hal_usb__read(volatile char *buffer, uint16_t buffer_size)
{
    return USBCDC_receiveDataInBuffer((uint8_t *)(buffer), buffer_size, CDC0_INTFNUM); /* Add bytes in USB buffer to the string */
}

uint16_t hal_usb__write(volatile const char *buffer, uint16_t buffer_size)
{
    /* Protect against zero size transmissions */
    if (!buffer_size)
    {
        return 0;
    }
    /* Cap the maximum transmission length */
    if (buffer_size > HAL_USB__WRITE_MAX_SIZE)
    {
        buffer_size = HAL_USB__WRITE_MAX_SIZE;
    }
    switch (USBCDC_sendDataInBackground((uint8_t *)(buffer), buffer_size, CDC0_INTFNUM, 5))
    {
    /* Success */
    case 0:
        return buffer_size;

    /* Waiting for the previous send to finish */
    case 1:
        return 0;

    /* Failure or no bus available, throw the data away */
    default:
        return buffer_size;
    }
}

bool hal_usb__get_connected()
{
    return USB_getConnectionState() == ST_ENUM_ACTIVE;
}
