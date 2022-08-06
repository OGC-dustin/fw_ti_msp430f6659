/*
 * Copyright (C) 2020 Banyan Water
 * adc.A
 * HAL for manipulating the MSP430 ADC
 */
#include "adc.h"
#include <driverlib.h>

#include <string.h>

#define HAL_ADC__ALPHA 3

#define HAL_ADC__CHANNEL_COUNT 16
#define HAL_ADC__AVCC_OVER_TWO_CHANNEL 11

struct s_TLV_ADC_Cal_Data *hal_adc__adc_cal_data;
struct s_TLV_REF_Cal_Data *hal_adc__ref_cal_data;

volatile uint32_t hal_adc__accumulator[HAL_ADC__CHANNEL_COUNT] = {0};

/******************************************************************************
 * hal_adc__get_average
 * Get the average from an accumulator
 *****************************************************************************/
inline uint16_t hal_adc__get_average(uint32_t accumulator)
{
    return (accumulator + (1 << (HAL_ADC__ALPHA - 1))) >> HAL_ADC__ALPHA;
}

/******************************************************************************
 * hal_adc__accumulate
 * Adds a new value to an accumulator
 *****************************************************************************/
inline uint32_t hal_adc__accumulate(uint16_t new_value, uint32_t accumulator)
{
    return new_value + accumulator - hal_adc__get_average(accumulator);
}

/******************************************************************************
 * hal_adc__ref_cal
 * Calibrates a raw ADC value measured against the 2.5 V reference.
 *      Derived from the user guide
 *****************************************************************************/
inline uint16_t hal_adc__ref_cal(uint16_t new_value)
{
    return (((uint32_t)(new_value)) * hal_adc__ref_cal_data->ref_ref25) >> 15;
}

void hal_adc__init()
{
    /* Setup the Ref Module to 2.5 Volts*/
    Ref_setReferenceVoltage(REF_BASE, REF_VREF2_5V);
    Ref_enableTempSensor(REF_BASE);
    Ref_enableReferenceVoltage(REF_BASE);

    /* Clear the accumulators */
    memset(&hal_adc__accumulator, 0, sizeof(hal_adc__accumulator));

    /* Get calibration data */
    uint8_t length;
    TLV_getInfo(TLV_TAG_ADC12CAL, 0, &length, (uint16_t **)(&hal_adc__adc_cal_data));
    TLV_getInfo(TLV_TAG_REFCAL, 0, &length, (uint16_t **)(&hal_adc__ref_cal_data));

    /* Configure the ADC registers */
    ADC12_A_init(ADC12_A_BASE, ADC12_A_SAMPLEHOLDSOURCE_SC, ADC12_A_CLOCKSOURCE_ADC12OSC, UCS_CLOCK_DIVIDER_2);
    ADC12_A_enableReferenceBurst(ADC12_A_BASE);
    ADC12_A_disableConversions(ADC12_A_BASE, ADC12_A_PREEMPTCONVERSION);
    ADC12_A_setupSamplingTimer(ADC12_A_BASE, ADC12_A_CYCLEHOLD_1024_CYCLES, ADC12_A_CYCLEHOLD_1024_CYCLES, ADC12_A_MULTIPLESAMPLESENABLE);
    ADC12_A_setReferenceBufferSamplingRate(ADC12_A_BASE, ADC12_A_MAXSAMPLINGRATE_50KSPS);
    /* Disable all interupts */
    ADC12IE = 0;
    /* Clear all interupts */
    ADC12IFG = 0;

    /* Configure the memory control registers */
    uint8_t channel;
    ADC12_A_configureMemoryParam params;

    for (channel = 0; channel < HAL_ADC__CHANNEL_COUNT; channel++)
    {
        /* Default channels */
        params.inputSourceSelect = channel;
        params.memoryBufferControlIndex = channel;
        params.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
        params.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
        params.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
        /* Final Channel */
        if (params.memoryBufferControlIndex == ADC12_A_MEMORY_15)
        {
            params.endOfSequence = ADC12_A_ENDOFSEQUENCE;
        }

        /* Temperature Sensor Channel */
        if (params.memoryBufferControlIndex == HAL_ADC__TEMPERATURE_CHANNEL)
        {
            params.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_INT;
        }
        /* AVCC /2 Reference Channel */
        if (params.memoryBufferControlIndex == HAL_ADC__AVCC_OVER_TWO_CHANNEL)
        {
            params.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_INT;
        }

        ADC12_A_configureMemory(ADC12_A_BASE, &params);
    }

    /* Enable interrupts */
    ADC12IE = 0xFFFF;
    ADC12_A_enable(ADC12_A_BASE);
}

void hal_adc__trigger_conversion()
{
    ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_0, ADC12_A_SEQOFCHANNELS);
}

uint16_t hal_adc__read(uint8_t msp430_channel_id)
{
    if (msp430_channel_id < 16)
    {
        /* Apply the adc gain and offset calibration to the measured value. See MSP430 Userguide 1.13.5.2 */
        return (((uint32_t)hal_adc__get_average(hal_adc__accumulator[msp430_channel_id]) * hal_adc__adc_cal_data->adc_gain_factor) >> 15) + hal_adc__adc_cal_data->adc_offset;
    }
    else
    {
        return 0;
    }
}

uint8_t hal_adc__read_temp()
{
    /* Apply the temperature sensor calibration. See MSP430 Userguide 1.13.5.3 */
    int32_t raw_average = hal_adc__get_average(hal_adc__accumulator[HAL_ADC__TEMPERATURE_CHANNEL]);
    int32_t result = ((raw_average - hal_adc__adc_cal_data->adc_ref25_30_temp) * (85 - 30)) / (hal_adc__adc_cal_data->adc_ref25_85_temp - hal_adc__adc_cal_data->adc_ref25_30_temp) + 30;

    /* Make sure we don't overflow or underflow the 8 bit type */
    if (result < 0)
    {
        return 0;
    }
    else if (result < UINT8_MAX)
    {
        return result;
    }
    else
    {
        return UINT8_MAX;
    }
}

uint16_t hal_adc__read_avcc()
{
    /* Read the system rail voltage. AVCC/2 referenced and calibrated to the 2.5V reference and multipled by two. See MSP430 Userguide 1.13.5.1 */
    return ((uint32_t)5 * hal_adc__ref_cal(hal_adc__read(HAL_ADC__AVCC_OVER_TWO_CHANNEL))) >> 2;
}

uint16_t hal_adc__avcc_cal(uint16_t adc_value)
{
    /* Convert a raw ADC value referenced to AVCC to a voltage */
    uint32_t avcc_value = hal_adc__read_avcc();
    /* Output is Multiplied by 1024 */
    return (avcc_value * adc_value) >> 12;
}

/******************************************************************************
 * ADC12_ISR
 * ISR that accumlates ADC values
 *****************************************************************************/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC12_VECTOR)))
#endif
    void
    ADC12_ISR(void)
{
    uint8_t channel;
    switch (__even_in_range((unsigned int)ADC12IV, ADC12IV_ADC12IFG15))
    {
    case ADC12IV_NONE:
        return;
    case ADC12IV_ADC12IFG0:
        channel = 0;
        break;
    case ADC12IV_ADC12IFG1:
        channel = 1;
        break;
    case ADC12IV_ADC12IFG2:
        channel = 2;
        break;
    case ADC12IV_ADC12IFG3:
        channel = 3;
        break;
    case ADC12IV_ADC12IFG4:
        channel = 4;
        break;
    case ADC12IV_ADC12IFG5:
        channel = 5;
        break;
    case ADC12IV_ADC12IFG6:
        channel = 6;
        break;
    case ADC12IV_ADC12IFG7:
        channel = 7;
        break;
    case ADC12IV_ADC12IFG8:
        channel = 8;
        break;
    case ADC12IV_ADC12IFG9:
        channel = 9;
        break;
    case ADC12IV_ADC12IFG10:
        channel = 10;
        break;
    case ADC12IV_ADC12IFG11:
        channel = 11;
        break;
    case ADC12IV_ADC12IFG12:
        channel = 12;
        break;
    case ADC12IV_ADC12IFG13:
        channel = 13;
        break;
    case ADC12IV_ADC12IFG14:
        channel = 14;
        break;
    case ADC12IV_ADC12IFG15:
        channel = 15;
        break;
    }
    hal_adc__accumulator[channel] = hal_adc__accumulate(ADC12_A_getResults(ADC12_A_BASE, channel), hal_adc__accumulator[channel]);
}
