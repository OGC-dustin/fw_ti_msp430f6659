/*
 * Copyright (C) 2020 Banyan Water
 * adc.h
 * HAL for manipulating the MSP430 ADC
 */

#ifndef MSP430_HAL_ADC_H_
#define MSP430_HAL_ADC_H_

#include <msp430.h>
#include <stdint.h>

#define HAL_ADC__TEMPERATURE_CHANNEL 10

/******************************************************************************
 * hal_adc__init
 * Configures the ADC
 *****************************************************************************/
void hal_adc__init();

/******************************************************************************
 * hal_adc__trigger_conversion
 * Triggers a conversion cycle of all channel on the ADC
 *****************************************************************************/
void hal_adc__trigger_conversion();

/******************************************************************************
 * hal_adc__read
 * Reads a value from the ADC
 * uint8_t msp430_channel_id - the channel to measure
 * return uint16_t - the measured value
 *****************************************************************************/
uint16_t hal_adc__read(uint8_t msp430_channel_id);

/******************************************************************************
 * hal_adc__read_temp
 * Reads the temperature sensor
 * return uint8_t - the measured temperature in degrees Celsius
 *****************************************************************************/
uint8_t hal_adc__read_temp();

/******************************************************************************
 * hal_adc__read_avcc
 * Reads the rail voltage measured against the internal 2.5 V reference
 * return uint16_t - the measured rail voltage * 1024
 *****************************************************************************/
uint16_t hal_adc__read_avcc();

/******************************************************************************
 * hal_adc__avcc_cal
 * Converts a raw ADC value referenced to AVCC to a voltage
 * uint16_t adc_value - the raw adc value
 * return uint16_t - the measured voltage * 1024
 *****************************************************************************/
uint16_t hal_adc__avcc_cal(uint16_t adc_value);

#endif /* MSP430_HAL_ADC_H_ */
