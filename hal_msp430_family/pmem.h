/*
 * Copyright (C) 2020 Banyan Water
 * pmem.h
 * HAL for access the info memory sections
 */

#ifndef MSP430_HAL_PMEM_H_
#define MSP430_HAL_PMEM_H_

#include <stdbool.h>

/******************************************************************************
 * hal_pmem__info_c_segment_t
 * Memory layout of segment C
 *****************************************************************************/
struct hal_pmem__info_c_segment_t
{
    char update_key[16];
    char last_will[16];
}; /* Max 128 Byte */
extern struct hal_pmem__info_c_segment_t hal_pmem__info_c_segment;

/******************************************************************************
 * hal_pmem__info_d_segment_t
 * Memory layout of segment D
 *****************************************************************************/
struct hal_pmem__info_d_segment_t
{
    char serial_number[11];
}; /* Max 128 Byte */
extern struct hal_pmem__info_d_segment_t hal_pmem__info_d_segment;

/******************************************************************************
 * hal_pmem__write_segment
 * Writes the indicated segment
 * void *segment_dest - the address of the destination segment
 * void *segment_src - the address of the memory that holds what should be
 *      written
 * return bool - whether the operation was successfully completed
 *****************************************************************************/
bool hal_pmem__write_segment(void *segment_dest, void *segment_src);

/******************************************************************************
 * hal_pmem__get_info_c_segment
 * Gets a pointer to the c segment struct
 * const struct hal_pmem__info_c_segment_t * - the c segment struct
 *****************************************************************************/
const struct hal_pmem__info_c_segment_t *hal_pmem__get_info_c_segment();

/******************************************************************************
 * hal_pmem__get_info_d_segment
 * Gets a pointer to the d segment struct
 * const struct hal_pmem__info_d_segment_t * - the d segment struct
 *****************************************************************************/
const struct hal_pmem__info_d_segment_t *hal_pmem__get_info_d_segment();

#endif /* MSP430_HAL_PMEM_H_ */
