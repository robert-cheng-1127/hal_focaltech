/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file    ft9001_hal_cache.h
 * @brief   FT9001 Cache HAL.
 *
 * Regions:
 *   - BOOT  : CSACR.ROMR_x (4 slots)
 *   - ROM   : CACR.ROM_*   (cacheable / WT_WB)
 *   - SPIM1 : CSACR.SPI1_x (4 slots)
 *   - SPIM2 : CSACR.SPI2_x (4 slots)
 *   - SPIM3 : CSACR.SPI3_x (4 slots)
 *
 * Maintenance:
 *   - Global invalidate via CCR.INVW1|INVW0 + GO
 *   - Range invalidate via CPEA/CPES (auto 16B align)
 *
 * See ft9001.h for register/bit definitions.  // CCR.ENCache/GO, CACR/CSACR, CPES etc.
 */

#ifndef FT9001_HAL_CACHE_H_
#define FT9001_HAL_CACHE_H_

#include <stdbool.h>
#include <stdint.h>
#include "ft9001.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Cache policy for a region. */
typedef enum {
	HAL_CACHE_MODE_OFF = 0,       /*!< Not cacheable (clear CACHEABLE/WT_WB) */
	HAL_CACHE_MODE_WRITE_THROUGH, /*!< Cacheable + WT (clear WT_WB)          */
	HAL_CACHE_MODE_WRITE_BACK,    /*!< Cacheable + WB (set   WT_WB)          */
} HAL_CACHE_Mode;

/** @brief Logical region selector (maps to CACR/CSACR fields). */
typedef enum {
	HAL_CACHE_REGION_BOOT = 0,     /*!< BOOT: CSACR.ROMR_x (4 slots) */
	HAL_CACHE_REGION_ROM,          /*!< ROM : CACR.ROM_*             */
	HAL_CACHE_REGION_SPIM1,        /*!< SPI1: CSACR.SPI1_x           */
	HAL_CACHE_REGION_SPIM2,        /*!< SPI2: CSACR.SPI2_x           */
	HAL_CACHE_REGION_SPIM3,        /*!< SPI3: CSACR.SPI3_x           */
} HAL_CACHE_Region;

/**
 * @brief Enable or disable the cache.
 * @param inst   Cache instance (ICACHE or DCACHE).
 * @param enable true to enable, false to disable.
 */
void HAL_CACHE_Enable(CACHE_TypeDef *inst, bool enable);

/**
 * @brief Set cache policy for one region.
 * @param inst   Cache instance.
 * @param region Target region (BOOT / ROM / SPIM1 / SPIM2 / SPIM3).
 * @param mode   Policy: OFF, WRITE_THROUGH, or WRITE_BACK.
 */
void HAL_CACHE_SetRegionMode(CACHE_TypeDef *inst,
							 HAL_CACHE_Region region,
							 HAL_CACHE_Mode mode);

/**
 * @brief Configure policies for all regions in one call.
 * @param inst   Cache instance.
 * @param boot   BOOT region policy.
 * @param rom    ROM  region policy.
 * @param spim1  SPIM1 region policy.
 * @param spim2  SPIM2 region policy.
 * @param spim3  SPIM3 region policy.
 */
void HAL_CACHE_ConfigRegions(CACHE_TypeDef *inst,
							 HAL_CACHE_Mode boot,
							 HAL_CACHE_Mode rom,
							 HAL_CACHE_Mode spim1,
							 HAL_CACHE_Mode spim2,
							 HAL_CACHE_Mode spim3);

/**
 * @brief Global invalidate (all ways/lines).
 * @param inst Cache instance.
 * @note  Uses CCR (INVW1|INVW0|GO) and waits until GO clears.
 */
void HAL_CACHE_InvalidateAll(CACHE_TypeDef *inst);

/**
 * @brief Range invalidate (auto 16‑byte alignment).
 * @param inst Cache instance.
 * @param addr Base address (no alignment required).
 * @param size Length in bytes (no alignment required).
 * @note  Uses CPEA/CPES to trigger; no-op if ENCACHE == 0.
 */
void HAL_CACHE_InvalidateRange(CACHE_TypeDef *inst, uint32_t addr, uint32_t size);

/**
 * @brief One-shot init: disable -> config regions -> invalidate-all -> enable.
 * @param inst   Cache instance.
 * @param boot   BOOT region policy.
 * @param rom    ROM  region policy.
 * @param spim1  SPIM1 region policy.
 * @param spim2  SPIM2 region policy.
 * @param spim3  SPIM3 region policy.
 * @note  Performs a global invalidate every call.
 */
void HAL_CACHE_Init(CACHE_TypeDef *inst,
					HAL_CACHE_Mode boot,
					HAL_CACHE_Mode rom,
					HAL_CACHE_Mode spim1,
					HAL_CACHE_Mode spim2,
					HAL_CACHE_Mode spim3);

#ifdef __cplusplus
}
#endif

#endif /* FT9001_HAL_CACHE_H_ */
