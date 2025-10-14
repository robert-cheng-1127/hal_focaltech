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

#include <stddef.h>
#include <stdint.h>

/* IRQ numbers: define ONLY the core exceptions required by CMSIS. */
typedef enum IRQn {
  NonMaskableInt_IRQn = -14,
  HardFault_IRQn = -13,
  MemoryManagement_IRQn = -12,
  BusFault_IRQn = -11,
  UsageFault_IRQn = -10,
  SVCall_IRQn = -5,
  DebugMonitor_IRQn = -4,
  PendSV_IRQn = -2,
  SysTick_IRQn = -1
} IRQn_Type;

#ifndef __CM4_REV
#define __CM4_REV 0x0100U
#endif

#ifndef __MPU_PRESENT
#define __MPU_PRESENT 1U
#endif

#ifndef __FPU_PRESENT
#define __FPU_PRESENT 1U
#endif

#ifndef __NVIC_PRIO_BITS
#define __NVIC_PRIO_BITS 3U
#endif

#ifndef __Vendor_SysTickConfig
#define __Vendor_SysTickConfig 0U
#endif

#include "cmsis_compiler.h"
#include "core_cm4.h"

#ifdef __cplusplus
}
#endif

#endif /* __FT9001_H */
