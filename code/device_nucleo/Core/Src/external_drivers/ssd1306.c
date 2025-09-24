#include "external_drivers/ssd1306.h"
#include <stdint.h>

// Global variables
static I2C_HandleTypeDef *ssd1306_i2c;

// Create buffers as unions, to make it cleaner on the code
union input_buffer {
	uint8_t byte[(SSD1306_LOGICAL_H * SSD1306_WIDTH) / 8];
	uint32_t word[SSD1306_WIDTH];
} input_buffer;
union output_buffer {
	uint8_t byte[(SSD1306_HEIGHT / 8) * SSD1306_WIDTH];
	uint64_t dword[SSD1306_WIDTH]; // Account for command byte
} output_buffer;

// Internal helpers
static void ssd1306_WriteCommand(I2C_HandleTypeDef *hi2c, uint8_t cmd)
{
    uint8_t d[2] = {0x00, cmd};
    HAL_I2C_Master_Transmit(hi2c, SSD1306_I2C_ADDR, d, 2, HAL_MAX_DELAY);
}

// Public functions
void SSD1306_Init(I2C_HandleTypeDef *hi2c) {
    ssd1306_i2c = hi2c;

    HAL_Delay(100); // power-on delay

    ssd1306_WriteCommand(hi2c, 0xAE); // Display off
    ssd1306_WriteCommand(hi2c, 0x20); ssd1306_WriteCommand(hi2c, 0x01); // Vertical addressing
    ssd1306_WriteCommand(hi2c, 0xA1); // Segment remap 127->0
    ssd1306_WriteCommand(hi2c, 0xC8); // COM scan direction remapped
    ssd1306_WriteCommand(hi2c, 0x81); ssd1306_WriteCommand(hi2c, 0x7F); // contrast
    ssd1306_WriteCommand(hi2c, 0xA6); // normal display
    ssd1306_WriteCommand(hi2c, 0xA8); ssd1306_WriteCommand(hi2c, 0x3F); // MUX 63
    ssd1306_WriteCommand(hi2c, 0xD3); ssd1306_WriteCommand(hi2c, 0x00); // display offset
    ssd1306_WriteCommand(hi2c, 0xD5); ssd1306_WriteCommand(hi2c, 0x80); // display clock
    ssd1306_WriteCommand(hi2c, 0xD9); ssd1306_WriteCommand(hi2c, 0xF1); // pre-charge
    ssd1306_WriteCommand(hi2c, 0xDA); ssd1306_WriteCommand(hi2c, 0x12); // COM pins
    ssd1306_WriteCommand(hi2c, 0xDB); ssd1306_WriteCommand(hi2c, 0x40); // VCOM detect
    ssd1306_WriteCommand(hi2c, 0x8D); ssd1306_WriteCommand(hi2c, 0x14); // charge pump
    ssd1306_WriteCommand(hi2c, 0xAF); // Display ON
    return;
}


void SSD1306_SendBuffer(I2C_HandleTypeDef *hi2c, const uint8_t *buf)
{
	// empty the buffers and copy the input into the internal_buffers
	memset(output_buffer.byte, 0x00, sizeof(output_buffer.byte));
	memcpy((void *)input_buffer.byte, buf, sizeof(input_buffer.byte));

	// Transfer data from one buffer to the other, while converting bits positions :
	// In fact, we iterate over the "columns" as the screen naming conventions, but on our case (rotated buffer + rotated screen, it look like "lines").
	for (uint8_t line = 0; line < SSD1306_WIDTH; line++)
	{
		uint64_t tmp = 0;
		// iterate over each pixels
		for (uint8_t pixel = 0; pixel <32; pixel++)
		{
			// uint8_t bit = (input_buffer.word[line] & (1 << pixel)) >> pixel;
			uint8_t bit = (pixel % 2 == 0) ? 0 : 1;

			tmp |= (bit << (2 * pixel));
			tmp |= (bit << ((2 * pixel) + 1));
		}

		/*
		 * Finally, we need to swap the bytes order : The Page 0 is the "end" of our screen line,
		 * where, if we send the whole buffer at once, the first byte is Page 7. This will create
		 * issues on the buffer rendering on screen.
		 *
		 * Actually, we have
		 * Byte 0	1	2	3	4	5	6	7
		 * Page 0	1	2	3	4	5	6	7
		 *
		 * Thus, we need to ensure that :
		 * Byte 0	1	2	3	4	5	6	7
		 * Page 7	6	5	4	3	2	1	0
		 *
		 * Bits inside pages / bytes are not affected by this logic.
		 */

		output_buffer.dword[line] = ((tmp & 0x00000000000000FF) >> 0)  << 56 |
									((tmp & 0x000000000000FF00) >> 8)  << 48 |
									((tmp & 0x0000000000FF0000) >> 16) << 40 |
									((tmp & 0x00000000FF000000) >> 24) << 32 |
									((tmp & 0x000000FF00000000) >> 32) << 24 |
									((tmp & 0x0000FF0000000000) >> 40) << 16 |
									((tmp & 0x00FF000000000000) >> 48) << 8  |
									((tmp & 0xFF00000000000000) >> 56) << 0  ;
	}
	// Set vertical addressing mode
	ssd1306_WriteCommand(hi2c, 0x20);
	ssd1306_WriteCommand(hi2c, 0x01);

	// Write commands about the position and parameters of the screen
    ssd1306_WriteCommand(hi2c, 0x21); // Column address start/end
    ssd1306_WriteCommand(hi2c, 0x00); // start column
    ssd1306_WriteCommand(hi2c, 0x7F); // end column 127

    ssd1306_WriteCommand(hi2c, 0x22); // Page address start/end
    ssd1306_WriteCommand(hi2c, 0x00); // start page
    ssd1306_WriteCommand(hi2c, 0x07); // end page 7

    // Send control byte first
    uint8_t control = 0x40;
    HAL_I2C_Master_Transmit(	hi2c,
    							SSD1306_I2C_ADDR,
								&control,
								1,
								HAL_MAX_DELAY);

    // Then send the 1024 bytes to fill the memory
    HAL_I2C_Master_Transmit(	hi2c,
    							SSD1306_I2C_ADDR,
								output_buffer.byte,
								(SSD1306_HEIGHT / 8) * SSD1306_WIDTH,
								HAL_MAX_DELAY);

    return;
}

