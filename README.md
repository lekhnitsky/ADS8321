ADS8321 is a lightweight, STM32 HAL-based C library for communicating with the ADS8321 16-bit, single-channel SAR ADC over SPI. It offers a simple handle-based API—initialization, raw 16-bit sample reads, and voltage conversion in either ±2.5 V or 0–5 V ranges—while handling all of the low-level SPI transactions, chip-select toggling, bit-shifting and two’s-complement decoding internally. You can apply a user-defined offset calibration by setting the `cc` field in your `ADS8321_HandleTypeDef`. For diagnostics, defining `DEBUG_ADS8321` before including the library enables rich `DEBUG_PRINT()` logs of each SPI transfer and conversion step; omitting that macro yields a minimal, production-ready binary. Integration is as simple as copying `ads8321.c` and `ads8321.h` into your project alongside your HAL SPI and GPIO setup, declaring and configuring an `ADS8321_HandleTypeDef`, calling

```c
ADS8321_Init(&hads8321);
```

and then using

```c
uint16_t raw;
float voltage;
ADS8321_Read(&hads8321, &raw);                     // raw 16-bit value  
ADS8321_ReadVoltage(&hads8321, mode, &voltage);    // converted volts  
```

All functions return standard `HAL_StatusTypeDef` codes for straightforward error checking. The library has no dynamic memory or RTOS dependencies and is safe to call from both main and interrupt contexts. 
Distributed under the MIT License—feel free to copy, modify and integrate it in your STM32CubeIDE or Makefile-based projects.
