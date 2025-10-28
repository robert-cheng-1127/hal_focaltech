/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ft9001_hal.h"
#include "system_ft9001.h"

uint32_t SystemCoreClock = 160000000U;

void SystemInit(void)
{
	/* Disable Watchdog */
	HAL_WDT_Disable(WDT);

	/* Default TC to one-shot. Does NOT start the counter. */
	HAL_TC_ConfigMode(TC, HAL_TC_MODE_ONE_SHOT);

	/* Configure system clock */
	HAL_CPM_SetHighSpeedOscTrim(CPM_OSC_FREQ_320MHZ);
	HAL_CPM_SetSysClock(CPM_SYSCLK_OSC400M, 5U);
	HAL_CPM_SetIpsClkDiv(1U);
}

void SystemCoreClockUpdate(void)
{
	SystemCoreClock = HAL_CPM_GetSysClockFreqHz();
}
