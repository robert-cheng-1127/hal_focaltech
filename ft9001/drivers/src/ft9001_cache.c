/*
 * Copyright (c) 2025-2026, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdbool.h>

#include "ft9001_cache.h"

#define CACHE_LINE_SIZE (16U)

static inline bool cache_is_enabled(CACHE_TypeDef *inst)
{
	return FT9001_READ_BIT(inst->CACHE_CCR, CACHE_CCR_ENCACHE) != 0U;
}

static inline void cache_wait_go_clear(CACHE_TypeDef *inst)
{
	while (FT9001_READ_BIT(inst->CACHE_CCR, CACHE_CCR_GO)) {
	}
}

static inline void cache_start_cmd(CACHE_TypeDef *inst, uint32_t ccr_bits)
{
	FT9001_SET_BIT(inst->CACHE_CCR, ccr_bits | CACHE_CCR_GO);
	cache_wait_go_clear(inst);
}

static void cache_apply_mode(uint32_t *reg, uint32_t cacheable_mask, uint32_t wt_wb_mask,
			     enum ft9001_cache_mode mode)
{
	switch (mode) {
	case FT9001_CACHE_MODE_OFF:
		FT9001_CLEAR_BIT(*reg, cacheable_mask | wt_wb_mask);
		break;
	case FT9001_CACHE_MODE_WRITE_THROUGH:
		FT9001_SET_BIT(*reg, cacheable_mask);
		FT9001_CLEAR_BIT(*reg, wt_wb_mask);
		break;
	case FT9001_CACHE_MODE_WRITE_BACK:
	default:
		FT9001_SET_BIT(*reg, cacheable_mask | wt_wb_mask);
		break;
	}
}

void ft9001_cache_region_mode_set(CACHE_TypeDef *inst, enum ft9001_cache_region region,
				  enum ft9001_cache_mode mode)
{
	uint32_t v;

	switch (region) {
	case FT9001_CACHE_REGION_BOOT:
		v = FT9001_READ_REG(inst->CACHE_CSACR);
		cache_apply_mode(&v,
				 CACHE_CSACR_ROMR_3_CACHEABLE | CACHE_CSACR_ROMR_2_CACHEABLE |
					 CACHE_CSACR_ROMR_1_CACHEABLE |
					 CACHE_CSACR_ROMR_0_CACHEABLE,
				 CACHE_CSACR_ROMR_3_WT_WB | CACHE_CSACR_ROMR_2_WT_WB |
					 CACHE_CSACR_ROMR_1_WT_WB | CACHE_CSACR_ROMR_0_WT_WB,
				 mode);
		FT9001_WRITE_REG(inst->CACHE_CSACR, v);
		break;

	case FT9001_CACHE_REGION_ROM:
		v = FT9001_READ_REG(inst->CACHE_CACR);
		cache_apply_mode(&v, CACHE_CACR_ROM_CACHEABLE, CACHE_CACR_ROM_WT_WB, mode);
		FT9001_WRITE_REG(inst->CACHE_CACR, v);
		break;

	case FT9001_CACHE_REGION_SPIM1:
		v = FT9001_READ_REG(inst->CACHE_CSACR);
		cache_apply_mode(&v,
				 CACHE_CSACR_SPI1_3_CACHEABLE | CACHE_CSACR_SPI1_2_CACHEABLE |
					 CACHE_CSACR_SPI1_1_CACHEABLE |
					 CACHE_CSACR_SPI1_0_CACHEABLE,
				 CACHE_CSACR_SPI1_3_WT_WB | CACHE_CSACR_SPI1_2_WT_WB |
					 CACHE_CSACR_SPI1_1_WT_WB | CACHE_CSACR_SPI1_0_WT_WB,
				 mode);
		FT9001_WRITE_REG(inst->CACHE_CSACR, v);
		break;

	case FT9001_CACHE_REGION_SPIM2:
		v = FT9001_READ_REG(inst->CACHE_CSACR);
		cache_apply_mode(&v,
				 CACHE_CSACR_SPI2_3_CACHEABLE | CACHE_CSACR_SPI2_2_CACHEABLE |
					 CACHE_CSACR_SPI2_1_CACHEABLE |
					 CACHE_CSACR_SPI2_0_CACHEABLE,
				 CACHE_CSACR_SPI2_3_WT_WB | CACHE_CSACR_SPI2_2_WT_WB |
					 CACHE_CSACR_SPI2_1_WT_WB | CACHE_CSACR_SPI2_0_WT_WB,
				 mode);
		FT9001_WRITE_REG(inst->CACHE_CSACR, v);
		break;

	case FT9001_CACHE_REGION_SPIM3:
		v = FT9001_READ_REG(inst->CACHE_CSACR);
		cache_apply_mode(&v,
				 CACHE_CSACR_SPI3_3_CACHEABLE | CACHE_CSACR_SPI3_2_CACHEABLE |
					 CACHE_CSACR_SPI3_1_CACHEABLE |
					 CACHE_CSACR_SPI3_0_CACHEABLE,
				 CACHE_CSACR_SPI3_3_WT_WB | CACHE_CSACR_SPI3_2_WT_WB |
					 CACHE_CSACR_SPI3_1_WT_WB | CACHE_CSACR_SPI3_0_WT_WB,
				 mode);
		FT9001_WRITE_REG(inst->CACHE_CSACR, v);
		break;

	default:
		break;
	}
}

void ft9001_cache_regions_configure(CACHE_TypeDef *inst, const struct ft9001_cache_config *cfg)
{
	ft9001_cache_region_mode_set(inst, FT9001_CACHE_REGION_BOOT, cfg->boot);
	ft9001_cache_region_mode_set(inst, FT9001_CACHE_REGION_ROM, cfg->rom);
	ft9001_cache_region_mode_set(inst, FT9001_CACHE_REGION_SPIM1, cfg->spim1);
	ft9001_cache_region_mode_set(inst, FT9001_CACHE_REGION_SPIM2, cfg->spim2);
	ft9001_cache_region_mode_set(inst, FT9001_CACHE_REGION_SPIM3, cfg->spim3);
}

void ft9001_cache_invalidate_all(CACHE_TypeDef *inst)
{
	cache_start_cmd(inst, CACHE_CCR_INVW1 | CACHE_CCR_INVW0);
}

void ft9001_cache_invalidate_range(CACHE_TypeDef *inst, uint32_t addr, uint32_t size)
{
	uint32_t base;
	uint32_t tail;
	uint32_t len;

	if (!cache_is_enabled(inst)) {
		return;
	}

	base = addr & ~(CACHE_LINE_SIZE - 1U);
	tail = (addr - base) + size;
	len = (tail + (CACHE_LINE_SIZE - 1U)) & ~(CACHE_LINE_SIZE - 1U);

	FT9001_WRITE_REG(inst->CACHE_CPEA, base);
	FT9001_WRITE_REG(inst->CACHE_CPES, len | CACHE_CPES_START_INVAL);

	while (FT9001_READ_BIT(inst->CACHE_CPES, CACHE_CPES_START_INVAL)) {
	}
}

void ft9001_cache_init(CACHE_TypeDef *inst, const struct ft9001_cache_config *cfg)
{
	ft9001_cache_disable(inst);
	ft9001_cache_regions_configure(inst, cfg);
	ft9001_cache_invalidate_all(inst);
	ft9001_cache_enable(inst);
}
