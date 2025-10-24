/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file    ft9001_hal_cpm.h
 * @brief   FT9001 HAL – Clock & Power Management (CPM) public API.
 *
 * This header exposes a minimal, opinionated HAL for configuring the FT9001
 * system clock tree. The API covers:
 *   - Selecting the system clock source,
 *   - Programming the high‑speed oscillator (HSOSC) trim from OTP,
 *   - Configuring IPS bus clock division,
 *   - Querying current system clock source and effective core frequency.
 *
 * @attention The APIs in this header are not re‑entrant and are intended to be
 *            called during early system initialization or from a context that
 *            guarantees exclusive access to CPM registers.
 */

#ifndef FT9001_HAL_CPM_H_
#define FT9001_HAL_CPM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ft9001_hal_def.h"

/**
 * @addtogroup FT9001_HAL_Driver
 * @{
 */

/**
 * @defgroup HAL_CPM Clock & Power Management
 * @brief    HAL layer for FT9001 Clock & Power Management.
 * @{
 */

/**
 * @brief System clock source encoding (maps to CSWCFGR.SYS_SEL[1:0]).
 *
 * - @ref CPM_SYSCLK_OSC8M  : Internal 8 MHz RC oscillator (LSI).
 * - @ref CPM_SYSCLK_OSC400M: High-speed oscillator domain (HSOSC). The actual
 *                            nominal frequency is determined by the trim value
 *                            previously programmed to O400MTRIMR (typically
 *                            320 MHz or 400 MHz via @ref HAL_CPM_SetHighSpeedOscTrim).
 */
typedef enum {
	CPM_SYSCLK_OSC8M   = 0U,  /*!< Internal 8 MHz RC oscillator */
	CPM_SYSCLK_OSC400M = 1U   /*!< High-speed oscillator (HSOSC; 320/400 MHz after trim) */
} CPM_SysClkSourceTypeDef;

/**
 * @brief High‑speed oscillator nominal frequencies supported by OTP trims.
 *
 * This value selects which OTP trim blob to load into O400MTRIMR when calling
 * @ref HAL_CPM_SetHighSpeedOscTrim. It also updates the HAL’s internal notion
 * of the HSOSC nominal frequency used by @ref HAL_CPM_GetSysClockFreqHz.
 */
typedef enum {
	CPM_OSC_FREQ_320MHZ = 0U, /*!< HSOSC trimmed to 320 MHz nominal */
	CPM_OSC_FREQ_400MHZ = 1U  /*!< HSOSC trimmed to 400 MHz nominal */
} CPM_OscFreqTypeDef;

/**
 * @brief Program HSOSC trim from OTP into O400MTRIMR.
 *
 * This routine reads the appropriate OTP location and writes the trim value to
 * O400MTRIMR using the VCCCTMR unlock/override sequence. For safety, it first
 * switches the system clock to OSC8M before writing the trim register, then
 * restores only the trim (it does not change the current SYSCLK source).
 *
 * @note  This function does **not** switch the system clock to HSOSC. To run
 *        the system from HSOSC after programming the trim, call
 *        @ref HAL_CPM_SetSysClock with @ref CPM_SYSCLK_OSC400M.
 * @note  On success this function updates the internal nominal HSOSC frequency
 *        used by @ref HAL_CPM_GetSysClockFreqHz.
 *
 * @param  freq        Requested nominal HSOSC frequency trim (320 or 400 MHz).
 * @retval HAL_OK      Trim was found in OTP and written to O400MTRIMR.
 * @retval HAL_ERROR   No valid trim blob found or an internal check failed.
 */
HAL_StatusTypeDef HAL_CPM_SetHighSpeedOscTrim(CPM_OscFreqTypeDef freq);

/**
 * @brief Switch the system clock source and wait until the switch completes.
 *
 * This routine enables the selected clock source (if required), waits for its
 * stability flag, programs CSWCFGR.SYS_SEL and waits for the corresponding
 * SYS_SEL_ST status to assert.
 *
 * @param  source      Target system clock source
 *                     (OSC8M or HSOSC via @ref CPM_SysClkSourceTypeDef).
 * @param  timeout_ms  Maximum time to wait for stability and switch complete.
 *                     Pass 0 to wait “forever”.
 * @retval HAL_OK      Source became stable and system clock switch completed.
 * @retval HAL_TIMEOUT Timeout while waiting for source stable or switch status.
 * @retval HAL_ERROR   Unsupported source or internal parameter check failed.
 *
 * @note   Selecting @ref CPM_SYSCLK_OSC400M assumes that a valid HSOSC trim
 *         has already been programmed (see @ref HAL_CPM_SetHighSpeedOscTrim).
 */
HAL_StatusTypeDef HAL_CPM_SetSysClock(CPM_SysClkSourceTypeDef source, uint32_t timeout_ms);

/**
 * @brief Configure the IPS bus divider (PCDIVR1.IPS_DIV) and update.
 *
 * Programs PCDIVR1.IPS_DIV with the provided value and asserts
 * CDIVUPDR.PERDIV_UPD to commit the change.
 *
 * @param  div         Raw divider field (0..15). The effective divide factor
 *                     is (div + 1), as defined by the hardware.
 * @retval HAL_OK      Divider programmed and update triggered.
 * @retval HAL_ERROR   Divider value out of range.
 */
HAL_StatusTypeDef HAL_CPM_SetIpsClkDiv(uint32_t div);

/**
 * @brief  Compute the current core clock (HCLK) in Hz.
 *
 * The routine determines the base clock from the configured SYSCLK source:
 *  - OSC8M  → 8,000,000 Hz
 *  - HSOSC  → the most recent nominal HSOSC frequency set by
 *             @ref HAL_CPM_SetHighSpeedOscTrim (e.g. 320/400 MHz)
 *
 * Then it reads SCDIVR.SYS_DIV and returns:
 *     HCLK = base / (SYS_DIV + 1).
 *
 * @return uint32_t  Estimated core clock frequency in Hz.
 *
 * @note This is a best‑effort software computation; it assumes the OTP trim
 *       and SCDIVR settings match the current hardware state. It does not
 *       account for external clock sources.
 */
uint32_t HAL_CPM_GetSysClockFreqHz(void);

/**
 * @}
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* FT9001_HAL_CPM_H_ */
