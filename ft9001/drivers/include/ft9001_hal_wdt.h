/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file    ft9001_hal_wdt.h
 * @brief   FT9001 Watchdog Timer (WDT) HAL.
 *
 * Thin, register-level interface for the watchdog timer: enable/disable,
 * mode configuration, reload programming, counter read, and periodic refresh.
 * The service key sequence is handled inside the HAL and not exposed to users.
 *
 * Register semantics:
 * - WCR   : Control register. EN enables WDT; DBG/DOZE/WAIT define halt behavior.
 * - WMR   : Reload register. A write loads the counter immediately and sets the
 *           value used on subsequent refresh operations.
 * - WCNTR : Counter register (read-only). Returns the current 16-bit value.
 * - WSR   : Service register. Requires two successive 16-bit keys to refresh;
 *           the sequence is performed by the HAL.
 */

#ifndef FT9001_HAL_WDT_H_
#define FT9001_HAL_WDT_H_

#include <stdint.h>
#include <stdbool.h>
#include "ft9001.h"
#include "ft9001_hal_def.h"

/**
 * @defgroup FT9001_HAL_WDT Watchdog Timer HAL
 * @brief    Public API for FT9001 watchdog timer.
 * @{
 */

/** @brief WDT mode bit-mask (maps to WCR DBG/DOZE/WAIT). */
typedef uint32_t HAL_WDT_ModeTypeDef;

/** @brief No halt condition. */
#define HAL_WDT_MODE_NONE        (0U)
/** @brief Halt WDT when CPU is debug‑halted. */
#define HAL_WDT_MODE_DEBUG_STOP  ((HAL_WDT_ModeTypeDef)WDT_WCR_DBG_Msk)
/** @brief Halt WDT in DOZE mode. */
#define HAL_WDT_MODE_DOZE_STOP   ((HAL_WDT_ModeTypeDef)WDT_WCR_DOZE_Msk)
/** @brief Halt WDT in WAIT mode. */
#define HAL_WDT_MODE_WAIT_STOP   ((HAL_WDT_ModeTypeDef)WDT_WCR_WAIT_Msk)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Refresh (feed) the watchdog to prevent a system reset.
 *
 * This function performs the hardware‑mandated two‑key service sequence by
 * writing the keys back‑to‑back to WSR. The key values and exact sequence
 * are kept private to the HAL implementation.
 *
 * @note This routine performs two 16‑bit writes with no intervening writes.
 *       If an ISR might also refresh the watchdog, ensure proper serialization
 *       (e.g., disable interrupts around the call or centralize refresh).
 */
void HAL_WDT_Refresh(WDT_TypeDef *inst);

/**
 * @brief Enable the watchdog (sets WCR.EN).
 *
 * @return HAL_OK on success.
 *
 * @note This call does not configure timeout. Call HAL_WDT_SetReload()
 *       beforehand to program a suitable reload value for your system.
 */
HAL_StatusTypeDef HAL_WDT_Enable(WDT_TypeDef *inst);

/**
 * @brief Disable the watchdog (clears WCR.EN).
 *
 * @return HAL_OK on success.
 *
 * @warning Some systems may restrict disabling WDT in production (e.g., fuses/
 *          life‑cycle). This HAL does not enforce such policies; use with care.
 */
HAL_StatusTypeDef HAL_WDT_Disable(WDT_TypeDef *inst);

/**
 * @brief Configure WDT halt behavior in DBG/DOZE/WAIT modes.
 *
 * This routine **replaces** the three mode bits atomically:
 * it clears (DBG/DOZE/WAIT) and then sets the provided mask.
 *
 * @param mode OR‑combination of HAL_WDT_MODE_* flags.
 * @return HAL_OK on success.
 *
 * @note To emulate legacy “OR‑accumulate” behavior, pass the final desired mask
 *       each time; this API does not preserve previously set bits.
 */
HAL_StatusTypeDef HAL_WDT_ConfigMode(WDT_TypeDef *inst, HAL_WDT_ModeTypeDef mode);

/**
 * @brief Program the watchdog reload value.
 *
 * Writing WMR immediately loads the counter to @p reload and also sets the
 * subsequent refresh reload value used by HAL_WDT_Refresh().
 *
 * @param reload 16‑bit reload value.
 * @return HAL_OK on success.
 *
 * @note Because the counter is reloaded immediately, consider calling this
 *       routine in an initialization path or within a protected critical section
 *       to avoid shortening the remaining time unexpectedly.
 */
HAL_StatusTypeDef HAL_WDT_SetReload(WDT_TypeDef *inst, uint16_t reload);

/**
 * @brief Read the current 16‑bit counter value (WCNTR).
 *
 * @return Current counter value.
 *
 * @note The counter is read atomically as a single 16‑bit access; splitting
 *       into high/low reads is not guaranteed coherent.
 */
uint16_t HAL_WDT_GetCounter(WDT_TypeDef *inst);

#ifdef __cplusplus
}
#endif

#endif /* FT9001_HAL_WDT_H_ */
