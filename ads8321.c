/*
 * @file    ads8321.c
 * @author  lekhnitsky
 * @brief   STM32 HAL-based library implementation for SPI 16-bit ADC ADS8321.
 * @date    2025-03-19
 */

#include "ads8321.h"

// Enable or disable debug mode
//#define DEBUG_ADS8321

#ifdef DEBUG_ADS8321
    #define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) // No operation
#endif

// ADC Reference voltage
#define ADS8321_VREF       2.5f  ///< Reference voltage in volts
#define ADS8321_SPI_TIMEOUT 1     ///< SPI timeout in milliseconds
#define ADS8321_MSB_MASK   0x8000 ///< Mask for MSB check

void ADS8321_Init(ADS8321_HandleTypeDef *hads8321) {
    if (!hads8321 || !hads8321->hspi) {
        DEBUG_PRINT("Error: Invalid handle or SPI pointer in ADS8321_Init\r\n");
        return;
    }

    // Set Chip Select (CS) high initially
    HAL_GPIO_WritePin(hads8321->cs_port, hads8321->cs_pin, GPIO_PIN_SET);
    DEBUG_PRINT("ADS8321 Initialized successfully\r\n");
}

HAL_StatusTypeDef ADS8321_Read(ADS8321_HandleTypeDef *hads8321, uint16_t *value) {
    if (!hads8321 || !value) {
        DEBUG_PRINT("Error: Invalid parameters in ADS8321_Read\r\n");
        return HAL_ERROR;
    }

    uint8_t received_data[3] = {0}; // Buffer for raw SPI data

    HAL_GPIO_WritePin(hads8321->cs_port, hads8321->cs_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Receive(hads8321->hspi, received_data, 3, ADS8321_SPI_TIMEOUT);
    HAL_GPIO_WritePin(hads8321->cs_port, hads8321->cs_pin, GPIO_PIN_SET);

    if (status == HAL_OK) {
        *value = ((((received_data[0] << 8) | received_data[1]) << 6) & 0xFFFF) | (received_data[2] >> 2); //move received data by 6 to left (sampling + first dummy bit)
        //DEBUG_PRINT("Received Data: 0x%02X 0x%02X 0x%02X\r\n", received_data[0], received_data[1], received_data[2]);
        //DEBUG_PRINT("Data: 0x%04X\r\n", *value);
        return HAL_OK;
    } else {
        DEBUG_PRINT("SPI Read Failed!\r\n");
        return HAL_ERROR;
    }
}

HAL_StatusTypeDef ADS8321_ReadVoltage(ADS8321_HandleTypeDef *hads8321, const uint8_t mode, float *voltage) {
    if (!hads8321 || !voltage) {
        DEBUG_PRINT("Error: Invalid parameters in ADS8321_ReadVoltage!\r\n");
        return HAL_ERROR;
    }

    if (mode != 1 && mode != 0 ) {
    	DEBUG_PRINT("Error: Invalid mode in ADS8321_ReadVoltage!\r\n");
		return HAL_ERROR;
    }

    uint16_t raw_data = 0;

    if (ADS8321_Read(hads8321, &raw_data ) != HAL_OK) {
        return HAL_ERROR;
    }

    int16_t result = (int16_t)raw_data;

    if (result & ADS8321_MSB_MASK) { //if MSB=1
		result ^= ADS8321_MSB_MASK;  //then flip it
		result = -(32768 - result);  //and destroy binary two's complement
	}

    *voltage = hads8321->cc + (float)result * (ADS8321_VREF * 2 / 65536.0f);

    if (mode == 0)
    	*voltage += 2.5f;

    //DEBUG_PRINT("Voltage: %.5f V\r\n", *voltage);
    // --- Additional formatted debug output ---
//	{
//		// Build 16-bit binary string
//		char bin_str[17];
//		for (int i = 0; i < 16; i++) {
//			bin_str[15 - i] = (raw_data >> i) & 1 ? '1' : '0';
//		}
//		bin_str[16] = '\0';
//
//		// Aligned output: voltage width 6 w/2 decimals, raw_data width 5
//		printf("U = %6.2f V  |  Raw = %5u  |  Bits = %s\r\n",
//					*voltage, raw_data, bin_str);
//	}
    return HAL_OK;
}
