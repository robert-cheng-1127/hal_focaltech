/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ft9001.h"
#include "ft9001_hal_tc.h"

void HAL_TC_Start(TC_TypeDef *inst)
{
	/* Clear WAIT/DOZE/STOP/DBG in one operation. */
	uint16_t clr = (uint16_t)(TC_TCCR_WAIT_Msk |
							  TC_TCCR_DOZE_Msk |
							  TC_TCCR_STOP_Msk |
							  TC_TCCR_DBG_Msk);

	inst->TCCR &= (uint16_t)~clr;
}

void HAL_TC_Stop(TC_TypeDef *inst)
{
	SET_BIT(inst->TCCR, TC_TCCR_STOP);
}

uint16_t HAL_TC_GetCounter(TC_TypeDef *inst)
{
	return inst->TCCNTR;
}

uint16_t HAL_TC_GetReload(TC_TypeDef *inst)
{
	return inst->TCMR;
}

void HAL_TC_SetReload(TC_TypeDef *inst, uint16_t reload)
{
	/* Program TCMR then request immediate load. */
	inst->TCMR = reload;
	SET_BIT(inst->TCCR, TC_TCCR_CU);
}

void HAL_TC_SetPrescaler(TC_TypeDef *inst, HAL_TC_Prescaler psc)
{
	/* Only WDP[2:0] are modified. Encoding maps to DIV2048..DIV16. */
	MODIFY_REG(inst->TCCR,
			   (uint16_t)TC_TCCR_WDP_Msk,
			   (uint16_t)TC_TCCR_WDP_VAL(psc & 0x7u));
}

void HAL_TC_ConfigMode(TC_TypeDef *inst, HAL_TC_Mode mode)
{
	if (mode == HAL_TC_MODE_PERIODIC) {
		/* reload at zero */
		SET_BIT(inst->TCCR, TC_TCCR_RN);
	}
	else {
		/* one-shot */
		CLEAR_BIT(inst->TCCR, TC_TCCR_RN);
	}
}

void HAL_TC_ConfigHaltBits(TC_TypeDef *inst, bool dbg, bool doze, bool wait)
{
	/* Replace semantics: clear then set per arguments. */
	uint16_t clr = (uint16_t)(TC_TCCR_DBG_Msk | TC_TCCR_DOZE_Msk | TC_TCCR_WAIT_Msk);
	uint16_t set = (uint16_t)((dbg  ? TC_TCCR_DBG_Msk  : 0u) |
							  (doze ? TC_TCCR_DOZE_Msk : 0u) |
							  (wait ? TC_TCCR_WAIT_Msk : 0u));

	MODIFY_REG(inst->TCCR, clr, set);
}

void HAL_TC_EnableIT_Update(TC_TypeDef *inst, bool enable)
{
	if (enable) {
		SET_BIT(inst->TCCR, TC_TCCR_IE);
	}
	else {
		CLEAR_BIT(inst->TCCR, TC_TCCR_IE);
	}
}

bool HAL_TC_GetFlag_Update(TC_TypeDef *inst)
{
	return (READ_BIT(inst->TCCR, TC_TCCR_IF_Msk) != 0u);
}

void HAL_TC_ClearFlag_Update(TC_TypeDef *inst)
{
	/* W1C: writing '1' to IF clears the flag. */
	SET_BIT(inst->TCCR, TC_TCCR_IF);
}
