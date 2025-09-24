#include "external_drivers/ssd1306.h"

// Global variables
static I2C_HandleTypeDef *ssd1306_i2c;
static uint8_t ssd1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
static uint8_t logicalBuffer[SSD1306_WIDTH * SSD1306_LOGICAL_H / 8]; // 128x32

// Internal helpers
static void ssd1306_WriteCommand(uint8_t command) {
    uint8_t data[2] = {0x00, command}; // 0x00 = Co = 0, D/C# = 0
    HAL_I2C_Master_Transmit(ssd1306_i2c, SSD1306_I2C_ADDR, data, 2, HAL_MAX_DELAY);
}

static void ssd1306_WriteData(uint8_t *data, size_t size) {
    uint8_t buffer[size + 1];
    buffer[0] = 0x40; // 0x40 = Co = 0, D/C# = 1
    memcpy(&buffer[1], data, size);
    HAL_I2C_Master_Transmit(ssd1306_i2c, SSD1306_I2C_ADDR, buffer, size + 1, HAL_MAX_DELAY);
}

// Rotate + remap
static inline void logicalToPhysical(uint8_t x, uint8_t y, uint8_t *px, uint8_t *py) {
    // Rotate 90° (vertical, pins at top)
    uint8_t xr = y;
    uint8_t yr = (SSD1306_WIDTH - 1) - x;

    // Remap: skip every second line
    *px = xr;
    *py = yr * 2 + 1;
}

// Public functions
void SSD1306_Init(I2C_HandleTypeDef *hi2c) {
    ssd1306_i2c = hi2c;

    HAL_Delay(100); // power-on delay

    // Init sequence
    ssd1306_WriteCommand(0xAE); // Display off
    ssd1306_WriteCommand(0x20); // Set Memory Addressing Mode
    ssd1306_WriteCommand(0x00); // Horizontal addressing mode
    ssd1306_WriteCommand(0xB0); // Page start address
    ssd1306_WriteCommand(0xC8); // COM scan direction remapped
    ssd1306_WriteCommand(0x00); // Low column
    ssd1306_WriteCommand(0x10); // High column
    ssd1306_WriteCommand(0x40); // Start line address
    ssd1306_WriteCommand(0x81); ssd1306_WriteCommand(0x7F); // Contrast
    ssd1306_WriteCommand(0xA1); // Segment remap
    ssd1306_WriteCommand(0xA6); // Normal display
    ssd1306_WriteCommand(0xA8); ssd1306_WriteCommand(0x3F); // Multiplex ratio
    ssd1306_WriteCommand(0xA4); // Display follows RAM
    ssd1306_WriteCommand(0xD3); ssd1306_WriteCommand(0x00); // Display offset
    ssd1306_WriteCommand(0xD5); ssd1306_WriteCommand(0x80); // Clock divide
    ssd1306_WriteCommand(0xD9); ssd1306_WriteCommand(0xF1); // Pre-charge
    ssd1306_WriteCommand(0xDA); ssd1306_WriteCommand(0x12); // COM pins
    ssd1306_WriteCommand(0xDB); ssd1306_WriteCommand(0x40); // VCOM detect
    ssd1306_WriteCommand(0x8D); ssd1306_WriteCommand(0x14); // Charge pump
    ssd1306_WriteCommand(0xAF); // Display ON

    SSD1306_Fill(false);
    SSD1306_UpdateScreen();
}

void SSD1306_Fill(bool color) {
    memset(ssd1306_Buffer, (color ? 0xFF : 0x00), sizeof(ssd1306_Buffer));
}

void SSD1306_UpdateScreen(void) {
    // Clear physical buffer
    memset(ssd1306_Buffer, 0x00, sizeof(ssd1306_Buffer));

    // Map logical buffer → physical
    for (uint8_t y = 0; y < SSD1306_LOGICAL_H; y++) {
        for (uint8_t x = 0; x < SSD1306_WIDTH; x++) {
            uint16_t idx = x + (y/8)*SSD1306_WIDTH;
            bool pixel = (logicalBuffer[idx] >> (y%8)) & 1;

            if (pixel) {
                uint8_t px, py;
                logicalToPhysical(x, y, &px, &py);
                uint16_t pidx = px + (py/8)*SSD1306_WIDTH;
                ssd1306_Buffer[pidx] |= (1 << (py%8));
            }
        }
    }

    // Send to OLED
    for (uint8_t page=0; page<8; page++) {
        ssd1306_WriteCommand(0xB0 + page);
        ssd1306_WriteCommand(0x00);
        ssd1306_WriteCommand(0x10);
        ssd1306_WriteData(&ssd1306_Buffer[SSD1306_WIDTH*page], SSD1306_WIDTH);
    }
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y, bool color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_LOGICAL_H) return;
    uint16_t byteIndex = x + (y/8)*SSD1306_WIDTH;
    if (color) logicalBuffer[byteIndex] |= (1 << (y%8));
    else       logicalBuffer[byteIndex] &= ~(1 << (y%8));
}

void SSD1306_DrawBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *data) {
    for (uint8_t j=0; j<h; j++) {
        for (uint8_t i=0; i<w; i++) {
            uint8_t byte = data[(j*w + i)/8];
            uint8_t bit = 1 << (j % 8);
            if (byte & bit) SSD1306_DrawPixel(x+i, y+j, true);
        }
    }
}

void SSD1306_PrintDigit(uint8_t x, uint8_t y, int digit, const uint8_t font[][32]) {
    if (digit<0 || digit>9) return;
    SSD1306_DrawBitmap(x, y, 8, 16, font[digit]); // assume font is 8x16 (32 bytes)
}
