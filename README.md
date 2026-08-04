# hal_focaltech

Register-level support library for FocalTech SoCs, packaged as a Zephyr
module.

## Layout

    ft9001/soc/        register maps and the CMSIS system files
    ft9001/drivers/    per-block operations: CPM, WDT, TC, cache, UART

## Integration

Zephyr picks the module up through `zephyr/module.yml`. `HAS_FT9001_HAL`
is enabled by the SoC; the `USE_FT9001_HAL_*` symbols select which blocks
are compiled in, and `USE_FT9001_SYSTEM_INIT` adds the vendor
SystemInit() path for platforms that boot through it.

## License

Apache-2.0, see `LICENSE`.
