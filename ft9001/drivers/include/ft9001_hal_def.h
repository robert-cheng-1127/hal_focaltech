/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef FT9001_HAL_DEF_H_
#define FT9001_HAL_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

typedef enum {
	HAL_OK      = 0x00U,
	HAL_ERROR   = 0x01U,
	HAL_BUSY    = 0x02U,
	HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;

#ifdef __cplusplus
}
#endif

#endif /* FT9001_HAL_DEF_H_ */
