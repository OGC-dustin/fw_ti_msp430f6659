/*
 * Copyright (C) 2020 Banyan Water
 * pmem.c
 * HAL for access the info memory sections
 */

#include "pmem.h"

#include <stdint.h>
#include "driverlib.h"

/* Setup for segment c */
#pragma RETAIN(hal_pmem__info_c_segment)                    /* Retain prevents the symbol from being removed if it is not accessed */
#pragma DATA_SECTION(hal_pmem__info_c_segment, ".infoC")    /* Tells the compiler to place the symbol in the infoC area */
struct hal_pmem__info_c_segment_t hal_pmem__info_c_segment; /* This variable points to the infoC flash segment */

/* Setup for segment d */
#pragma RETAIN(hal_pmem__info_d_segment)                    /* Retain prevents the symbol from being removed if it is not accessed */
#pragma DATA_SECTION(hal_pmem__info_d_segment, ".infoD")    /* Tells the compiler to place the symbol in the infoD area */
struct hal_pmem__info_d_segment_t hal_pmem__info_d_segment; /* This variable points to the infoD flash segment */

bool hal_pmem__write_segment(void *segment_dest, void *segment_src)
{
    if (((uint32_t)segment_dest == (uint32_t)(&hal_pmem__info_c_segment)) ||
        ((uint32_t)segment_dest == (uint32_t)(&hal_pmem__info_d_segment)))
    {
        /* Write the segment */
        FlashCtl_eraseSegment(segment_dest);
        FlashCtl_write8(segment_src, segment_dest, 128);
        return true;
    }
    else
    {
        return false;
    }
}

const struct hal_pmem__info_c_segment_t *hal_pmem__get_info_c_segment()
{
    return &hal_pmem__info_c_segment;
}

const struct hal_pmem__info_d_segment_t *hal_pmem__get_info_d_segment()
{
    return &hal_pmem__info_d_segment;
}
