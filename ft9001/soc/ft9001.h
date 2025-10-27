/*
 * Copyright (c) 2025, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __FT9001_H
#define __FT9001_H

#ifdef __cplusplus
extern "C" {
#endif

#include "system_ft9001.h"
#include <stdint.h>

#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif
#ifndef __IO
#define __IO volatile
#endif

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define CLEAR_REG(REG)        ((REG) = (0x0))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  \
	(WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK))))

/* -------------------------------------------------------------------------- */
/* CPM register map                                                           */
/* -------------------------------------------------------------------------- */

typedef struct
{
	__IO uint32_t SLPCFGR;      /*!< 0x00 Sleep configuration register */
	__IO uint32_t SLPCR;        /*!< 0x04 Sleep control register */
	__IO uint32_t SCDIVR;       /*!< 0x08 System clock divider register */
	__IO uint32_t PCDIVR1;      /*!< 0x0C Peripheral clock divider register 1 */
	__IO uint32_t PCDIVR2;      /*!< 0x10 Peripheral clock divider register 2 */
	__IO uint32_t RESERVED0;    /*!< 0x14 Reserved */
	__IO uint32_t CDIVUPDR;     /*!< 0x18 Clock divider update register */
	__IO uint32_t CDIVENR;      /*!< 0x1C Clock divider enable register */
	__IO uint32_t OCSR;         /*!< 0x20 Oscillator control and status register */
	__IO uint32_t CSWCFGR;      /*!< 0x24 Clock switch configuration register */
	__IO uint32_t CTICKR;       /*!< 0x28 Core tick timer register */
	__IO uint32_t CHIPCFGR;     /*!< 0x2C Chip configuration register */
	__IO uint32_t PWRCR;        /*!< 0x30 Power control register */
	__IO uint32_t SLPCNTR;      /*!< 0x34 Sleep counter register */
	__IO uint32_t WKPCNTR;      /*!< 0x38 Wake-up counter register */
	__IO uint32_t MULTICGTCR;   /*!< 0x3C Multiple clock gate control register */
	__IO uint32_t SYSCGTCR;     /*!< 0x40 System clock gate control register */
	__IO uint32_t AHB3CGTCR;    /*!< 0x44 AHB3 clock gate control register */
	__IO uint32_t ARITHCGTCR;   /*!< 0x48 Arithmetic clock gate control register */
	__IO uint32_t IPSCGTCR;     /*!< 0x4C IPS clock gate control register */
	__IO uint32_t VCCGTRIMR;    /*!< 0x50 VCC general trim register */
	__IO uint32_t VCCLTRIMR;    /*!< 0x54 VCC low-voltage detect trim register */
	__IO uint32_t VCCVTRIMR;    /*!< 0x58 VCC Vref trim register */
	__IO uint32_t VCCCTMR;      /*!< 0x5C VCC core test mode register */
	__IO uint32_t O8MTRIMR;     /*!< 0x60 OSC 8 MHz trim register */
	__IO uint32_t RESERVED1;    /*!< 0x64 Reserved */
	__IO uint32_t O400MTRIMR;   /*!< 0x68 High-speed OSC trim register (320/360/400 MHz) */
	__IO uint32_t CARDTRIMR;    /*!< 0x6C Card LDO trim register */
	__IO uint32_t OSCLSTIMER;   /*!< 0x70 OSCL stable time register */
	__IO uint32_t OSCHSTIMER;   /*!< 0x74 OSCH stable time register */
	__IO uint32_t OSCESTIMER;   /*!< 0x78 OSCE stable time register */
	__IO uint32_t PWRSR;        /*!< 0x7C Power status register */
	__IO uint32_t EPORTSLPCFGR; /*!< 0x80 EPORT sleep configuration register */
	__IO uint32_t EPORTCGTR;    /*!< 0x84 EPORT clock gate control register */
	__IO uint32_t EPORTRSTCR;   /*!< 0x88 EPORT reset control register */
	__IO uint32_t RTCTRIMR;     /*!< 0x8C RTC trim register */
	__IO uint32_t PADWKINTCR;   /*!< 0x90 Pad wakeup interrupt control register */
	__IO uint32_t WKPFILTCNTR;  /*!< 0x94 Wakeup filter counter register */
	__IO uint32_t CARDPOCR;     /*!< 0x98 Card power-on counter register */
	__IO uint32_t RTCSTIMER;    /*!< 0x9C RTC 32k stable time register */
	__IO uint32_t MPDSLPCR;     /*!< 0xA0 Memory power-down sleep control register */
	__IO uint32_t RESERVED2[2]; /*!< 0xA4 - 0xA8 Reserved */
	__IO uint32_t MULTIRSTCR;   /*!< 0xAC Multiple reset control register */
	__IO uint32_t SYSRSTCR;     /*!< 0xB0 System reset control register */
	__IO uint32_t AHB3RSTCR;    /*!< 0xB4 AHB3 reset control register */
	__IO uint32_t ARITHRSTTCR;  /*!< 0xB8 Arithmetic reset control register */
	__IO uint32_t IPRSTCR;      /*!< 0xBC IPS reset control register */
	__IO uint32_t SLPCFGR2;     /*!< 0xC0 Sleep configuration register 2 */
	__IO uint32_t RESERVED3[3]; /*!< 0xC4 - 0xCC Reserved */
	__IO uint32_t PDNCNTR;      /*!< 0xD0 Power-down counter register */
	__IO uint32_t PONCNTR;      /*!< 0xD4 Power-on counter register */
	__IO uint32_t PCDIVR4;      /*!< 0xD8 Peripheral clock divider register 4 */
	__IO uint32_t RESERVED4;    /*!< 0xDC Reserved */
	__IO uint32_t PLLNFCCFGR;   /*!< 0xE0 NFC PLL configuration register */
	__IO uint32_t PLLNFCSTIMER; /*!< 0xE4 NFC PLL stable time register */
} CPM_TypeDef;

/* -------------------------------------------------------------------------- */
/* WDT register map                                                           */
/* -------------------------------------------------------------------------- */

typedef struct
{
	__IO uint16_t WCR;    /*!< 0x00 Control */
	__IO uint16_t WMR;    /*!< 0x02 Reload/Match */
	__I  uint16_t WCNTR;  /*!< 0x04 Counter (RO) */
	__IO uint16_t WSR;    /*!< 0x06 Service */
} WDT_TypeDef;

/* Base addresses for memory regions */
#define PERIPH_BASE      0x40000000UL    /*!< Peripheral base address*/

/* Peripheral base addresses */
#define CPM_BASE         (PERIPH_BASE + 0x00004000UL) /*!< Clock and Power Management base address */
#define WDT_BASE         (PERIPH_BASE + 0x00005000UL)

/* peripheral declaration */
#define CPM              ((CPM_TypeDef *)(CPM_BASE))
#define WDT              ((WDT_TypeDef *)(WDT_BASE))

/* -------------------------------------------------------------------------- */
/* Bit definitions                                                            */
/* -------------------------------------------------------------------------- */

/******************************************************************************/
/*                                                                            */
/*                        Clock Power Module                                  */
/*                                                                            */
/******************************************************************************/

/*******************  Bits definition for CPM_SLPCFGR register  ***************/

/* [31:30] SLEEP_MODE[1:0] */
#define CPM_SLPCFGR_SLEEP_MODE_Pos           (30U)
#define CPM_SLPCFGR_SLEEP_MODE_Msk           (0x3UL << CPM_SLPCFGR_SLEEP_MODE_Pos)  /*!< 0xC0000000 */
#define CPM_SLPCFGR_SLEEP_MODE               CPM_SLPCFGR_SLEEP_MODE_Msk
#define CPM_SLPCFGR_SLEEP_MODE_LOW_POWER     (0x0UL << CPM_SLPCFGR_SLEEP_MODE_Pos)  /*!< 0x00000000 */
#define CPM_SLPCFGR_SLEEP_MODE_HIBERNATION   (0x2UL << CPM_SLPCFGR_SLEEP_MODE_Pos)  /*!< 0x80000000 */

/* [29] EN_LP_DIS */
#define CPM_SLPCFGR_EN_LP_DIS_Pos            (29U)
#define CPM_SLPCFGR_EN_LP_DIS_Msk            (0x1UL << CPM_SLPCFGR_EN_LP_DIS_Pos)   /*!< 0x20000000 */
#define CPM_SLPCFGR_EN_LP_DIS                CPM_SLPCFGR_EN_LP_DIS_Msk

/* [28] EXTFLASH_IPSLP6 (SSI6) */
#define CPM_SLPCFGR_EXTFLASH_IPSLP6_Pos      (28U)
#define CPM_SLPCFGR_EXTFLASH_IPSLP6_Msk      (0x1UL << CPM_SLPCFGR_EXTFLASH_IPSLP6_Pos) /*!< 0x10000000 */
#define CPM_SLPCFGR_EXTFLASH_IPSLP6          CPM_SLPCFGR_EXTFLASH_IPSLP6_Msk

/* [27] EXTFLASH_IPSLP5 (SSI5) */
#define CPM_SLPCFGR_EXTFLASH_IPSLP5_Pos      (27U)
#define CPM_SLPCFGR_EXTFLASH_IPSLP5_Msk      (0x1UL << CPM_SLPCFGR_EXTFLASH_IPSLP5_Pos) /*!< 0x08000000 */
#define CPM_SLPCFGR_EXTFLASH_IPSLP5          CPM_SLPCFGR_EXTFLASH_IPSLP5_Msk

/* [26] EXTFLASH_IPSLP4 (SSI4) */
#define CPM_SLPCFGR_EXTFLASH_IPSLP4_Pos      (26U)
#define CPM_SLPCFGR_EXTFLASH_IPSLP4_Msk      (0x1UL << CPM_SLPCFGR_EXTFLASH_IPSLP4_Pos) /*!< 0x04000000 */
#define CPM_SLPCFGR_EXTFLASH_IPSLP4          CPM_SLPCFGR_EXTFLASH_IPSLP4_Msk

/* [23] MCC_SLPEN */
#define CPM_SLPCFGR_MCC_SLPEN_Pos            (23U)
#define CPM_SLPCFGR_MCC_SLPEN_Msk            (0x1UL << CPM_SLPCFGR_MCC_SLPEN_Pos)   /*!< 0x00800000 */
#define CPM_SLPCFGR_MCC_SLPEN                CPM_SLPCFGR_MCC_SLPEN_Msk

/* [22] HP_READY_WKPWAIT */
#define CPM_SLPCFGR_HP_READY_WKPWAIT_Pos     (22U)
#define CPM_SLPCFGR_HP_READY_WKPWAIT_Msk     (0x1UL << CPM_SLPCFGR_HP_READY_WKPWAIT_Pos) /*!< 0x00400000 */
#define CPM_SLPCFGR_HP_READY_WKPWAIT         CPM_SLPCFGR_HP_READY_WKPWAIT_Msk

/* [21] WAKEUP_NM_SLPWAIT */
#define CPM_SLPCFGR_WAKEUP_NM_SLPWAIT_Pos    (21U)
#define CPM_SLPCFGR_WAKEUP_NM_SLPWAIT_Msk    (0x1UL << CPM_SLPCFGR_WAKEUP_NM_SLPWAIT_Pos) /*!< 0x00200000 */
#define CPM_SLPCFGR_WAKEUP_NM_SLPWAIT        CPM_SLPCFGR_WAKEUP_NM_SLPWAIT_Msk

/* [20] OSCEXT_SLPEN */
#define CPM_SLPCFGR_OSCEXT_SLPEN_Pos         (20U)
#define CPM_SLPCFGR_OSCEXT_SLPEN_Msk         (0x1UL << CPM_SLPCFGR_OSCEXT_SLPEN_Pos) /*!< 0x00100000 */
#define CPM_SLPCFGR_OSCEXT_SLPEN             CPM_SLPCFGR_OSCEXT_SLPEN_Msk

/* [19] PMU128K_SLPEN */
#define CPM_SLPCFGR_PMU128K_SLPEN_Pos        (19U)
#define CPM_SLPCFGR_PMU128K_SLPEN_Msk        (0x1UL << CPM_SLPCFGR_PMU128K_SLPEN_Pos) /*!< 0x00080000 */
#define CPM_SLPCFGR_PMU128K_SLPEN            CPM_SLPCFGR_PMU128K_SLPEN_Msk

/* [16] RTC32K_SLPEN */
#define CPM_SLPCFGR_RTC32K_SLPEN_Pos         (16U)
#define CPM_SLPCFGR_RTC32K_SLPEN_Msk         (0x1UL << CPM_SLPCFGR_RTC32K_SLPEN_Pos) /*!< 0x00010000 */
#define CPM_SLPCFGR_RTC32K_SLPEN             CPM_SLPCFGR_RTC32K_SLPEN_Msk

/* [11] CARD0PD */
#define CPM_SLPCFGR_CARD0PD_Pos              (11U)
#define CPM_SLPCFGR_CARD0PD_Msk              (0x1UL << CPM_SLPCFGR_CARD0PD_Pos)     /*!< 0x00000800 */
#define CPM_SLPCFGR_CARD0PD                  CPM_SLPCFGR_CARD0PD_Msk

/* [9:8] CARD0PO[1:0] */
#define CPM_SLPCFGR_CARD0PO_Pos              (8U)
#define CPM_SLPCFGR_CARD0PO_Msk              (0x3UL << CPM_SLPCFGR_CARD0PO_Pos)     /*!< 0x00000300 */
#define CPM_SLPCFGR_CARD0PO                  CPM_SLPCFGR_CARD0PO_Msk
#define CPM_SLPCFGR_CARD0PO_1V8              (0x0UL << CPM_SLPCFGR_CARD0PO_Pos)     /*!< 0x00000000 */
#define CPM_SLPCFGR_CARD0PO_3V0              (0x1UL << CPM_SLPCFGR_CARD0PO_Pos)     /*!< 0x00000100 */
#define CPM_SLPCFGR_CARD0PO_RSVD             (0x2UL << CPM_SLPCFGR_CARD0PO_Pos)     /*!< 0x00000200 */
#define CPM_SLPCFGR_CARD0PO_3V3              (0x3UL << CPM_SLPCFGR_CARD0PO_Pos)     /*!< 0x00000300 */

/* [4] OTP_IPSLP */
#define CPM_SLPCFGR_OTP_IPSLP_Pos            (4U)
#define CPM_SLPCFGR_OTP_IPSLP_Msk            (0x1UL << CPM_SLPCFGR_OTP_IPSLP_Pos)   /*!< 0x00000010 */
#define CPM_SLPCFGR_OTP_IPSLP                CPM_SLPCFGR_OTP_IPSLP_Msk

/* [1] SSI_IDLE_WKPWAIT */
#define CPM_SLPCFGR_SSI_IDLE_WKPWAIT_Pos     (1U)
#define CPM_SLPCFGR_SSI_IDLE_WKPWAIT_Msk     (0x1UL << CPM_SLPCFGR_SSI_IDLE_WKPWAIT_Pos) /*!< 0x00000002 */
#define CPM_SLPCFGR_SSI_IDLE_WKPWAIT         CPM_SLPCFGR_SSI_IDLE_WKPWAIT_Msk

/* [0] SSI_IDLE_SLPWAIT */
#define CPM_SLPCFGR_SSI_IDLE_SLPWAIT_Pos     (0U)
#define CPM_SLPCFGR_SSI_IDLE_SLPWAIT_Msk     (0x1UL << CPM_SLPCFGR_SSI_IDLE_SLPWAIT_Pos) /*!< 0x00000001 */
#define CPM_SLPCFGR_SSI_IDLE_SLPWAIT         CPM_SLPCFGR_SSI_IDLE_SLPWAIT_Msk

/*******************  Bits definition for CPM_SCDIVR register  *****************/
/**
 * System / TRACE / CLKOUT clock division
 * Effective divider = field_value + 1
 * New values take effect when CDIVUPDR.SYSDIV_UPD is set.
 */

/* [23:16] CLKOUT_DIV[7:0] */
#define CPM_SCDIVR_CLKOUT_DIV_Pos            (16U)
#define CPM_SCDIVR_CLKOUT_DIV_Msk            (0xFFUL << CPM_SCDIVR_CLKOUT_DIV_Pos)   /*!< 0x00FF0000 */
#define CPM_SCDIVR_CLKOUT_DIV                CPM_SCDIVR_CLKOUT_DIV_Msk
/* Helper to build field value: n = 0..255 (divider = n+1) */
#define CPM_SCDIVR_CLKOUT_DIV_VAL(n)         (((uint32_t)(n) & 0xFFUL) << CPM_SCDIVR_CLKOUT_DIV_Pos)

/* [15:8] TRACE_DIV[7:0] */
#define CPM_SCDIVR_TRACE_DIV_Pos             (8U)
#define CPM_SCDIVR_TRACE_DIV_Msk             (0xFFUL << CPM_SCDIVR_TRACE_DIV_Pos)    /*!< 0x0000FF00 */
#define CPM_SCDIVR_TRACE_DIV                 CPM_SCDIVR_TRACE_DIV_Msk
#define CPM_SCDIVR_TRACE_DIV_VAL(n)          (((uint32_t)(n) & 0xFFUL) << CPM_SCDIVR_TRACE_DIV_Pos)

/* [7:0] SYS_DIV[7:0] */
#define CPM_SCDIVR_SYS_DIV_Pos               (0U)
#define CPM_SCDIVR_SYS_DIV_Msk               (0xFFUL << CPM_SCDIVR_SYS_DIV_Pos)      /*!< 0x000000FF */
#define CPM_SCDIVR_SYS_DIV                   CPM_SCDIVR_SYS_DIV_Msk
#define CPM_SCDIVR_SYS_DIV_VAL(n)            (((uint32_t)(n) & 0xFFUL) << CPM_SCDIVR_SYS_DIV_Pos)

/*******************  Bits definition for CPM_PCDIVR1 register  ****************/
/* [15:12] ARITH_DIV[3:0] */
#define CPM_PCDIVR1_ARITH_DIV_Pos           (12U)
#define CPM_PCDIVR1_ARITH_DIV_Msk           (0xFUL << CPM_PCDIVR1_ARITH_DIV_Pos)
#define CPM_PCDIVR1_ARITH_DIV               CPM_PCDIVR1_ARITH_DIV_Msk

/* [11:8] AHB3_DIV[3:0] */
#define CPM_PCDIVR1_AHB3_DIV_Pos            (8U)
#define CPM_PCDIVR1_AHB3_DIV_Msk            (0xFUL << CPM_PCDIVR1_AHB3_DIV_Pos)
#define CPM_PCDIVR1_AHB3_DIV                CPM_PCDIVR1_AHB3_DIV_Msk

/* [3:0] IPS_DIV[3:0] */
#define CPM_PCDIVR1_IPS_DIV_Pos             (0U)
#define CPM_PCDIVR1_IPS_DIV_Msk             (0xFUL << CPM_PCDIVR1_IPS_DIV_Pos)
#define CPM_PCDIVR1_IPS_DIV                 CPM_PCDIVR1_IPS_DIV_Msk

/*******************  Bits definition for CPM_CDIVUPDR register  **************/
/* [1] SYSDIV_UPD */
#define CPM_CDIVUPDR_SYSDIV_UPD_Pos         (1U)
#define CPM_CDIVUPDR_SYSDIV_UPD_Msk         (0x1UL << CPM_CDIVUPDR_SYSDIV_UPD_Pos)
#define CPM_CDIVUPDR_SYSDIV_UPD             CPM_CDIVUPDR_SYSDIV_UPD_Msk

/* [0] PERDIV_UPD */
#define CPM_CDIVUPDR_PERDIV_UPD_Pos         (0U)
#define CPM_CDIVUPDR_PERDIV_UPD_Msk         (0x1UL << CPM_CDIVUPDR_PERDIV_UPD_Pos)
#define CPM_CDIVUPDR_PERDIV_UPD             CPM_CDIVUPDR_PERDIV_UPD_Msk

/*******************  Bits definition for CPM_CDIVENR register  ***************/
/* [23] I2S_S_DIVEN */
#define CPM_CDIVENR_I2S_S_DIVEN_Pos         (23U)
#define CPM_CDIVENR_I2S_S_DIVEN_Msk         (0x1UL << CPM_CDIVENR_I2S_S_DIVEN_Pos)
#define CPM_CDIVENR_I2S_S_DIVEN             CPM_CDIVENR_I2S_S_DIVEN_Msk

/* [22] I2S_M_DIVEN */
#define CPM_CDIVENR_I2S_M_DIVEN_Pos         (22U)
#define CPM_CDIVENR_I2S_M_DIVEN_Msk         (0x1UL << CPM_CDIVENR_I2S_M_DIVEN_Pos)
#define CPM_CDIVENR_I2S_M_DIVEN             CPM_CDIVENR_I2S_M_DIVEN_Msk

/* [15] CLKOUT_DIVEN */
#define CPM_CDIVENR_CLKOUT_DIVEN_Pos        (15U)
#define CPM_CDIVENR_CLKOUT_DIVEN_Msk        (0x1UL << CPM_CDIVENR_CLKOUT_DIVEN_Pos)
#define CPM_CDIVENR_CLKOUT_DIVEN            CPM_CDIVENR_CLKOUT_DIVEN_Msk

/* [14] TRACE_DIVEN */
#define CPM_CDIVENR_TRACE_DIVEN_Pos         (14U)
#define CPM_CDIVENR_TRACE_DIVEN_Msk         (0x1UL << CPM_CDIVENR_TRACE_DIVEN_Pos)
#define CPM_CDIVENR_TRACE_DIVEN             CPM_CDIVENR_TRACE_DIVEN_Msk

/* [13] TC_DIVEN */
#define CPM_CDIVENR_TC_DIVEN_Pos            (13U)
#define CPM_CDIVENR_TC_DIVEN_Msk            (0x1UL << CPM_CDIVENR_TC_DIVEN_Pos)
#define CPM_CDIVENR_TC_DIVEN                CPM_CDIVENR_TC_DIVEN_Msk

/* [12] MESH_DIVEN */
#define CPM_CDIVENR_MESH_DIVEN_Pos          (12U)
#define CPM_CDIVENR_MESH_DIVEN_Msk          (0x1UL << CPM_CDIVENR_MESH_DIVEN_Pos)
#define CPM_CDIVENR_MESH_DIVEN              CPM_CDIVENR_MESH_DIVEN_Msk

/* [10] ADC_DIVEN */
#define CPM_CDIVENR_ADC_DIVEN_Pos           (10U)
#define CPM_CDIVENR_ADC_DIVEN_Msk           (0x1UL << CPM_CDIVENR_ADC_DIVEN_Pos)
#define CPM_CDIVENR_ADC_DIVEN               CPM_CDIVENR_ADC_DIVEN_Msk

/* [8] MCC_DIVEN */
#define CPM_CDIVENR_MCC_DIVEN_Pos           (8U)
#define CPM_CDIVENR_MCC_DIVEN_Msk           (0x1UL << CPM_CDIVENR_MCC_DIVEN_Pos)
#define CPM_CDIVENR_MCC_DIVEN               CPM_CDIVENR_MCC_DIVEN_Msk

/* [3] ARITH_DIVEN */
#define CPM_CDIVENR_ARITH_DIVEN_Pos         (3U)
#define CPM_CDIVENR_ARITH_DIVEN_Msk         (0x1UL << CPM_CDIVENR_ARITH_DIVEN_Pos)
#define CPM_CDIVENR_ARITH_DIVEN             CPM_CDIVENR_ARITH_DIVEN_Msk

/* [2] AHB3_DIVEN */
#define CPM_CDIVENR_AHB3_DIVEN_Pos          (2U)
#define CPM_CDIVENR_AHB3_DIVEN_Msk          (0x1UL << CPM_CDIVENR_AHB3_DIVEN_Pos)
#define CPM_CDIVENR_AHB3_DIVEN              CPM_CDIVENR_AHB3_DIVEN_Msk

/* [0] IPS_DIVEN */
#define CPM_CDIVENR_IPS_DIVEN_Pos           (0U)
#define CPM_CDIVENR_IPS_DIVEN_Msk           (0x1UL << CPM_CDIVENR_IPS_DIVEN_Pos)
#define CPM_CDIVENR_IPS_DIVEN               CPM_CDIVENR_IPS_DIVEN_Msk

/*******************  Bits definition for CPM_OCSR register  ******************/
/* [15]..[8] status, [7]..[0] enable */
/* [15] PLLNFC_STABLE */
#define CPM_OCSR_PLLNFC_STABLE_Pos           (15U)
#define CPM_OCSR_PLLNFC_STABLE_Msk           (0x1UL << CPM_OCSR_PLLNFC_STABLE_Pos)
#define CPM_OCSR_PLLNFC_STABLE               CPM_OCSR_PLLNFC_STABLE_Msk

/* [14] PMU2K_VALID */
#define CPM_OCSR_PMU2K_VALID_Pos             (14U)
#define CPM_OCSR_PMU2K_VALID_Msk             (0x1UL << CPM_OCSR_PMU2K_VALID_Pos)
#define CPM_OCSR_PMU2K_VALID                 CPM_OCSR_PMU2K_VALID_Msk

/* [13] RTC32K_STABLE */
#define CPM_OCSR_RTC32K_STABLE_Pos           (13U)
#define CPM_OCSR_RTC32K_STABLE_Msk           (0x1UL << CPM_OCSR_RTC32K_STABLE_Pos)
#define CPM_OCSR_RTC32K_STABLE               CPM_OCSR_RTC32K_STABLE_Msk

/* [12] OSCEXT_STABLE */
#define CPM_OCSR_OSCEXT_STABLE_Pos           (12U)
#define CPM_OCSR_OSCEXT_STABLE_Msk           (0x1UL << CPM_OCSR_OSCEXT_STABLE_Pos)
#define CPM_OCSR_OSCEXT_STABLE               CPM_OCSR_OSCEXT_STABLE_Msk

/* [11] OSC400M_STABLE */
#define CPM_OCSR_OSC400M_STABLE_Pos          (11U)
#define CPM_OCSR_OSC400M_STABLE_Msk          (0x1UL << CPM_OCSR_OSC400M_STABLE_Pos)
#define CPM_OCSR_OSC400M_STABLE              CPM_OCSR_OSC400M_STABLE_Msk

/* [10] USBPHY240M_STABLE */
#define CPM_OCSR_USBPHY240M_STABLE_Pos       (10U)
#define CPM_OCSR_USBPHY240M_STABLE_Msk       (0x1UL << CPM_OCSR_USBPHY240M_STABLE_Pos)
#define CPM_OCSR_USBPHY240M_STABLE           CPM_OCSR_USBPHY240M_STABLE_Msk

/* [9] PMU128K_STABLE */
#define CPM_OCSR_PMU128K_STABLE_Pos          (9U)
#define CPM_OCSR_PMU128K_STABLE_Msk          (0x1UL << CPM_OCSR_PMU128K_STABLE_Pos)
#define CPM_OCSR_PMU128K_STABLE              CPM_OCSR_PMU128K_STABLE_Msk

/* [8] OSC8M_STABLE */
#define CPM_OCSR_OSC8M_STABLE_Pos            (8U)
#define CPM_OCSR_OSC8M_STABLE_Msk            (0x1UL << CPM_OCSR_OSC8M_STABLE_Pos)
#define CPM_OCSR_OSC8M_STABLE                CPM_OCSR_OSC8M_STABLE_Msk

/* [7] PLLNFC_EN */
#define CPM_OCSR_PLLNFC_EN_Pos               (7U)
#define CPM_OCSR_PLLNFC_EN_Msk               (0x1UL << CPM_OCSR_PLLNFC_EN_Pos)
#define CPM_OCSR_PLLNFC_EN                   CPM_OCSR_PLLNFC_EN_Msk

/* [6] PMU2K_EN */
#define CPM_OCSR_PMU2K_EN_Pos                (6U)
#define CPM_OCSR_PMU2K_EN_Msk                (0x1UL << CPM_OCSR_PMU2K_EN_Pos)
#define CPM_OCSR_PMU2K_EN                    CPM_OCSR_PMU2K_EN_Msk

/* [5] RTC32K_EN */
#define CPM_OCSR_RTC32K_EN_Pos               (5U)
#define CPM_OCSR_RTC32K_EN_Msk               (0x1UL << CPM_OCSR_RTC32K_EN_Pos)
#define CPM_OCSR_RTC32K_EN                   CPM_OCSR_RTC32K_EN_Msk

/* [4] OSCEXT_EN */
#define CPM_OCSR_OSCEXT_EN_Pos               (4U)
#define CPM_OCSR_OSCEXT_EN_Msk               (0x1UL << CPM_OCSR_OSCEXT_EN_Pos)
#define CPM_OCSR_OSCEXT_EN                   CPM_OCSR_OSCEXT_EN_Msk

/* [3] OSC400M_EN */
#define CPM_OCSR_OSC400M_EN_Pos              (3U)
#define CPM_OCSR_OSC400M_EN_Msk              (0x1UL << CPM_OCSR_OSC400M_EN_Pos)
#define CPM_OCSR_OSC400M_EN                  CPM_OCSR_OSC400M_EN_Msk

/* [2] USBPHY240M_EN */
#define CPM_OCSR_USBPHY240M_EN_Pos           (2U)
#define CPM_OCSR_USBPHY240M_EN_Msk           (0x1UL << CPM_OCSR_USBPHY240M_EN_Pos)
#define CPM_OCSR_USBPHY240M_EN               CPM_OCSR_USBPHY240M_EN_Msk

/* [1] PMU128K_EN */
#define CPM_OCSR_PMU128K_EN_Pos              (1U)
#define CPM_OCSR_PMU128K_EN_Msk              (0x1UL << CPM_OCSR_PMU128K_EN_Pos)
#define CPM_OCSR_PMU128K_EN                  CPM_OCSR_PMU128K_EN_Msk

/* [0] OSC8M_EN */
#define CPM_OCSR_OSC8M_EN_Pos                (0U)
#define CPM_OCSR_OSC8M_EN_Msk                (0x1UL << CPM_OCSR_OSC8M_EN_Pos)
#define CPM_OCSR_OSC8M_EN                    CPM_OCSR_OSC8M_EN_Msk

/*******************  Bits definition for CPM_CSWCFGR register  *****************/

/* [31:28] CLKOUT_SEL_ST[3:0] */
#define CPM_CSWCFGR_CLKOUT_SEL_ST_Pos        (28U)
#define CPM_CSWCFGR_CLKOUT_SEL_ST_Msk        (0xFUL << CPM_CSWCFGR_CLKOUT_SEL_ST_Pos)   /*!< 0xF0000000 */
#define CPM_CSWCFGR_CLKOUT_SEL_ST            CPM_CSWCFGR_CLKOUT_SEL_ST_Msk
#define CPM_CSWCFGR_CLKOUT_SEL_ST_SYS        (0x1UL << CPM_CSWCFGR_CLKOUT_SEL_ST_Pos)   /*!< 0x10000000 */
#define CPM_CSWCFGR_CLKOUT_SEL_ST_ARITH      (0x2UL << CPM_CSWCFGR_CLKOUT_SEL_ST_Pos)   /*!< 0x20000000 */
#define CPM_CSWCFGR_CLKOUT_SEL_ST_PLLNFC     (0x4UL << CPM_CSWCFGR_CLKOUT_SEL_ST_Pos)   /*!< 0x40000000 */
#define CPM_CSWCFGR_CLKOUT_SEL_ST_OSCL       (0x8UL << CPM_CSWCFGR_CLKOUT_SEL_ST_Pos)   /*!< 0x80000000 */

/* [25:24] CLKOUT_SEL[1:0] */
#define CPM_CSWCFGR_CLKOUT_SEL_Pos           (24U)
#define CPM_CSWCFGR_CLKOUT_SEL_Msk           (0x3UL << CPM_CSWCFGR_CLKOUT_SEL_Pos)      /*!< 0x03000000 */
#define CPM_CSWCFGR_CLKOUT_SEL               CPM_CSWCFGR_CLKOUT_SEL_Msk
#define CPM_CSWCFGR_CLKOUT_SEL_SYS           (0x0UL << CPM_CSWCFGR_CLKOUT_SEL_Pos)      /*!< 0x00000000 */
#define CPM_CSWCFGR_CLKOUT_SEL_ARITH         (0x1UL << CPM_CSWCFGR_CLKOUT_SEL_Pos)      /*!< 0x01000000 */
#define CPM_CSWCFGR_CLKOUT_SEL_PLLNFC        (0x2UL << CPM_CSWCFGR_CLKOUT_SEL_Pos)      /*!< 0x02000000 */
#define CPM_CSWCFGR_CLKOUT_SEL_OSCL          (0x3UL << CPM_CSWCFGR_CLKOUT_SEL_Pos)      /*!< 0x03000000 */

/* [21:20] OSCL_SEL_ST[1:0] */
#define CPM_CSWCFGR_OSCL_SEL_ST_Pos          (20U)
#define CPM_CSWCFGR_OSCL_SEL_ST_Msk          (0x3UL << CPM_CSWCFGR_OSCL_SEL_ST_Pos)     /*!< 0x00300000 */
#define CPM_CSWCFGR_OSCL_SEL_ST              CPM_CSWCFGR_OSCL_SEL_ST_Msk
#define CPM_CSWCFGR_OSCL_SEL_ST_PMU128K      (0x1UL << CPM_CSWCFGR_OSCL_SEL_ST_Pos)     /*!< 0x00100000 */
#define CPM_CSWCFGR_OSCL_SEL_ST_RTC32K       (0x2UL << CPM_CSWCFGR_OSCL_SEL_ST_Pos)     /*!< 0x00200000 */

/* [9:8] SYS_SEL_ST[1:0] */
#define CPM_CSWCFGR_SYS_SEL_ST_Pos           (8U)
#define CPM_CSWCFGR_SYS_SEL_ST_Msk           (0x3UL << CPM_CSWCFGR_SYS_SEL_ST_Pos)      /*!< 0x00000300 */
#define CPM_CSWCFGR_SYS_SEL_ST               CPM_CSWCFGR_SYS_SEL_ST_Msk
#define CPM_CSWCFGR_SYS_SEL_ST_OSC8M         (0x1UL << CPM_CSWCFGR_SYS_SEL_ST_Pos)      /*!< 0x00000100 */
#define CPM_CSWCFGR_SYS_SEL_ST_OSC400M       (0x2UL << CPM_CSWCFGR_SYS_SEL_ST_Pos)      /*!< 0x00000200 */

/* [6] OSCL_SEL */
#define CPM_CSWCFGR_OSCL_SEL_Pos             (6U)
#define CPM_CSWCFGR_OSCL_SEL_Msk             (0x1UL << CPM_CSWCFGR_OSCL_SEL_Pos)        /*!< 0x00000040 */
#define CPM_CSWCFGR_OSCL_SEL                 CPM_CSWCFGR_OSCL_SEL_Msk
#define CPM_CSWCFGR_OSCL_SEL_PMU128K         (0x0UL << CPM_CSWCFGR_OSCL_SEL_Pos)        /*!< 0x00000000 */
#define CPM_CSWCFGR_OSCL_SEL_RTC32K          (0x1UL << CPM_CSWCFGR_OSCL_SEL_Pos)        /*!< 0x00000040 */

/* [0] SYS_SEL */
#define CPM_CSWCFGR_SYS_SEL_Pos              (0U)
#define CPM_CSWCFGR_SYS_SEL_Msk              (0x1UL << CPM_CSWCFGR_SYS_SEL_Pos)     /*!< 0x00000001 */
#define CPM_CSWCFGR_SYS_SEL                  CPM_CSWCFGR_SYS_SEL_Msk
#define CPM_CSWCFGR_SYS_SEL_OSC8M            (0x0UL << CPM_CSWCFGR_SYS_SEL_Pos)     /*!< 0x00000000 */
#define CPM_CSWCFGR_SYS_SEL_OSC400M          (0x1UL << CPM_CSWCFGR_SYS_SEL_Pos)     /*!< 0x00000001 */

/*******************  Bits definition for CPM_VCCCTMR register  *****************/
/* [31:30] CORE_TEST_KEY[1:0] */
#define CPM_VCCCTMR_CORE_TEST_KEY_Pos        (30U)
#define CPM_VCCCTMR_CORE_TEST_KEY_Msk        (0x3UL << CPM_VCCCTMR_CORE_TEST_KEY_Pos)
#define CPM_VCCCTMR_CORE_TEST_KEY            CPM_VCCCTMR_CORE_TEST_KEY_Msk
#define CPM_VCCCTMR_CORE_TEST_KEY_00         (0x0UL << CPM_VCCCTMR_CORE_TEST_KEY_Pos)
#define CPM_VCCCTMR_CORE_TEST_KEY_01         (0x1UL << CPM_VCCCTMR_CORE_TEST_KEY_Pos)
#define CPM_VCCCTMR_CORE_TEST_KEY_10         (0x2UL << CPM_VCCCTMR_CORE_TEST_KEY_Pos)
#define CPM_VCCCTMR_CORE_TEST_KEY_11         (0x3UL << CPM_VCCCTMR_CORE_TEST_KEY_Pos)

/* [29] OVERWR_CSWCFGR_TRIM */
#define CPM_VCCCTMR_OVERWR_CSWCFGR_TRIM_Pos  (29U)
#define CPM_VCCCTMR_OVERWR_CSWCFGR_TRIM_Msk  (0x1UL << CPM_VCCCTMR_OVERWR_CSWCFGR_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_CSWCFGR_TRIM      CPM_VCCCTMR_OVERWR_CSWCFGR_TRIM_Msk

/* [28] OVERWR_RTC_TRIM */
#define CPM_VCCCTMR_OVERWR_RTC_TRIM_Pos      (28U)
#define CPM_VCCCTMR_OVERWR_RTC_TRIM_Msk      (0x1UL << CPM_VCCCTMR_OVERWR_RTC_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_RTC_TRIM          CPM_VCCCTMR_OVERWR_RTC_TRIM_Msk

/* [26] OVERWR_RTC_STABLE_TRIM */
#define CPM_VCCCTMR_OVERWR_RTC_STABLE_TRIM_Pos  (26U)
#define CPM_VCCCTMR_OVERWR_RTC_STABLE_TRIM_Msk  (0x1UL << CPM_VCCCTMR_OVERWR_RTC_STABLE_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_RTC_STABLE_TRIM      CPM_VCCCTMR_OVERWR_RTC_STABLE_TRIM_Msk

/* [24] OVERWR_CARDLDO_TRIM */
#define CPM_VCCCTMR_OVERWR_CARDLDO_TRIM_Pos  (24U)
#define CPM_VCCCTMR_OVERWR_CARDLDO_TRIM_Msk  (0x1UL << CPM_VCCCTMR_OVERWR_CARDLDO_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_CARDLDO_TRIM      CPM_VCCCTMR_OVERWR_CARDLDO_TRIM_Msk

/* [23] OVERWR_VCC_TRIM */
#define CPM_VCCCTMR_OVERWR_VCC_TRIM_Pos      (23U)
#define CPM_VCCCTMR_OVERWR_VCC_TRIM_Msk      (0x1UL << CPM_VCCCTMR_OVERWR_VCC_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_VCC_TRIM          CPM_VCCCTMR_OVERWR_VCC_TRIM_Msk

/* [22] OVERWR_LVD_TRIM */
#define CPM_VCCCTMR_OVERWR_LVD_TRIM_Pos      (22U)
#define CPM_VCCCTMR_OVERWR_LVD_TRIM_Msk      (0x1UL << CPM_VCCCTMR_OVERWR_LVD_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_LVD_TRIM          CPM_VCCCTMR_OVERWR_LVD_TRIM_Msk

/* [21] OVERWR_VREF_TRIM */
#define CPM_VCCCTMR_OVERWR_VREF_TRIM_Pos     (21U)
#define CPM_VCCCTMR_OVERWR_VREF_TRIM_Msk     (0x1UL << CPM_VCCCTMR_OVERWR_VREF_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_VREF_TRIM         CPM_VCCCTMR_OVERWR_VREF_TRIM_Msk

/* [20] OVERWR_OSC8M_TRIM */
#define CPM_VCCCTMR_OVERWR_OSC8M_TRIM_Pos    (20U)
#define CPM_VCCCTMR_OVERWR_OSC8M_TRIM_Msk    (0x1UL << CPM_VCCCTMR_OVERWR_OSC8M_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_OSC8M_TRIM        CPM_VCCCTMR_OVERWR_OSC8M_TRIM_Msk

/* [19] OVERWR_OSC400M_TRIM */
#define CPM_VCCCTMR_OVERWR_OSC400M_TRIM_Pos  (19U)
#define CPM_VCCCTMR_OVERWR_OSC400M_TRIM_Msk  (0x1UL << CPM_VCCCTMR_OVERWR_OSC400M_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_OSC400M_TRIM      CPM_VCCCTMR_OVERWR_OSC400M_TRIM_Msk

/* [18] OVERWR_OSCL_STABLE_TRIM */
#define CPM_VCCCTMR_OVERWR_OSCL_STABLE_TRIM_Pos (18U)
#define CPM_VCCCTMR_OVERWR_OSCL_STABLE_TRIM_Msk (0x1UL << CPM_VCCCTMR_OVERWR_OSCL_STABLE_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_OSCL_STABLE_TRIM     CPM_VCCCTMR_OVERWR_OSCL_STABLE_TRIM_Msk

/* [17] OVERWR_OSCH_STABLE_TRIM */
#define CPM_VCCCTMR_OVERWR_OSCH_STABLE_TRIM_Pos (17U)
#define CPM_VCCCTMR_OVERWR_OSCH_STABLE_TRIM_Msk (0x1UL << CPM_VCCCTMR_OVERWR_OSCH_STABLE_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_OSCH_STABLE_TRIM     CPM_VCCCTMR_OVERWR_OSCH_STABLE_TRIM_Msk

/* [16] OVERWR_OSCE_STABLE_TRIM */
#define CPM_VCCCTMR_OVERWR_OSCE_STABLE_TRIM_Pos (16U)
#define CPM_VCCCTMR_OVERWR_OSCE_STABLE_TRIM_Msk (0x1UL << CPM_VCCCTMR_OVERWR_OSCE_STABLE_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_OSCE_STABLE_TRIM     CPM_VCCCTMR_OVERWR_OSCE_STABLE_TRIM_Msk

/* [13] OVERWR_ARITHCGT_TRIM */
#define CPM_VCCCTMR_OVERWR_ARITHCGT_TRIM_Pos (13U)
#define CPM_VCCCTMR_OVERWR_ARITHCGT_TRIM_Msk (0x1UL << CPM_VCCCTMR_OVERWR_ARITHCGT_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_ARITHCGT_TRIM     CPM_VCCCTMR_OVERWR_ARITHCGT_TRIM_Msk

/* [11] OVERWR_SCDIV_TRIM */
#define CPM_VCCCTMR_OVERWR_SCDIV_TRIM_Pos    (11U)
#define CPM_VCCCTMR_OVERWR_SCDIV_TRIM_Msk    (0x1UL << CPM_VCCCTMR_OVERWR_SCDIV_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_SCDIV_TRIM        CPM_VCCCTMR_OVERWR_SCDIV_TRIM_Msk

/* [10] OVERWR_PCDIV_TRIM */
#define CPM_VCCCTMR_OVERWR_PCDIV_TRIM_Pos    (10U)
#define CPM_VCCCTMR_OVERWR_PCDIV_TRIM_Msk    (0x1UL << CPM_VCCCTMR_OVERWR_PCDIV_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_PCDIV_TRIM        CPM_VCCCTMR_OVERWR_PCDIV_TRIM_Msk

/* [9] OVERWR_OCSR_TRIM */
#define CPM_VCCCTMR_OVERWR_OCSR_TRIM_Pos     (9U)
#define CPM_VCCCTMR_OVERWR_OCSR_TRIM_Msk     (0x1UL << CPM_VCCCTMR_OVERWR_OCSR_TRIM_Pos)
#define CPM_VCCCTMR_OVERWR_OCSR_TRIM         CPM_VCCCTMR_OVERWR_OCSR_TRIM_Msk

/* [8] OFF_MODE_WK */
#define CPM_VCCCTMR_OFF_MODE_WK_Pos          (8U)
#define CPM_VCCCTMR_OFF_MODE_WK_Msk          (0x1UL << CPM_VCCCTMR_OFF_MODE_WK_Pos)
#define CPM_VCCCTMR_OFF_MODE_WK              CPM_VCCCTMR_OFF_MODE_WK_Msk

/* [7] CPU_CORE_TEST_EN */
#define CPM_VCCCTMR_CPU_CORE_TEST_EN_Pos     (7U)
#define CPM_VCCCTMR_CPU_CORE_TEST_EN_Msk     (0x1UL << CPM_VCCCTMR_CPU_CORE_TEST_EN_Pos)
#define CPM_VCCCTMR_CPU_CORE_TEST_EN         CPM_VCCCTMR_CPU_CORE_TEST_EN_Msk

/* [3] SOFT_POR */
#define CPM_VCCCTMR_SOFT_POR_Pos             (3U)
#define CPM_VCCCTMR_SOFT_POR_Msk             (0x1UL << CPM_VCCCTMR_SOFT_POR_Pos)
#define CPM_VCCCTMR_SOFT_POR                 CPM_VCCCTMR_SOFT_POR_Msk

/* [2] OFF_MODE2 */
#define CPM_VCCCTMR_OFF_MODE2_Pos            (2U)
#define CPM_VCCCTMR_OFF_MODE2_Msk            (0x1UL << CPM_VCCCTMR_OFF_MODE2_Pos)
#define CPM_VCCCTMR_OFF_MODE2                CPM_VCCCTMR_OFF_MODE2_Msk

/* [0] EN_LP */
#define CPM_VCCCTMR_EN_LP_Pos                (0U)
#define CPM_VCCCTMR_EN_LP_Msk                (0x1UL << CPM_VCCCTMR_EN_LP_Pos)
#define CPM_VCCCTMR_EN_LP                    CPM_VCCCTMR_EN_LP_Msk

/*******************  Bits definition for CPM_O400MTRIMR register  ************/
/* [16:0] OSC400M_TRIM[16:0] */
#define CPM_O400MTRIMR_OSC400M_TRIM_Pos      (0U)
#define CPM_O400MTRIMR_OSC400M_TRIM_Msk      (0x1FFFFUL << CPM_O400MTRIMR_OSC400M_TRIM_Pos)
#define CPM_O400MTRIMR_OSC400M_TRIM          CPM_O400MTRIMR_OSC400M_TRIM_Msk


/******************************************************************************/
/*                                                                            */
/*                        Watchdog Module                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bits definition for WDT_WCR register  *******************/
/* [3] WAIT */
#define WDT_WCR_WAIT_Pos        (3U)
#define WDT_WCR_WAIT_Msk        (0x1U << WDT_WCR_WAIT_Pos)
#define WDT_WCR_WAIT            WDT_WCR_WAIT_Msk

/* [2] DOZE */
#define WDT_WCR_DOZE_Pos        (2U)
#define WDT_WCR_DOZE_Msk        (0x1U << WDT_WCR_DOZE_Pos)
#define WDT_WCR_DOZE            WDT_WCR_DOZE_Msk

/* [1] DBG */
#define WDT_WCR_DBG_Pos         (1U)
#define WDT_WCR_DBG_Msk         (0x1U << WDT_WCR_DBG_Pos)
#define WDT_WCR_DBG             WDT_WCR_DBG_Msk

/* [0] EN */
#define WDT_WCR_EN_Pos          (0U)
#define WDT_WCR_EN_Msk          (0x1U << WDT_WCR_EN_Pos)
#define WDT_WCR_EN              WDT_WCR_EN_Msk

/* Convenience mask for mode bits (DBG/DOZE/WAIT) */
#define WDT_WCR_MODE_Msk        (WDT_WCR_DBG_Msk | WDT_WCR_DOZE_Msk | WDT_WCR_WAIT_Msk)

/*******************  Bits definition for WDT_WMR register  *******************/
/* [15:0] WM */
#define WDT_WMR_WM_Pos          (0U)
#define WDT_WMR_WM_Msk          (0xFFFFUL << WDT_WMR_WM_Pos)
#define WDT_WMR_WM              WDT_WMR_WM_Msk

/*******************  Bits definition for WDT_WCNTR register ******************/
/* [15:0] WC */
#define WDT_WCNTR_WC_Pos        (0U)
#define WDT_WCNTR_WC_Msk        (0xFFFFUL << WDT_WCNTR_WC_Pos)
#define WDT_WCNTR_WC            WDT_WCNTR_WC_Msk

/*******************  Bits definition for WDT_WSR register  *******************/
/* [15:0] WS */
#define WDT_WSR_WS_Pos          (0U)
#define WDT_WSR_WS_Msk          (0xFFFFUL << WDT_WSR_WS_Pos)
#define WDT_WSR_WS              WDT_WSR_WS_Msk


#ifdef __cplusplus
}

#endif

#endif /* __FT9001_H */
