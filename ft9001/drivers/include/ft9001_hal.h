/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef FT9001_HAL_H_
#define FT9001_HAL_H_

#include <stdint.h>
#include "ft9001_hal_cpm.h"
#include "ft9001_hal_wdt.h"
#include "ft9001_hal_tc.h"
#include "ft9001_hal_cache.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t *HAL_GetVersion(void);

#ifdef __cplusplus
}
#endif

#endif /* FT9001_HAL_H_ */
