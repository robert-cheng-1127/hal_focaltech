/*
 * Copyright (c) 2025-2026, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef FT9001_HAL_H_
#define FT9001_HAL_H_

#include "ft9001_cpm.h"
#include "ft9001_tc.h"
#include "ft9001_wdt.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Version of this HAL, as a "vMAJOR.MINOR.PATCH" string. */
const char *ft9001_hal_version_get(void);

#ifdef __cplusplus
}
#endif

#endif /* FT9001_HAL_H_ */
