/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ft9001_hal.h"

static uint8_t __FT9001_HAL_VERSION[] = "v1.0.0";

uint8_t *HAL_GetVersion(void)
{
	return __FT9001_HAL_VERSION;
}
