/** ================================================================
 * @file    external_drivers/ssd1306.c
 *
 * @brief   Source file of the SSD 1306 driver (thanks to chatGPT)
 *
 * @date 	24-09-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */
/*
 * -----------------------------------------------------------------
 * Includes
 * -----------------------------------------------------------------
 */
#include "external_drivers/ssd1306.h"

// Dependencies
#include "stm32f3xx_hal.h"
#include <stdint.h>

// Global variables and buffer
static I2C_HandleTypeDef *ssd1306_i2c;
static uint8_t ssd1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

/*
 * -----------------------------------------------------------------
 * Helpers
 * -----------------------------------------------------------------
 */
static void __write_command(uint8_t cmd)
{
    uint8_t d[2] = {0x00, cmd};
    HAL_I2C_Master_Transmit(ssd1306_i2c, SSD1306_I2C_ADDR, d, 2, HAL_MAX_DELAY);
}

static void __write_data(uint8_t *data, size_t size) {
    uint8_t buffer[size + 1];
    buffer[0] = 0x40; // 0x40 = Co = 0, D/C# = 1
    memcpy(&buffer[1], data, size);
    HAL_I2C_Master_Transmit(ssd1306_i2c, SSD1306_I2C_ADDR, buffer, size + 1, HAL_MAX_DELAY);
}

/*
 * -----------------------------------------------------------------
 * Public API
 * -----------------------------------------------------------------
 */
void SSD1306_Init(I2C_HandleTypeDef *hi2c) {
    ssd1306_i2c = hi2c;

    HAL_Delay(100); // power-on delay

    // Init sequence
    __write_command(0xAE); 							// Display off
    __write_command(0x20); 							// Set Memory Addressing Mode
    __write_command(0x00); 							// Horizontal addressing mode
    __write_command(0xB0); 							// Page start address
    __write_command(0xC8); 							// COM scan direction remapped
    __write_command(0x00); 							// Low column
    __write_command(0x10); 							// High column
    __write_command(0x40); 							// Start line address
    __write_command(0x81); __write_command(0x7F); 	// Contrast
    __write_command(0xA1); 							// Segment remap
    __write_command(0xA6); 							// Normal display
    __write_command(0xA8); __write_command(0x3F); 	// Multiplex ratio
    __write_command(0xA4); 							// Display follows RAM
    __write_command(0xD3); __write_command(0x00); 	// Display offset
    __write_command(0xD5); __write_command(0x80); 	// Clock divide
    __write_command(0xD9); __write_command(0xF1); 	// Pre-charge
    __write_command(0xDA); __write_command(0x12); 	// COM pins
    __write_command(0xDB); __write_command(0x40); 	// VCOM detect
    __write_command(0x8D); __write_command(0x14); 	// Charge pump
    __write_command(0xAF); 							// Display ON

    SSD1306_Fill(false);							// Clear memory
    SSD1306_UpdateScreen();							// Update screen to apply the emptied buffer
}

void SSD1306_Fill(bool color) {
    memset(ssd1306_Buffer, (color ? 0xFF : 0x00), sizeof(ssd1306_Buffer));
}


void SSD1306_DrawPixel(uint8_t x, uint8_t y, bool color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;

    if (color) {
        ssd1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    } else {
        ssd1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}


void SSD1306_SendBuffer(const uint8_t *buf)
{
	uint8_t py = 63;

	for (int col = 0; col < 128; col ++)
	{
		// Fetch data
		int tmp = buf[4 * col]  << 24 | buf[(4 * col) + 1]  << 16 | buf[(4 * col) + 2]  << 8 | buf[(4 * col) + 3];

		for (int row = 0; row < 32; row ++)
		{
			// for each buffer pixel, set it's value
			if ((tmp & (1 << row)) != 0)
			{
				SSD1306_DrawPixel(col, py, true);
				py -= 2;
			}
			else
			{
				SSD1306_DrawPixel(col, py, false);
				py -= 2;
			}
		}
		// reset the py counter back
		py = 63;
	}

    return;
}


// Last function, the only to make massives I2C calls.
void SSD1306_UpdateScreen(void) {
    for (uint8_t page = 0; page < 8; page++) {

        __write_command(0xB0 + page);
        __write_command(0x00);
        __write_command(0x10);

        __write_data(&ssd1306_Buffer[SSD1306_WIDTH * page], SSD1306_WIDTH);
    }
}

