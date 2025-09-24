/** ================================================================
 * @file    external_drivers/ssd1306.h
 *
 * @brief   Header file to declare functions to control the ssd1306
 * 			oled screen.
 *
 * @date 	24-09-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------
 * Includes
 * -----------------------------------------------------------------
 */
#include "stm32f3xx_hal.h"

#include <stdbool.h>
#include <string.h>

/*
 * -----------------------------------------------------------------
 * Defines (library configuration)
 * -----------------------------------------------------------------
 */
#define SSD1306_I2C_ADDR      (0x3C << 1) 	// default 0x3C, shift for HAL
#define SSD1306_WIDTH         128
#define SSD1306_LOGICAL_H     32   			// Number of user accessible pixels
#define SSD1306_HEIGHT        64			// Number of "real" pixels on the controller.
#define SSD1306_USE_ODD_ROWS  1

/*
 * -----------------------------------------------------------------
 * API
 * -----------------------------------------------------------------
 */
/**
 * 	Initialize the screen for a given I2C handle. Also call configurations commands.
 */
void SSD1306_Init(I2C_HandleTypeDef *hi2c);

/**
 * Take a buffer for the screen helper lib, and send it to the screen.
 */
void SSD1306_SendBuffer(I2C_HandleTypeDef *hi2c, const uint8_t *src);


#ifdef __cplusplus
}
#endif
