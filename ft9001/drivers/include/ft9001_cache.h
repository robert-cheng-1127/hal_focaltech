/*
 * Copyright (c) 2025-2026, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file    ft9001_cache.h
 * @brief   FT9001 cache configuration and maintenance.
 *
 * Regions map onto register fields as BOOT to CSACR.ROMR_x (4 slots), ROM to
 * CACR.ROM_*, and SPIM1..3 to CSACR.SPIn_x (4 slots each). Maintenance runs
 * through CCR.INVW1|INVW0|GO for a global invalidate and through CPEA/CPES for a
 * range invalidate.
 */

#ifndef FT9001_CACHE_H_
#define FT9001_CACHE_H_

#include <stdint.h>

#include "ft9001.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Cache policy for a region. */
enum ft9001_cache_mode {
	/** Not cacheable: CACHEABLE and WT_WB both clear. */
	FT9001_CACHE_MODE_OFF = 0,
	/** Cacheable, write-through: WT_WB clear. */
	FT9001_CACHE_MODE_WRITE_THROUGH,
	/** Cacheable, write-back: WT_WB set. */
	FT9001_CACHE_MODE_WRITE_BACK,
};

/** @brief Logical region selector. */
enum ft9001_cache_region {
	FT9001_CACHE_REGION_BOOT = 0,
	FT9001_CACHE_REGION_ROM,
	FT9001_CACHE_REGION_SPIM1,
	FT9001_CACHE_REGION_SPIM2,
	FT9001_CACHE_REGION_SPIM3,
};

/** @brief Region policies applied together by the bulk helpers. */
struct ft9001_cache_config {
	enum ft9001_cache_mode boot;
	enum ft9001_cache_mode rom;
	enum ft9001_cache_mode spim1;
	enum ft9001_cache_mode spim2;
	enum ft9001_cache_mode spim3;
};

/** @brief Enable the cache instance (CCR.ENCACHE). */
static inline void ft9001_cache_enable(CACHE_TypeDef *inst)
{
	FT9001_SET_BIT(inst->CACHE_CCR, CACHE_CCR_ENCACHE);
}

/** @brief Disable the cache instance (CCR.ENCACHE). */
static inline void ft9001_cache_disable(CACHE_TypeDef *inst)
{
	FT9001_CLEAR_BIT(inst->CACHE_CCR, CACHE_CCR_ENCACHE);
}

/** @brief Set the policy for one region. */
void ft9001_cache_region_mode_set(CACHE_TypeDef *inst, enum ft9001_cache_region region,
				  enum ft9001_cache_mode mode);

/** @brief Set the policy for every region in one call. */
void ft9001_cache_regions_configure(CACHE_TypeDef *inst, const struct ft9001_cache_config *cfg);

/** @brief Invalidate all ways and lines, waiting for CCR.GO to clear. */
void ft9001_cache_invalidate_all(CACHE_TypeDef *inst);

/**
 * @brief Invalidate an address range.
 *
 * The start is aligned down and the length up to the 16-byte line size, so no
 * alignment is required from the caller. Does nothing while the cache is off.
 */
void ft9001_cache_invalidate_range(CACHE_TypeDef *inst, uint32_t addr, uint32_t size);

/**
 * @brief Bring a cache instance up: disable, configure, invalidate, enable.
 *
 * Performs a global invalidate on every call.
 */
void ft9001_cache_init(CACHE_TypeDef *inst, const struct ft9001_cache_config *cfg);

#ifdef __cplusplus
}
#endif

#endif /* FT9001_CACHE_H_ */
