/*
 * Copyright (c) 2026, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <stdint.h>

#include "ft9001_uart.h"

static int baudrate_div_calc(uint32_t pclk_hz, uint32_t baudrate, uint32_t *div_x64)
{
	uint32_t div;

	if (pclk_hz == 0U || baudrate == 0U) {
		return -EINVAL;
	}

	/* Holding the intermediate in 32 bits caps the clock this can serve. */
	if (pclk_hz > (UINT32_MAX / 8U)) {
		return -EINVAL;
	}

	/* Round the whole fixed-point divisor once before splitting it, so the
	 * fraction carries into the integer part.
	 */
	*div_x64 = (((pclk_hz * 8U) / baudrate) + 1U) / 2U;
	div = *div_x64 >> 6;

	if (div == 0U || div > UINT16_MAX) {
		return -EINVAL;
	}

	return 0;
}

static void baudrate_div_apply(UART_TypeDef *inst, uint32_t div_x64)
{
	uint32_t div = div_x64 >> 6;

	/* The fraction has to reach the block before the integer part. */
	inst->SCIBRDF = (uint8_t)(div_x64 & UART_SCIBRDF_FRAC_Msk);
	inst->SCIBDH = (uint8_t)((div >> 8) & 0xFFU);
	inst->SCIBDL = (uint8_t)(div & 0xFFU);
}

int ft9001_uart_baudrate_set(UART_TypeDef *inst, uint32_t pclk_hz, uint32_t baudrate)
{
	uint32_t div_x64;
	int ret = baudrate_div_calc(pclk_hz, baudrate, &div_x64);

	if (ret != 0) {
		return ret;
	}

	baudrate_div_apply(inst, div_x64);

	return 0;
}

int ft9001_uart_configure(UART_TypeDef *inst, const struct ft9001_uart_config *cfg,
			  uint32_t pclk_hz)
{
	uint32_t div_x64;
	uint8_t cr1 = 0U;
	int ret;

	switch (cfg->data_bits) {
	case FT9001_UART_DATA_BITS_8:
		break;
	case FT9001_UART_DATA_BITS_9:
		cr1 |= (uint8_t)UART_SCICR1_M;
		break;
	default:
		return -EINVAL;
	}

	switch (cfg->parity) {
	case FT9001_UART_PARITY_NONE:
		break;
	case FT9001_UART_PARITY_EVEN:
		cr1 |= (uint8_t)UART_SCICR1_PE;
		break;
	case FT9001_UART_PARITY_ODD:
		cr1 |= (uint8_t)(UART_SCICR1_PE | UART_SCICR1_PT);
		break;
	default:
		return -EINVAL;
	}

	switch (cfg->stop_bits) {
	case FT9001_UART_STOP_BITS_1:
		break;
	case FT9001_UART_STOP_BITS_2:
		if (cfg->data_bits != FT9001_UART_DATA_BITS_9) {
			return -ENOTSUP;
		}
		break;
	default:
		return -EINVAL;
	}

	ret = baudrate_div_calc(pclk_hz, cfg->baudrate, &div_x64);
	if (ret != 0) {
		return ret;
	}

	/* Nothing above reaches the block, so a rejected configuration leaves it
	 * running rather than disabled.
	 */
	inst->SCICR2 = 0U;
	inst->SCIFCR = 0U;
	inst->SCIFCR = (uint8_t)(UART_SCIFCR_RFEN | UART_SCIFCR_TFEN);

	baudrate_div_apply(inst, div_x64);

	inst->SCICR1 = cr1;

	/* Write outright: R8 shares this register and writing it back stalls
	 * the receiver.
	 */
	if (cfg->data_bits == FT9001_UART_DATA_BITS_9) {
		inst->SCIDRH = (cfg->stop_bits == FT9001_UART_STOP_BITS_2)
				       ? (uint8_t)UART_SCIDRH_T8
				       : 0U;
	}

	FT9001_SET_BIT(inst->SCIFCR,
		       (uint8_t)(UART_SCIFCR_RXFLSEL_1_8 | UART_SCIFCR_TXFLSEL_1_8));

	inst->SCIFCR2 = (uint8_t)(UART_SCIFCR2_RXFTOE | UART_SCIFCR2_RXFCLR | UART_SCIFCR2_TXFCLR);
	inst->SCIFSR2 = (uint8_t)UART_SCIFSR2_ERR_Msk;

	ft9001_uart_enable(inst);

	return 0;
}
