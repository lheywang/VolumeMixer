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
 * Fill the screen with the color (bool value).
 */
void SSD1306_Fill(bool color);
/**
 * Write the internal buffer on the screen. Enable way better performances than reading
 * and updating each pixels one by one.
 */
void SSD1306_UpdateScreen(void);

// Drawings primitives :
/**
 *	Draw a pixel on the screen (only applied after SSD1306_UpdateScreen function call)
 */
void SSD1306_DrawPixel(uint8_t x, uint8_t y, bool color);
/**
 * Draw a bitmap on the screen. Used to flush an icon on the screen.
 */
void SSD1306_DrawBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *data);
/**
 * Draw a bitmap on the screen, intepreted as a digit.
 */
void SSD1306_PrintDigit(uint8_t x, uint8_t y, int digit, const uint8_t font[][32]);


#ifdef __cplusplus
}
#endif
