/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ft9001.h"
#include "ft9001_hal_cpm.h"

/* OTP constants */
#define OTP_VALID_SIGNATURE (0x55AA55AAUL)

/* 320M trim key in OTP(partX EN address) */
#define OTP_OSC320_TRIM_KEY (0x77658320UL)

/* 400M trim blob tag at [31:24] == 0x92 */
#define OTP_OSC_VALID_TAG (0x92000000UL)
#define OTP_OSC_VALID_MASK (0xFF000000UL)

/* Part valid flags */
#define OTP_PART0_VALID_ADDR (0x082007F0UL)
#define OTP_PART1_VALID_ADDR (0x082007F4UL)
#define OTP_PART2_VALID_ADDR (0x082007F8UL)

/* 320M trim */
#define OTP_BASE_TRIM_VALUE (0x08200700UL) /* partX value    */
#define OTP_BASE_TRIM_EN (0x08200704UL)	   /* partX enable   */
#define OTP_PART_STRIDE (0x60UL)

/* 400M trim */
#define OTP_OSC400_TRIM_ADDR (0x082000E4UL)

/* Track last HSOSC nominal freq when SYSCLK = OSC400M */
static uint32_t s_hsosc_nominal_hz = 320000000UL;

static HAL_StatusTypeDef CPM_WaitBitsSet(volatile uint32_t *reg, uint32_t mask, uint32_t timeout_ms)
{
	uint32_t budget = (timeout_ms == 0U) ? 0xFFFFFFFFUL : (timeout_ms * 20000UL);

	while (((*reg) & mask) != mask) {
		if (budget-- == 0U) {
			return HAL_TIMEOUT;
		}
	}

	return HAL_OK;
}

/* VCCCTMR: lock/undo override window used for protected trim registers */
static inline void CPM_LockOverride(void)
{
	uint32_t base = READ_REG(CPM->VCCCTMR) & ~CPM_VCCCTMR_CORE_TEST_KEY;

	WRITE_REG(CPM->VCCCTMR, base | CPM_VCCCTMR_CORE_TEST_KEY_00);
}

static inline void CPM_UnlockOverride(uint32_t override_bits)
{
	uint32_t base = READ_REG(CPM->VCCCTMR) & ~CPM_VCCCTMR_CORE_TEST_KEY;

	WRITE_REG(CPM->VCCCTMR, base | CPM_VCCCTMR_CORE_TEST_KEY_01);
	WRITE_REG(CPM->VCCCTMR, base | CPM_VCCCTMR_CORE_TEST_KEY_10);
	WRITE_REG(CPM->VCCCTMR, base | CPM_VCCCTMR_CORE_TEST_KEY_11);
	WRITE_REG(CPM->VCCCTMR, base | CPM_VCCCTMR_CORE_TEST_KEY_11 | override_bits);
}

/* Choose OTP part: prefer part2, then part1, then part0; default to part2. */
static inline uint32_t CPM_PickOtpPart(void)
{
	if (*(volatile uint32_t *)OTP_PART2_VALID_ADDR == OTP_VALID_SIGNATURE) return 2UL;
	if (*(volatile uint32_t *)OTP_PART1_VALID_ADDR == OTP_VALID_SIGNATURE) return 1UL;
	if (*(volatile uint32_t *)OTP_PART0_VALID_ADDR == OTP_VALID_SIGNATURE) return 0UL;
	return 2UL;
}

static inline CPM_SysClkSourceTypeDef CPM_GetSysClockSource(void)
{
	uint32_t v = (READ_REG(CPM->CSWCFGR) & CPM_CSWCFGR_SYS_SEL_Msk) >> CPM_CSWCFGR_SYS_SEL_Pos;

	return (v & 0x1UL) ? CPM_SYSCLK_OSC400M : CPM_SYSCLK_OSC8M;
}

/* ------------ Public API ------------ */

HAL_StatusTypeDef HAL_CPM_SetSysClock(CPM_SysClkSourceTypeDef source, uint32_t timeout_ms)
{
	HAL_StatusTypeDef st;

	switch (source) {
		case CPM_SYSCLK_OSC8M:
			/* Enable OSC8M and wait stable */
			SET_BIT(CPM->OCSR, CPM_OCSR_OSC8M_EN);
			st = CPM_WaitBitsSet(&CPM->OCSR, CPM_OCSR_OSC8M_STABLE, timeout_ms);
			if (st != HAL_OK) return st;

			/* Switch SYS_SEL -> OSC8M and wait status reflects it */
			MODIFY_REG(CPM->CSWCFGR, CPM_CSWCFGR_SYS_SEL_Msk, CPM_CSWCFGR_SYS_SEL_OSC8M);
			st = CPM_WaitBitsSet(&CPM->CSWCFGR, CPM_CSWCFGR_SYS_SEL_ST_OSC8M, timeout_ms);
			return st;

		case CPM_SYSCLK_OSC400M:
			/* Enable HSOSC and wait stable */
			SET_BIT(CPM->OCSR, CPM_OCSR_OSC400M_EN);
			st = CPM_WaitBitsSet(&CPM->OCSR, CPM_OCSR_OSC400M_STABLE, timeout_ms);
			if (st != HAL_OK) return st;

			/* Switch SYS_SEL -> HSOSC and wait status reflects it */
			MODIFY_REG(CPM->CSWCFGR, CPM_CSWCFGR_SYS_SEL_Msk, CPM_CSWCFGR_SYS_SEL_OSC400M);
			st = CPM_WaitBitsSet(&CPM->CSWCFGR, CPM_CSWCFGR_SYS_SEL_ST_OSC400M, timeout_ms);
			return st;

		default:
			return HAL_ERROR;
	}
}

HAL_StatusTypeDef HAL_CPM_SetHighSpeedOscTrim(CPM_OscFreqTypeDef freq)
{
	/* Switch to OSC8M before trimming the high-speed oscillator */
	HAL_StatusTypeDef st = HAL_CPM_SetSysClock(CPM_SYSCLK_OSC8M, 100U);
	if (st != HAL_OK) return st;

	switch (freq)
	{
		case CPM_OSC_FREQ_320MHZ:
		{
			uint32_t part     = CPM_PickOtpPart();
			uint32_t en_addr  = OTP_BASE_TRIM_EN    + part * OTP_PART_STRIDE;
			uint32_t val_addr = OTP_BASE_TRIM_VALUE + part * OTP_PART_STRIDE;

			if (*(volatile uint32_t *)en_addr == OTP_OSC320_TRIM_KEY)
			{
				CPM_UnlockOverride(CPM_VCCCTMR_OVERWR_OSC400M_TRIM);
				WRITE_REG(CPM->O400MTRIMR, *(volatile uint32_t *)val_addr);
				CPM_LockOverride();

				s_hsosc_nominal_hz = 320000000UL;
				return HAL_OK;
			}
			return HAL_ERROR;
		}

		case CPM_OSC_FREQ_400MHZ:
		{
			uint32_t v = *(volatile uint32_t *)OTP_OSC400_TRIM_ADDR;
			if ((v & OTP_OSC_VALID_MASK) == OTP_OSC_VALID_TAG)
			{
				CPM_UnlockOverride(CPM_VCCCTMR_OVERWR_OSC400M_TRIM);
				WRITE_REG(CPM->O400MTRIMR, v);
				CPM_LockOverride();

				s_hsosc_nominal_hz = 400000000UL;
				return HAL_OK;
			}
			return HAL_ERROR;
		}

		default:
			return HAL_ERROR;
	}
}

HAL_StatusTypeDef HAL_CPM_SetIpsClkDiv(uint32_t div)
{
	if (div > 0xFUL) {
		/* field is 4-bit wide: 0..15 => divide by (N+1) */
		return HAL_ERROR;
	}

	SET_BIT(CPM->CDIVENR, CPM_CDIVENR_IPS_DIVEN);

	MODIFY_REG(CPM->PCDIVR1,
			CPM_PCDIVR1_IPS_DIV_Msk,
			(div << CPM_PCDIVR1_IPS_DIV_Pos));

	SET_BIT(CPM->CDIVUPDR, CPM_CDIVUPDR_PERDIV_UPD);

	return HAL_OK;
}

uint32_t HAL_CPM_GetSysClockFreqHz(void)
{
	uint32_t base_hz;
	uint32_t sys_div;
	CPM_SysClkSourceTypeDef clk_src = CPM_GetSysClockSource();

	base_hz = (clk_src == CPM_SYSCLK_OSC8M) ? 8000000UL : s_hsosc_nominal_hz;
	sys_div = (READ_REG(CPM->SCDIVR) & CPM_SCDIVR_SYS_DIV_Msk) >> CPM_SCDIVR_SYS_DIV_Pos;

	return base_hz / (sys_div + 1UL);
}
