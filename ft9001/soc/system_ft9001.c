/*
 * Copyright (c) 2025-2026, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ft9001_hal.h"
#include "system_ft9001.h"

/* Budget for the OSC400M switch. Each poll is a single register read issued at
 * OSC8M, so this bounds the wait without depending on a wall-clock estimate.
 */
#define SYSCLK_SWITCH_POLLS (100000UL)

uint32_t SystemCoreClock = 160000000U;

void SystemInit(void)
{
	ft9001_wdt_disable(WDT);

	(void)ft9001_cpm_hsosc_trim_set(FT9001_CPM_OSC_FREQ_320MHZ);
	(void)ft9001_cpm_sysclk_source_set(FT9001_CPM_SYSCLK_OSC400M, SYSCLK_SWITCH_POLLS);
	(void)ft9001_cpm_ips_div_set(1U);
}

void SystemCoreClockUpdate(void)
{
	SystemCoreClock = ft9001_cpm_sysclk_freq_hz_get();
}
