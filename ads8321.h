/*
 * @file    ads8321.h
 * @author  lekhnitsky
 * @brief   STM32 HAL-based header for SPI 16-bit ADC ADS8321.
 * @date    2025-03-19
 */

#ifndef INC_ADS8321_H_
#define INC_ADS8321_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>

/**
 * @brief Handle structure for the ADS8321 ADC.
 */
typedef struct {
    SPI_HandleTypeDef *hspi;  ///< SPI handle for SPI communication
    GPIO_TypeDef *cs_port;    ///< GPIO port for Chip Select (CS)
    uint16_t cs_pin;          ///< GPIO pin for Chip Select (CS)
    GPIO_TypeDef *sclk_port;  ///< GPIO port for SPI Clock (SCLK)
    uint16_t sclk_pin;        ///< GPIO pin for SPI Clock (SCLK)
    float cc;				  ///< correction constant
} ADS8321_HandleTypeDef;


/**
 * @brief Initialize the ADS8321 ADC.
 * @param hads8321 Pointer to the ADS8321 handle structure.
 */
void ADS8321_Init(ADS8321_HandleTypeDef *hads8321);

/**
 * @brief Read raw 16-bit value from the ADS8321 ADC.
 * @param hads8321 Pointer to the ADS8321 handle structure.
 * @param value Pointer to store the raw 16-bit value.
 * @return HAL status of the operation.
 */
HAL_StatusTypeDef ADS8321_Read(ADS8321_HandleTypeDef *hads8321, uint16_t *value);

/**
 * @brief Read the converted voltage from the ADS8321 ADC.
 * @param hads8321 Pointer to the ADS8321 handle structure.
 * @param mode = 1 is -2.5V to +2.5V range, 0 is 0V t 5V range
 * @param voltage Pointer to store the voltage value in volts.
 * @return HAL status of the operation.
 */
HAL_StatusTypeDef ADS8321_ReadVoltage(ADS8321_HandleTypeDef *hads8321, uint8_t mode, float *voltage);



#ifdef __cplusplus
}
#endif

#endif /* INC_ADS8321_H_ */
