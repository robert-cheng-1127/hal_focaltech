/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file    ft9001_hal_tc.h
 * @brief   FT9001 Timer/Counter (TC) HAL.
 *
 * Thin, register-level interface for the FT9001 TC block.
 *
 * Semantics:
 *  - TCMR holds the reload (modulus) value.
 *  - Writing TCCR.CU = 1 requests an immediate load of TCMR into TCCNTR;
 *    CU self-clears after the update takes effect.
 *  - RN selects the operating mode:
 *      RN = 1 → periodic (auto-reload at zero)
 *      RN = 0 → one-shot (no reload at zero)
 *  - IF is an interrupt flag with W1C semantics (write '1' to clear).
 *  - WDP[2:0] selects the prescaler (000..111 ⇒ DIV2048..DIV16).
 */

#ifndef FT9001_HAL_TC_H_
#define FT9001_HAL_TC_H_

#include <stdint.h>
#include <stdbool.h>
#include "ft9001.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief TC prescaler selection mapped to WDP[2:0].
 *
 * Encoding (000..111):
 *  - HAL_TC_WDP_2048 .. HAL_TC_WDP_16 corresponds to DIV2048 .. DIV16.
 */
typedef enum {
    HAL_TC_WDP_2048 = 0x00, /**< Divide by 2048 (WDP=000). */
    HAL_TC_WDP_1024 = 0x01, /**< Divide by 1024 (WDP=001). */
    HAL_TC_WDP_512  = 0x02, /**< Divide by 512  (WDP=010). */
    HAL_TC_WDP_256  = 0x03, /**< Divide by 256  (WDP=011). */
    HAL_TC_WDP_128  = 0x04, /**< Divide by 128  (WDP=100). */
    HAL_TC_WDP_64   = 0x05, /**< Divide by 64   (WDP=101). */
    HAL_TC_WDP_32   = 0x06, /**< Divide by 32   (WDP=110). */
    HAL_TC_WDP_16   = 0x07  /**< Divide by 16   (WDP=111). */
} HAL_TC_Prescaler;

/**
 * @brief TC operating mode (encapsulates RN bit).
 */
typedef enum {
    HAL_TC_MODE_PERIODIC = 0, /**< RN=1: reload at zero (periodic). */
    HAL_TC_MODE_ONE_SHOT = 1  /**< RN=0: one-shot (no reload at zero). */
} HAL_TC_Mode;

/**
 * @brief Start the timer.
 *
 * Clears relevant halt controls (e.g., DBG/DOZE/WAIT) and the STOP bit,
 * allowing the counter to run.
 *
 * @param inst  TC instance (base address).
 */
void HAL_TC_Start(TC_TypeDef *inst);

/**
 * @brief Stop the timer.
 *
 * Sets STOP bit. Other mode bits are not altered.
 *
 * @param inst  TC instance (base address).
 */
void HAL_TC_Stop(TC_TypeDef *inst);

/**
 * @brief Read the current counter value.
 *
 * @param inst  TC instance (base address).
 * @return 16-bit counter value (width matches hardware register).
 */
uint16_t HAL_TC_GetCounter(TC_TypeDef *inst);

/**
 * @brief Read the current reload (modulus) value.
 *
 * @param inst  TC instance (base address).
 * @return 16-bit reload value from TCMR.
 */
uint16_t HAL_TC_GetReload(TC_TypeDef *inst);

/**
 * @brief Program the reload (modulus) value and apply immediately.
 *
 * Writes TCMR then sets the self-clearing CU bit to load the value to TCCNTR
 * without waiting for a wrap.
 *
 * @param inst    TC instance (base address).
 * @param reload  Reload value to write to TCMR.
 */
void HAL_TC_SetReload(TC_TypeDef *inst, uint16_t reload);

/**
 * @brief Configure the prescaler (WDP[2:0]).
 *
 * @param inst  TC instance (base address).
 * @param psc   Prescaler selection.
 */
void HAL_TC_SetPrescaler(TC_TypeDef *inst, HAL_TC_Prescaler psc);

/**
 * @brief Configure operating mode.
 *
 * @param inst  TC instance (base address).
 * @param mode  HAL_TC_MODE_PERIODIC (RN=1) or HAL_TC_MODE_ONE_SHOT (RN=0).
 */
void HAL_TC_ConfigMode(TC_TypeDef *inst, HAL_TC_Mode mode);

/**
 * @brief Configure DBG/DOZE/WAIT bits with replace semantics.
 *
 * This function clears the three bits and then sets them according to arguments,
 * avoiding OR-accumulation of stale state.
 *
 * @param inst  TC instance (base address).
 * @param dbg   true to halt in debug halt (sets DBG), false otherwise.
 * @param doze  true to halt in DOZE mode (sets DOZE), false otherwise.
 * @param wait  true to halt in WAIT mode (sets WAIT), false otherwise.
 */
void HAL_TC_ConfigHaltBits(TC_TypeDef *inst, bool dbg, bool doze, bool wait);

/**
 * @brief Enable/disable update interrupt.
 *
 * @param inst    TC instance (base address).
 * @param enable  true to set IE, false to clear IE.
 */
void HAL_TC_EnableIT_Update(TC_TypeDef *inst, bool enable);

/**
 * @brief Read update interrupt flag.
 *
 * @param inst  TC instance (base address).
 * @return true if IF is set, false otherwise.
 */
bool HAL_TC_GetFlag_Update(TC_TypeDef *inst);

/**
 * @brief Clear update interrupt flag (W1C).
 *
 * Writes '1' to IF. Other bits remain unchanged.
 *
 * @param inst  TC instance (base address).
 */
void HAL_TC_ClearFlag_Update(TC_TypeDef *inst);

#ifdef __cplusplus
}
#endif

#endif /* FT9001_HAL_TC_H_ */
