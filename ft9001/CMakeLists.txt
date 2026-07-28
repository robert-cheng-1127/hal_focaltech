# Copyright (c) 2025-2026, FocalTech Systems CO.,Ltd
# SPDX-License-Identifier: Apache-2.0

set(HAL_FT9001_ROOT ${CMAKE_CURRENT_LIST_DIR})

zephyr_library()

zephyr_include_directories(
    ${HAL_FT9001_ROOT}/soc
    ${HAL_FT9001_ROOT}/drivers/include
)

zephyr_library_sources(${HAL_FT9001_ROOT}/drivers/src/ft9001_hal.c)

zephyr_library_sources_ifdef(CONFIG_USE_FT9001_HAL_CPM
    ${HAL_FT9001_ROOT}/drivers/src/ft9001_cpm.c
)
zephyr_library_sources_ifdef(CONFIG_USE_FT9001_HAL_CACHE
    ${HAL_FT9001_ROOT}/drivers/src/ft9001_cache.c
)
zephyr_library_sources_ifdef(CONFIG_USE_FT9001_HAL_UART
    ${HAL_FT9001_ROOT}/drivers/src/ft9001_uart.c
)
zephyr_library_sources_ifdef(CONFIG_USE_FT9001_SYSTEM_INIT
    ${HAL_FT9001_ROOT}/soc/system_ft9001.c
)
