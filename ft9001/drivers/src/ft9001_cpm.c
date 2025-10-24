/*
 * Copyright (c) 2025-2026, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>

#include "ft9001.h"
#include "ft9001_cpm.h"

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

/* Budget for the OSC8M switch that precedes a trim write. Generous because the
 * routine may run from the high-speed oscillator, where each poll is cheap.
 */
#define CPM_TRIM_SWITCH_POLLS (2000000UL)

/* Track last HSOSC nominal freq when SYSCLK = OSC400M */
static uint32_t s_hsosc_nominal_hz = 320000000UL;

static int cpm_wait_bits_set(volatile uint32_t *reg, uint32_t mask, uint32_t poll_budget)
{
	if (poll_budget == FT9001_CPM_POLL_FOREVER) {
		while (((*reg) & mask) != mask) {
		}
		return 0;
	}

	while (poll_budget != 0U) {
		if (((*reg) & mask) == mask) {
			return 0;
		}
		poll_budget--;
	}

	return -ETIMEDOUT;
}

/* VCCCTMR: lock/undo override window used for protected trim registers */
static inline void cpm_lock_override(void)
{
	uint32_t base = FT9001_READ_REG(CPM->VCCCTMR) & ~CPM_VCCCTMR_CORE_TEST_KEY;

	FT9001_WRITE_REG(CPM->VCCCTMR, base | CPM_VCCCTMR_CORE_TEST_KEY_00);
}

static inline void cpm_unlock_override(uint32_t override_bits)
{
	uint32_t base = FT9001_READ_REG(CPM->VCCCTMR) & ~CPM_VCCCTMR_CORE_TEST_KEY;

	FT9001_WRITE_REG(CPM->VCCCTMR, base | CPM_VCCCTMR_CORE_TEST_KEY_01);
	FT9001_WRITE_REG(CPM->VCCCTMR, base | CPM_VCCCTMR_CORE_TEST_KEY_10);
	FT9001_WRITE_REG(CPM->VCCCTMR, base | CPM_VCCCTMR_CORE_TEST_KEY_11);
	FT9001_WRITE_REG(CPM->VCCCTMR, base | CPM_VCCCTMR_CORE_TEST_KEY_11 | override_bits);
}

/* Choose OTP part: prefer part2, then part1, then part0; default to part2. */
static inline uint32_t cpm_pick_otp_part(void)
{
	if (*(volatile uint32_t *)OTP_PART2_VALID_ADDR == OTP_VALID_SIGNATURE) {
		return 2UL;
	}
	if (*(volatile uint32_t *)OTP_PART1_VALID_ADDR == OTP_VALID_SIGNATURE) {
		return 1UL;
	}
	if (*(volatile uint32_t *)OTP_PART0_VALID_ADDR == OTP_VALID_SIGNATURE) {
		return 0UL;
	}
	return 2UL;
}

enum ft9001_cpm_sysclk_source ft9001_cpm_sysclk_source_get(void)
{
	uint32_t v = (FT9001_READ_REG(CPM->CSWCFGR) & CPM_CSWCFGR_SYS_SEL_Msk) >>
		     CPM_CSWCFGR_SYS_SEL_Pos;

	return (v & 0x1UL) ? FT9001_CPM_SYSCLK_OSC400M : FT9001_CPM_SYSCLK_OSC8M;
}

int ft9001_cpm_sysclk_source_set(enum ft9001_cpm_sysclk_source source, uint32_t poll_budget)
{
	int ret;

	switch (source) {
	case FT9001_CPM_SYSCLK_OSC8M:
		FT9001_SET_BIT(CPM->OCSR, CPM_OCSR_OSC8M_EN);
		ret = cpm_wait_bits_set(&CPM->OCSR, CPM_OCSR_OSC8M_STABLE, poll_budget);
		if (ret != 0) {
			return ret;
		}

		FT9001_MODIFY_REG(CPM->CSWCFGR, CPM_CSWCFGR_SYS_SEL_Msk,
				  CPM_CSWCFGR_SYS_SEL_OSC8M);
		return cpm_wait_bits_set(&CPM->CSWCFGR, CPM_CSWCFGR_SYS_SEL_ST_OSC8M, poll_budget);

	case FT9001_CPM_SYSCLK_OSC400M:
		FT9001_SET_BIT(CPM->OCSR, CPM_OCSR_OSC400M_EN);
		ret = cpm_wait_bits_set(&CPM->OCSR, CPM_OCSR_OSC400M_STABLE, poll_budget);
		if (ret != 0) {
			return ret;
		}

		FT9001_MODIFY_REG(CPM->CSWCFGR, CPM_CSWCFGR_SYS_SEL_Msk,
				  CPM_CSWCFGR_SYS_SEL_OSC400M);
		return cpm_wait_bits_set(&CPM->CSWCFGR, CPM_CSWCFGR_SYS_SEL_ST_OSC400M,
					 poll_budget);

	default:
		return -EINVAL;
	}
}

int ft9001_cpm_hsosc_trim_set(enum ft9001_cpm_osc_freq freq)
{
	/* The trim register may only be written while running from OSC8M. */
	int ret = ft9001_cpm_sysclk_source_set(FT9001_CPM_SYSCLK_OSC8M, CPM_TRIM_SWITCH_POLLS);

	if (ret != 0) {
		return ret;
	}

	switch (freq) {
	case FT9001_CPM_OSC_FREQ_320MHZ: {
		uint32_t part = cpm_pick_otp_part();
		uint32_t en_addr = OTP_BASE_TRIM_EN + part * OTP_PART_STRIDE;
		uint32_t val_addr = OTP_BASE_TRIM_VALUE + part * OTP_PART_STRIDE;

		if (*(volatile uint32_t *)en_addr != OTP_OSC320_TRIM_KEY) {
			return -ENOENT;
		}

		cpm_unlock_override(CPM_VCCCTMR_OVERWR_OSC400M_TRIM);
		FT9001_WRITE_REG(CPM->O400MTRIMR, *(volatile uint32_t *)val_addr);
		cpm_lock_override();

		s_hsosc_nominal_hz = 320000000UL;
		return 0;
	}

	case FT9001_CPM_OSC_FREQ_400MHZ: {
		uint32_t v = *(volatile uint32_t *)OTP_OSC400_TRIM_ADDR;

		if ((v & OTP_OSC_VALID_MASK) != OTP_OSC_VALID_TAG) {
			return -ENOENT;
		}

		cpm_unlock_override(CPM_VCCCTMR_OVERWR_OSC400M_TRIM);
		FT9001_WRITE_REG(CPM->O400MTRIMR, v);
		cpm_lock_override();

		s_hsosc_nominal_hz = 400000000UL;
		return 0;
	}

	default:
		return -EINVAL;
	}
}

int ft9001_cpm_ips_div_set(uint32_t div)
{
	/* 4-bit field: 0..15 divides by (N + 1) */
	if (div > 0xFUL) {
		return -EINVAL;
	}

	FT9001_SET_BIT(CPM->CDIVENR, CPM_CDIVENR_IPS_DIVEN);

	FT9001_MODIFY_REG(CPM->PCDIVR1, CPM_PCDIVR1_IPS_DIV_Msk,
			  (div << CPM_PCDIVR1_IPS_DIV_Pos));

	FT9001_SET_BIT(CPM->CDIVUPDR, CPM_CDIVUPDR_PERDIV_UPD);

	return 0;
}

uint32_t ft9001_cpm_sysclk_freq_hz_get(void)
{
	uint32_t base_hz;
	uint32_t sys_div;
	enum ft9001_cpm_sysclk_source clk_src = ft9001_cpm_sysclk_source_get();

	base_hz = (clk_src == FT9001_CPM_SYSCLK_OSC8M) ? 8000000UL : s_hsosc_nominal_hz;
	sys_div = (FT9001_READ_REG(CPM->SCDIVR) & CPM_SCDIVR_SYS_DIV_Msk) >>
		  CPM_SCDIVR_SYS_DIV_Pos;

	return base_hz / (sys_div + 1UL);
}
