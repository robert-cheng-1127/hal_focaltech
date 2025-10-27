/*
 * Copyright (c) 2025-2026, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file    ft9001_wdt.h
 * @brief   FT9001 watchdog timer register operations.
 *
 * Register semantics:
 * - WCR   : control. EN enables the watchdog; DBG/DOZE/WAIT define halt behaviour.
 * - WMR   : reload. A write loads the counter immediately and sets the value used
 *           by subsequent refresh operations.
 * - WCNTR : counter, read-only.
 * - WSR   : service. Two successive 16-bit keys refresh the watchdog.
 */

#ifndef FT9001_WDT_H_
#define FT9001_WDT_H_

#include <stdint.h>

#include "ft9001.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief No halt condition. */
#define FT9001_WDT_MODE_NONE       ((uint16_t)0U)
/** @brief Halt the watchdog while the CPU is debug-halted. */
#define FT9001_WDT_MODE_DEBUG_STOP ((uint16_t)WDT_WCR_DBG_Msk)
/** @brief Halt the watchdog in DOZE mode. */
#define FT9001_WDT_MODE_DOZE_STOP  ((uint16_t)WDT_WCR_DOZE_Msk)
/** @brief Halt the watchdog in WAIT mode. */
#define FT9001_WDT_MODE_WAIT_STOP  ((uint16_t)WDT_WCR_WAIT_Msk)

/** @brief First key of the two-write service sequence. */
#define FT9001_WDT_FEED_KEY1 ((uint16_t)0x5555U)
/** @brief Second key of the two-write service sequence. */
#define FT9001_WDT_FEED_KEY2 ((uint16_t)0xAAAAU)

/**
 * @brief Refresh (feed) the watchdog.
 *
 * The two keys must reach WSR back to back. Serialise against any other context
 * that also refreshes, otherwise an interleaved write voids the sequence.
 */
static inline void ft9001_wdt_refresh(WDT_TypeDef *inst)
{
	inst->WSR = FT9001_WDT_FEED_KEY1;
	inst->WSR = FT9001_WDT_FEED_KEY2;
}

/** @brief Enable the watchdog (WCR.EN). Program the reload first. */
static inline void ft9001_wdt_enable(WDT_TypeDef *inst)
{
	FT9001_SET_BIT(inst->WCR, (uint16_t)WDT_WCR_EN);
}

/** @brief Disable the watchdog (WCR.EN). */
static inline void ft9001_wdt_disable(WDT_TypeDef *inst)
{
	FT9001_CLEAR_BIT(inst->WCR, (uint16_t)WDT_WCR_EN_Msk);
}

/**
 * @brief Set the halt behaviour in DBG/DOZE/WAIT modes.
 *
 * Replaces all three bits, so pass the complete desired mask; previously set
 * bits are not preserved.
 *
 * @param mode OR-combination of FT9001_WDT_MODE_* flags.
 */
static inline void ft9001_wdt_mode_set(WDT_TypeDef *inst, uint16_t mode)
{
	FT9001_MODIFY_REG(inst->WCR, (uint16_t)WDT_WCR_MODE_Msk,
			  (uint16_t)(mode & (uint16_t)WDT_WCR_MODE_Msk));
}

/**
 * @brief Set the reload value.
 *
 * The write reloads the counter immediately, so calling this outside an
 * initialisation path can shorten the remaining time unexpectedly.
 */
static inline void ft9001_wdt_reload_set(WDT_TypeDef *inst, uint16_t reload)
{
	inst->WMR = reload;
}

/** @brief Read the counter (WCNTR). */
static inline uint16_t ft9001_wdt_counter_get(WDT_TypeDef *inst)
{
	return inst->WCNTR;
}

#ifdef __cplusplus
}
#endif

#endif /* FT9001_WDT_H_ */
