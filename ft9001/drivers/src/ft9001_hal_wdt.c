/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ft9001_hal_wdt.h"

#define WDT_FEED_KEY1   (0x5555U)
#define WDT_FEED_KEY2   (0xAAAAU)

void HAL_WDT_Refresh(WDT_TypeDef *inst)
{
	/* Two-step feed sequence: keys must be written consecutively to WSR. */
	inst->WSR = (uint16_t)WDT_FEED_KEY1;
	inst->WSR = (uint16_t)WDT_FEED_KEY2;
}

HAL_StatusTypeDef HAL_WDT_Enable(WDT_TypeDef *inst)
{
	uint16_t val = inst->WCR;

	val |= (uint16_t)WDT_WCR_EN;
	inst->WCR = val;

	return HAL_OK;
}

HAL_StatusTypeDef HAL_WDT_Disable(WDT_TypeDef *inst)
{
	uint16_t val = inst->WCR;

	val &= (uint16_t)~WDT_WCR_EN_Msk;
	inst->WCR = val;

	return HAL_OK;
}

HAL_StatusTypeDef HAL_WDT_ConfigMode(WDT_TypeDef *inst, HAL_WDT_ModeTypeDef mode)
{
	uint16_t val = inst->WCR;

	/* Replace the three mode bits atomically (DBG/DOZE/WAIT). */
	val &= (uint16_t)~WDT_WCR_MODE_Msk;
	val |= (uint16_t)(mode & WDT_WCR_MODE_Msk);
	inst->WCR = val;

	return HAL_OK;
}

HAL_StatusTypeDef HAL_WDT_SetReload(WDT_TypeDef *inst, uint16_t reload)
{
	/* A write to WMR immediately loads the counter and sets the reload value. */
	inst->WMR = reload;

	return HAL_OK;
}

uint16_t HAL_WDT_GetCounter(WDT_TypeDef *inst)
{
    /* Single 16-bit read; split high/low reads are not guaranteed coherent. */
	return inst->WCNTR;
}
