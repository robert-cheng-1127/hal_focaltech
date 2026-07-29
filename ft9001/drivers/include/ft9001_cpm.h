/*
 * Copyright (c) 2025-2026, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file    ft9001_cpm.h
 * @brief   FT9001 clock and power management.
 *
 * Covers system clock source selection, the high-speed oscillator trim held in
 * OTP, the IPS bus divider and the resulting core frequency.
 *
 * None of these routines are re-entrant. Call them during early initialisation
 * or from a context that has exclusive access to the CPM registers.
 */

#ifndef FT9001_CPM_H_
#define FT9001_CPM_H_

#include <stdint.h>

#include "ft9001.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Poll indefinitely instead of giving up after a bounded number of reads.
 */
#define FT9001_CPM_POLL_FOREVER 0U

/** @brief System clock source, encoded into CSWCFGR.SYS_SEL. */
enum ft9001_cpm_sysclk_source {
	/** Internal 8 MHz RC oscillator. */
	FT9001_CPM_SYSCLK_OSC8M = 0U,
	/** High-speed oscillator; 320 or 400 MHz depending on the programmed trim. */
	FT9001_CPM_SYSCLK_OSC400M = 1U,
};

/** @brief High-speed oscillator nominal frequency, selecting which OTP trim to load. */
enum ft9001_cpm_osc_freq {
	FT9001_CPM_OSC_FREQ_320MHZ = 0U,
	FT9001_CPM_OSC_FREQ_400MHZ = 1U,
};

/**
 * @brief Load the high-speed oscillator trim from OTP into O400MTRIMR.
 *
 * Switches the system clock to OSC8M before writing the protected trim register
 * and leaves it there; run @ref ft9001_cpm_sysclk_source_set afterwards to move
 * back onto the high-speed oscillator. On success the nominal frequency used by
 * @ref ft9001_cpm_sysclk_freq_hz_get is updated to match.
 *
 * @param  freq    Requested nominal frequency.
 * @retval 0       Trim found in OTP and written.
 * @retval -EINVAL Unsupported frequency.
 * @retval -ENOENT No valid trim blob in OTP.
 * @retval -ETIMEDOUT Could not switch to OSC8M.
 */
int ft9001_cpm_hsosc_trim_set(enum ft9001_cpm_osc_freq freq);

/**
 * @brief Switch the system clock source and wait for the switch to complete.
 *
 * Enables the requested source, waits for its stability flag, programs
 * CSWCFGR.SYS_SEL and waits for the matching SYS_SEL_ST status.
 *
 * Selecting @ref FT9001_CPM_SYSCLK_OSC400M assumes a valid trim has already been
 * programmed by @ref ft9001_cpm_hsosc_trim_set.
 *
 * @param  source      Target clock source.
 * @param  poll_budget Number of register reads to attempt before giving up, or
 *                     @ref FT9001_CPM_POLL_FOREVER. This is a retry count and
 *                     not a duration: the routine runs while the system clock is
 *                     being reconfigured, so the wall-clock time per read varies
 *                     with the current frequency and with the optimisation level
 *                     the HAL was built at.
 * @retval 0           Source stable and switch complete.
 * @retval -EINVAL     Unsupported source.
 * @retval -ETIMEDOUT  Budget exhausted waiting for stability or for the switch.
 */
int ft9001_cpm_sysclk_source_set(enum ft9001_cpm_sysclk_source source, uint32_t poll_budget);

/** @brief Read back the active system clock source (CSWCFGR.SYS_SEL). */
enum ft9001_cpm_sysclk_source ft9001_cpm_sysclk_source_get(void);

/**
 * @brief Set the IPS bus divider (PCDIVR1.IPS_DIV) and commit it.
 *
 * @param  div     Raw 4-bit field; the effective divide factor is (div + 1).
 * @retval 0       Divider programmed and update triggered.
 * @retval -EINVAL Divider out of range.
 */
int ft9001_cpm_ips_div_set(uint32_t div);

/**
 * @brief Compute the core clock (HCLK) in Hz.
 *
 * Takes the base frequency from the active source (8 MHz for OSC8M, otherwise
 * the nominal frequency recorded by the last successful
 * @ref ft9001_cpm_hsosc_trim_set) and divides it by (SCDIVR.SYS_DIV + 1).
 *
 * This is a software estimate: it assumes the OTP trim and SCDIVR reflect the
 * current hardware state, and it does not account for external clock sources.
 */
uint32_t ft9001_cpm_sysclk_freq_hz_get(void);

/**
 * @brief Compute the IPS bus clock in Hz.
 *
 * The SCI baud rate generator and the timers run from this, not from the core
 * clock. Shares the software-estimate caveat of @ref ft9001_cpm_sysclk_freq_hz_get.
 */
uint32_t ft9001_cpm_ips_freq_hz_get(void);

#ifdef __cplusplus
}
#endif

#endif /* FT9001_CPM_H_ */
