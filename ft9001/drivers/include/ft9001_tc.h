/*
 * Copyright (c) 2025-2026, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file    ft9001_tc.h
 * @brief   FT9001 timer/counter register operations.
 *
 * Register semantics:
 * - TCMR holds the reload (modulus) value.
 * - TCCR.CU requests an immediate load of TCMR into TCCNTR and self-clears once
 *   the update takes effect.
 * - TCCR.RN selects periodic (1, auto-reload at zero) or one-shot (0).
 * - TCCR.IF is write-one-to-clear.
 * - TCCR.WDP[2:0] selects the prescaler, 000..111 mapping to DIV2048..DIV16.
 */

#ifndef FT9001_TC_H_
#define FT9001_TC_H_

#include <stdbool.h>
#include <stdint.h>

#include "ft9001.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Prescaler selection, encoded into TCCR.WDP[2:0]. */
enum ft9001_tc_prescaler {
	FT9001_TC_PRESCALER_DIV2048 = 0x00,
	FT9001_TC_PRESCALER_DIV1024 = 0x01,
	FT9001_TC_PRESCALER_DIV512  = 0x02,
	FT9001_TC_PRESCALER_DIV256  = 0x03,
	FT9001_TC_PRESCALER_DIV128  = 0x04,
	FT9001_TC_PRESCALER_DIV64   = 0x05,
	FT9001_TC_PRESCALER_DIV32   = 0x06,
	FT9001_TC_PRESCALER_DIV16   = 0x07,
};

/** @brief Operating mode, encoded into TCCR.RN. */
enum ft9001_tc_mode {
	FT9001_TC_MODE_PERIODIC = 0,
	FT9001_TC_MODE_ONE_SHOT = 1,
};

/* IF acknowledges the pending event and CU reissues the reload, so every
 * read-modify-write puts them back as zero and the two routines that mean to
 * issue them do so explicitly.
 */
#define TC_TCCR_CMD_Msk ((uint16_t)(TC_TCCR_IF_Msk | TC_TCCR_CU_Msk))

static inline void tc_ccr_modify(TC_TypeDef *inst, uint16_t clear, uint16_t set)
{
	FT9001_MODIFY_REG(inst->TCCR, (uint16_t)(clear | TC_TCCR_CMD_Msk), set);
}

/** @brief Let the counter run: clear STOP along with the DBG/DOZE/WAIT halts. */
static inline void ft9001_tc_start(TC_TypeDef *inst)
{
	tc_ccr_modify(inst, (uint16_t)(TC_TCCR_WAIT_Msk | TC_TCCR_DOZE_Msk |
				       TC_TCCR_STOP_Msk | TC_TCCR_DBG_Msk), 0U);
}

/** @brief Stop the counter (TCCR.STOP). Other mode bits are left alone. */
static inline void ft9001_tc_stop(TC_TypeDef *inst)
{
	tc_ccr_modify(inst, 0U, (uint16_t)TC_TCCR_STOP);
}

/** @brief Read the counter (TCCNTR). */
static inline uint16_t ft9001_tc_counter_get(TC_TypeDef *inst)
{
	return inst->TCCNTR;
}

/** @brief Read the reload/modulus value (TCMR). */
static inline uint16_t ft9001_tc_reload_get(TC_TypeDef *inst)
{
	return inst->TCMR;
}

/** @brief Set the reload/modulus value and load it without waiting for a wrap. */
static inline void ft9001_tc_reload_set(TC_TypeDef *inst, uint16_t reload)
{
	inst->TCMR = reload;
	tc_ccr_modify(inst, 0U, (uint16_t)TC_TCCR_CU);
}

/** @brief Set the prescaler (TCCR.WDP). */
static inline void ft9001_tc_prescaler_set(TC_TypeDef *inst, enum ft9001_tc_prescaler psc)
{
	tc_ccr_modify(inst, (uint16_t)TC_TCCR_WDP_Msk,
		      (uint16_t)TC_TCCR_WDP_VAL((uint16_t)psc & 0x7U));
}

/** @brief Select periodic or one-shot operation (TCCR.RN). */
static inline void ft9001_tc_mode_set(TC_TypeDef *inst, enum ft9001_tc_mode mode)
{
	tc_ccr_modify(inst, (uint16_t)TC_TCCR_RN_Msk,
		      (mode == FT9001_TC_MODE_PERIODIC) ? (uint16_t)TC_TCCR_RN : 0U);
}

/**
 * @brief Set the DBG/DOZE/WAIT halt conditions.
 *
 * Replaces all three bits so stale state cannot accumulate.
 */
static inline void ft9001_tc_halt_bits_set(TC_TypeDef *inst, bool dbg, bool doze, bool wait)
{
	uint16_t clr = (uint16_t)(TC_TCCR_DBG_Msk | TC_TCCR_DOZE_Msk | TC_TCCR_WAIT_Msk);
	uint16_t set = (uint16_t)((dbg ? TC_TCCR_DBG_Msk : 0U) |
				  (doze ? TC_TCCR_DOZE_Msk : 0U) |
				  (wait ? TC_TCCR_WAIT_Msk : 0U));

	tc_ccr_modify(inst, clr, set);
}

/** @brief Enable the update interrupt (TCCR.IE). */
static inline void ft9001_tc_update_int_enable(TC_TypeDef *inst)
{
	tc_ccr_modify(inst, 0U, (uint16_t)TC_TCCR_IE);
}

/** @brief Disable the update interrupt (TCCR.IE). */
static inline void ft9001_tc_update_int_disable(TC_TypeDef *inst)
{
	tc_ccr_modify(inst, (uint16_t)TC_TCCR_IE_Msk, 0U);
}

/** @brief Read the update interrupt flag (TCCR.IF). */
static inline bool ft9001_tc_update_flag_get(TC_TypeDef *inst)
{
	return FT9001_READ_BIT(inst->TCCR, (uint16_t)TC_TCCR_IF_Msk) != 0U;
}

/** @brief Clear the update interrupt flag by writing one to TCCR.IF. */
static inline void ft9001_tc_update_flag_clear(TC_TypeDef *inst)
{
	tc_ccr_modify(inst, 0U, (uint16_t)TC_TCCR_IF);
}

#ifdef __cplusplus
}
#endif

#endif /* FT9001_TC_H_ */
