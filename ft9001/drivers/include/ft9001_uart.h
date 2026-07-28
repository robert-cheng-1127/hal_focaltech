/*
 * Copyright (c) 2026, FocalTech Systems CO.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file    ft9001_uart.h
 * @brief   FT9001 UART (SCI) register operations.
 *
 * The block transmits from a TX FIFO and receives into an RX FIFO, both enabled
 * through SCIFCR. Status lives in SCIFSR (fill levels, transmission complete)
 * and SCIFSR2 (receive errors, write-one-to-clear). Interrupt sources are
 * unmasked in SCIFCR2.
 *
 * Waiting is left to the caller: these routines report state and never block.
 */

#ifndef FT9001_UART_H_
#define FT9001_UART_H_

#include <stdbool.h>
#include <stdint.h>

#include "ft9001.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Parity mode. */
enum ft9001_uart_parity {
	FT9001_UART_PARITY_NONE = 0,
	FT9001_UART_PARITY_EVEN,
	FT9001_UART_PARITY_ODD,
};

/** @brief Data bits per frame. */
enum ft9001_uart_data_bits {
	/** 10-bit frame: start, 8 data, stop. */
	FT9001_UART_DATA_BITS_8 = 0,
	/** 11-bit frame: start, 9 data, stop. */
	FT9001_UART_DATA_BITS_9,
};

/** @brief Stop bits. */
enum ft9001_uart_stop_bits {
	FT9001_UART_STOP_BITS_1 = 0,
	/**
	 * Approximated by holding the 9th data bit high, so it requires
	 * @ref FT9001_UART_DATA_BITS_9 and costs that bit.
	 */
	FT9001_UART_STOP_BITS_2,
};

/** @brief Frame format and line rate. */
struct ft9001_uart_config {
	uint32_t baudrate;
	enum ft9001_uart_parity parity;
	enum ft9001_uart_data_bits data_bits;
	enum ft9001_uart_stop_bits stop_bits;
};

/** @brief TX FIFO below its trigger level. */
#define FT9001_UART_INT_TX         UART_SCIFCR2_TXFIE_Msk
/** @brief RX FIFO at or above its trigger level. */
#define FT9001_UART_INT_RX         UART_SCIFCR2_RXFIE_Msk
/** @brief RX idle for longer than the timeout counter. */
#define FT9001_UART_INT_RX_TIMEOUT UART_SCIFCR2_RXFTOIE_Msk
/** @brief RX FIFO overrun. */
#define FT9001_UART_INT_RX_OVERRUN UART_SCIFCR2_RXORIE_Msk

/** @brief Receive overrun. */
#define FT9001_UART_ERR_OVERRUN UART_SCIFSR2_FOR_Msk
/** @brief Noise detected on a received frame. */
#define FT9001_UART_ERR_NOISE   UART_SCIFSR2_FNF_Msk
/** @brief Framing error. */
#define FT9001_UART_ERR_FRAMING UART_SCIFSR2_FFE_Msk
/** @brief Parity error. */
#define FT9001_UART_ERR_PARITY  UART_SCIFSR2_FPF_Msk
/** @brief Every error flag, for use when clearing. */
#define FT9001_UART_ERR_ALL     UART_SCIFSR2_ERR_Msk

/** @brief Enable the transmitter and receiver. */
static inline void ft9001_uart_enable(UART_TypeDef *inst)
{
	FT9001_SET_BIT(inst->SCICR2, (uint8_t)(UART_SCICR2_TE | UART_SCICR2_RE));
}

/** @brief Disable the transmitter and receiver. */
static inline void ft9001_uart_disable(UART_TypeDef *inst)
{
	FT9001_CLEAR_BIT(inst->SCICR2, (uint8_t)(UART_SCICR2_TE_Msk | UART_SCICR2_RE_Msk));
}

/** @brief Read the FIFO status register (SCIFSR). */
static inline uint8_t ft9001_uart_status_get(UART_TypeDef *inst)
{
	return inst->SCIFSR;
}

/** @brief No received bytes are waiting. */
static inline bool ft9001_uart_rx_fifo_empty(UART_TypeDef *inst)
{
	return FT9001_READ_BIT(inst->SCIFSR, (uint8_t)UART_SCIFSR_REMPTY_Msk) != 0U;
}

/** @brief The RX FIFO has reached its trigger level. */
static inline bool ft9001_uart_rx_trigger_reached(UART_TypeDef *inst)
{
	return FT9001_READ_BIT(inst->SCIFSR, (uint8_t)UART_SCIFSR_RFTS_Msk) != 0U;
}

/** @brief The TX FIFO cannot accept another byte. */
static inline bool ft9001_uart_tx_fifo_full(UART_TypeDef *inst)
{
	return FT9001_READ_BIT(inst->SCIFSR, (uint8_t)UART_SCIFSR_TFULL_Msk) != 0U;
}

/** @brief The TX FIFO has drained and the last frame has left the shifter. */
static inline bool ft9001_uart_tx_complete(UART_TypeDef *inst)
{
	uint8_t fsr = inst->SCIFSR;

	return (fsr & (uint8_t)(UART_SCIFSR_TEMPTY_Msk | UART_SCIFSR_FTC_Msk)) ==
	       (uint8_t)(UART_SCIFSR_TEMPTY_Msk | UART_SCIFSR_FTC_Msk);
}

/** @brief Pop one byte from the RX FIFO. */
static inline uint8_t ft9001_uart_data_get(UART_TypeDef *inst)
{
	return inst->SCIDRL;
}

/** @brief Push one byte into the TX FIFO. */
static inline void ft9001_uart_data_set(UART_TypeDef *inst, uint8_t data)
{
	inst->SCIDRL = data;
}

/** @brief Unmask the given FT9001_UART_INT_* sources. */
static inline void ft9001_uart_int_enable(UART_TypeDef *inst, uint8_t mask)
{
	FT9001_SET_BIT(inst->SCIFCR2, mask);
}

/** @brief Mask the given FT9001_UART_INT_* sources. */
static inline void ft9001_uart_int_disable(UART_TypeDef *inst, uint8_t mask)
{
	FT9001_CLEAR_BIT(inst->SCIFCR2, mask);
}

/** @brief Read back which FT9001_UART_INT_* sources are unmasked. */
static inline uint8_t ft9001_uart_int_enabled_get(UART_TypeDef *inst)
{
	return inst->SCIFCR2;
}

/** @brief Read the FT9001_UART_ERR_* flags. */
static inline uint8_t ft9001_uart_error_flags_get(UART_TypeDef *inst)
{
	return (uint8_t)(inst->SCIFSR2 & (uint8_t)UART_SCIFSR2_ERR_Msk);
}

/** @brief Clear the given FT9001_UART_ERR_* flags. */
static inline void ft9001_uart_error_flags_clear(UART_TypeDef *inst, uint8_t mask)
{
	inst->SCIFSR2 = (uint8_t)(mask & (uint8_t)UART_SCIFSR2_ERR_Msk);
}

/** @brief Discard whatever is queued in both FIFOs. */
static inline void ft9001_uart_fifo_clear(UART_TypeDef *inst)
{
	FT9001_SET_BIT(inst->SCIFCR2, (uint8_t)(UART_SCIFCR2_RXFCLR | UART_SCIFCR2_TXFCLR));
}

/**
 * @brief Program the baud rate divisor.
 *
 * @param  pclk_hz  Peripheral clock feeding the block, up to 536 MHz.
 * @retval 0        Divisor programmed.
 * @retval -EINVAL  Zero or out-of-range clock, zero baud rate, or a divisor the
 *                  register cannot hold.
 */
int ft9001_uart_baudrate_set(UART_TypeDef *inst, uint32_t pclk_hz, uint32_t baudrate);

/**
 * @brief Apply a full configuration: frame format, baud rate and FIFOs.
 *
 * Leaves the transmitter and receiver enabled and both FIFOs empty, with every
 * interrupt source still masked.
 *
 * @param  pclk_hz  Peripheral clock feeding the block.
 * @retval 0        Applied.
 * @retval -EINVAL  Unrecognised frame format, or a baud rate out of reach.
 * @retval -ENOTSUP Two stop bits without nine data bits.
 */
int ft9001_uart_configure(UART_TypeDef *inst, const struct ft9001_uart_config *cfg,
			  uint32_t pclk_hz);

#ifdef __cplusplus
}
#endif

#endif /* FT9001_UART_H_ */
