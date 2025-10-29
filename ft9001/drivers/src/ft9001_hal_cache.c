/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ft9001_hal_cache.h"

#define CACHE_LINE_SIZE		(16U) /* 16-byte lines */

static inline bool CACHE_IsEnabled(CACHE_TypeDef *inst)
{
	return (READ_BIT(inst->CACHE_CCR, CACHE_CCR_ENCACHE) != 0U);
}

static inline void CACHE_WaitGoClear(CACHE_TypeDef *inst)
{
	while (READ_BIT(inst->CACHE_CCR, CACHE_CCR_GO)) {
		/* busy-wait */
	}
}

static inline void CACHE_StartCmd(CACHE_TypeDef *inst, uint32_t ccr_bits)
{
	/* Set command bits + GO, wait done */
	SET_BIT(inst->CACHE_CCR, ccr_bits | CACHE_CCR_GO);
	CACHE_WaitGoClear(inst);
}

static void CACHE_ApplyMode(uint32_t *reg,
                            uint32_t cacheable_mask,
                            uint32_t wt_wb_mask,
                            HAL_CACHE_Mode mode)
{
	switch (mode) {
		case HAL_CACHE_MODE_OFF:
			CLEAR_BIT(*reg, cacheable_mask | wt_wb_mask);
			break;
		case HAL_CACHE_MODE_WRITE_THROUGH:
			SET_BIT(*reg, cacheable_mask);
			CLEAR_BIT(*reg, wt_wb_mask);
			break;
		case HAL_CACHE_MODE_WRITE_BACK:
		default:
			SET_BIT(*reg, cacheable_mask | wt_wb_mask);
			break;
	}
}

void HAL_CACHE_Enable(CACHE_TypeDef *inst, bool enable)
{
	if (enable) SET_BIT(inst->CACHE_CCR, CACHE_CCR_ENCACHE);
	else        CLEAR_BIT(inst->CACHE_CCR, CACHE_CCR_ENCACHE);
}

void HAL_CACHE_SetRegionMode(CACHE_TypeDef *inst,
                             HAL_CACHE_Region region,
                             HAL_CACHE_Mode mode)
{
	switch (region) {
		case HAL_CACHE_REGION_BOOT: {
			/* CSACR: ROMR_x pair (CACHEABLE/WT_WB) */
			uint32_t v = READ_REG(inst->CACHE_CSACR);

			CACHE_ApplyMode(&v,
				(CACHE_CSACR_ROMR_3_CACHEABLE | CACHE_CSACR_ROMR_2_CACHEABLE |
					CACHE_CSACR_ROMR_1_CACHEABLE | CACHE_CSACR_ROMR_0_CACHEABLE),
				(CACHE_CSACR_ROMR_3_WT_WB     | CACHE_CSACR_ROMR_2_WT_WB     |
					CACHE_CSACR_ROMR_1_WT_WB     | CACHE_CSACR_ROMR_0_WT_WB),
				mode);
			WRITE_REG(inst->CACHE_CSACR, v);
			break;
		}

		case HAL_CACHE_REGION_ROM: {
			/* CACR: ROM_* pair */
			uint32_t v = READ_REG(inst->CACHE_CACR);

			switch (mode) {
				case HAL_CACHE_MODE_OFF:
					CLEAR_BIT(v, CACHE_CACR_ROM_CACHEABLE | CACHE_CACR_ROM_WT_WB);
					break;
				case HAL_CACHE_MODE_WRITE_THROUGH:
					SET_BIT(v, CACHE_CACR_ROM_CACHEABLE);
					CLEAR_BIT(v, CACHE_CACR_ROM_WT_WB);
					break;
				case HAL_CACHE_MODE_WRITE_BACK:
				default:
					SET_BIT(v, CACHE_CACR_ROM_CACHEABLE | CACHE_CACR_ROM_WT_WB);
					break;
			}
			WRITE_REG(inst->CACHE_CACR, v);
			break;
		}

		case HAL_CACHE_REGION_SPIM1: {
			/* CSACR: SPI1_x pair */
			uint32_t v = READ_REG(inst->CACHE_CSACR);

			CACHE_ApplyMode(&v,
				(CACHE_CSACR_SPI1_3_CACHEABLE | CACHE_CSACR_SPI1_2_CACHEABLE |
				CACHE_CSACR_SPI1_1_CACHEABLE  | CACHE_CSACR_SPI1_0_CACHEABLE),
				(CACHE_CSACR_SPI1_3_WT_WB     | CACHE_CSACR_SPI1_2_WT_WB     |
				CACHE_CSACR_SPI1_1_WT_WB      | CACHE_CSACR_SPI1_0_WT_WB),
				mode);
			WRITE_REG(inst->CACHE_CSACR, v);
			break;
		}

		case HAL_CACHE_REGION_SPIM2: {
			/* CSACR: SPI2_x pair */
			uint32_t v = READ_REG(inst->CACHE_CSACR);
			CACHE_ApplyMode(&v,
				(CACHE_CSACR_SPI2_3_CACHEABLE | CACHE_CSACR_SPI2_2_CACHEABLE |
				CACHE_CSACR_SPI2_1_CACHEABLE  | CACHE_CSACR_SPI2_0_CACHEABLE),
				(CACHE_CSACR_SPI2_3_WT_WB     | CACHE_CSACR_SPI2_2_WT_WB     |
				CACHE_CSACR_SPI2_1_WT_WB      | CACHE_CSACR_SPI2_0_WT_WB),
				mode);
			WRITE_REG(inst->CACHE_CSACR, v);
			break;
		}

		case HAL_CACHE_REGION_SPIM3: {
			/* CSACR: SPI3_x pair */
			uint32_t v = READ_REG(inst->CACHE_CSACR);
			CACHE_ApplyMode(&v,
				(CACHE_CSACR_SPI3_3_CACHEABLE | CACHE_CSACR_SPI3_2_CACHEABLE |
				CACHE_CSACR_SPI3_1_CACHEABLE  | CACHE_CSACR_SPI3_0_CACHEABLE),
				(CACHE_CSACR_SPI3_3_WT_WB     | CACHE_CSACR_SPI3_2_WT_WB     |
				CACHE_CSACR_SPI3_1_WT_WB      | CACHE_CSACR_SPI3_0_WT_WB),
				mode);
			WRITE_REG(inst->CACHE_CSACR, v);
			break;
		}

		default:
			/* no-op */
			break;
    }
}

void HAL_CACHE_ConfigRegions(CACHE_TypeDef *inst,
                             HAL_CACHE_Mode boot,
                             HAL_CACHE_Mode rom,
                             HAL_CACHE_Mode spim1,
                             HAL_CACHE_Mode spim2,
                             HAL_CACHE_Mode spim3)
{
    HAL_CACHE_SetRegionMode(inst, HAL_CACHE_REGION_BOOT,  boot);
    HAL_CACHE_SetRegionMode(inst, HAL_CACHE_REGION_ROM,   rom);
    HAL_CACHE_SetRegionMode(inst, HAL_CACHE_REGION_SPIM1, spim1);
    HAL_CACHE_SetRegionMode(inst, HAL_CACHE_REGION_SPIM2, spim2);
    HAL_CACHE_SetRegionMode(inst, HAL_CACHE_REGION_SPIM3, spim3);
}

void HAL_CACHE_InvalidateAll(CACHE_TypeDef *inst)
{
    /* INVW1|INVW0 + GO, wait GO clears. */
    CACHE_StartCmd(inst, CACHE_CCR_INVW1 | CACHE_CCR_INVW0);
}

void HAL_CACHE_InvalidateRange(CACHE_TypeDef *inst, uint32_t addr, uint32_t size)
{
    if (!CACHE_IsEnabled(inst)) return;

    /* Align start down and size up to 16B lines (legacy driver behavior). */
    uint32_t base = addr & ~(CACHE_LINE_SIZE - 1U);
    uint32_t tail = (addr - base) + size;
    uint32_t len  = (tail + (CACHE_LINE_SIZE - 1U)) & ~(CACHE_LINE_SIZE - 1U);

    WRITE_REG(inst->CACHE_CPEA, base);
    WRITE_REG(inst->CACHE_CPES, len | CACHE_CPES_START_INVAL);

	/* Wait for CPES.START_INVAL to clear. */
    while (READ_BIT(inst->CACHE_CPES, CACHE_CPES_START_INVAL)) {
		/* busy-wait */
	}
}

void HAL_CACHE_Init(CACHE_TypeDef *inst,
                    HAL_CACHE_Mode boot,
                    HAL_CACHE_Mode rom,
                    HAL_CACHE_Mode spim1,
                    HAL_CACHE_Mode spim2,
                    HAL_CACHE_Mode spim3)
{
    HAL_CACHE_Enable(inst, false);
    HAL_CACHE_ConfigRegions(inst, boot, rom, spim1, spim2, spim3);
    HAL_CACHE_InvalidateAll(inst);
    HAL_CACHE_Enable(inst, true);
}
