/** ================================================================
 * @file    external_drivers/24cl64.c
 *
 * @brief   Source file of the 24CL64 EEPROM Driver (Thanks to chatGPT).
 *
 * @date 	25-09-2025
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
#include "drivers/24cl64.h"
#include <string.h>

/*
 * -----------------------------------------------------------------
 * Private struct
 * -----------------------------------------------------------------
 */
static struct {
    I2C_HandleTypeDef *hi2c;   // HAL I2C handle
    uint16_t dev_addr;        // 7-bit device base address (left-shifted for HAL, see helper)
    uint16_t page_size;       // page size in bytes (usually 32)
    uint32_t size_bytes;      // total device capacity in bytes (optional)
} eeprom_t;

/*
 * -----------------------------------------------------------------
 * Private functions
 * -----------------------------------------------------------------
 */
static int eeprom_wait_write_complete(uint32_t timeout_ms)
{
    // HAL_I2C_IsDeviceReady expects the 7-bit address left-shifted already by HAL packaging,
    // but we stored dev->dev_addr as 8-bit (<<1). HAL wants devAddress parameter in 8-bit as well.
    uint32_t trials = (timeout_ms == 0) ? 10 : (timeout_ms / 5 + 1);
    if (trials == 0) trials = 1;
    for (uint32_t i = 0; i < trials; i++) {
        if (HAL_I2C_IsDeviceReady(eeprom_t.hi2c, eeprom_t.dev_addr, 1, 5) == HAL_OK) {
            return EEPROM_OK;
        }
        HAL_Delay(5);
    }
    return EEPROM_ERR_I2C;
}

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
int eeprom_init(I2C_HandleTypeDef *hi2c, uint8_t dev_addr_7bit, uint16_t page_size, uint32_t size_bytes)
{
    if (!hi2c || page_size == 0) return EEPROM_ERR_ARG;
    eeprom_t.hi2c = hi2c;
    eeprom_t.dev_addr = (uint16_t)dev_addr_7bit; // HAL expects 8-bit address (R/W bit cleared in functions)
    eeprom_t.page_size = page_size;
    eeprom_t.size_bytes = size_bytes;
    return EEPROM_OK;
}

int eeprom_read(uint16_t mem_address, uint8_t *buf, uint16_t len, uint32_t timeout_ms)
{
    if (!buf) return EEPROM_ERR_ARG;
    // Optionally check bounds
    if (eeprom_t.size_bytes && (mem_address + len > eeprom_t.size_bytes)) return EEPROM_ERR_PARAM;

    // Many AT24Cxx use 16-bit memory addressing when larger than 16kbit
    if (HAL_I2C_Mem_Read(eeprom_t.hi2c, eeprom_t.dev_addr, (uint16_t)mem_address, I2C_MEMADD_SIZE_16BIT, buf, len, timeout_ms) != HAL_OK)
    {
        return EEPROM_ERR_I2C;
    }
    return EEPROM_OK;
}

int eeprom_write(uint16_t mem_address, const uint8_t *buf, uint16_t len, uint32_t timeout_ms)
{
    if (!buf) return EEPROM_ERR_ARG;
    if (len == 0) return EEPROM_OK;
    if (eeprom_t.size_bytes && (mem_address + len > eeprom_t.size_bytes)) return EEPROM_ERR_PARAM;

    uint16_t remaining = len;
    uint16_t offset = 0;

    while (remaining > 0)
    {
        // compute page boundary
        uint16_t page_offset = mem_address % eeprom_t.page_size;
        uint16_t space_in_page = eeprom_t.page_size - page_offset;
        uint16_t chunk = (remaining < space_in_page) ? remaining : space_in_page;

        // HAL_I2C_Mem_Write will send mem address then data
        if (HAL_I2C_Mem_Write(eeprom_t.hi2c, eeprom_t.dev_addr, (uint16_t)(mem_address), I2C_MEMADD_SIZE_16BIT,
                              (uint8_t *)&buf[offset], chunk, timeout_ms) != HAL_OK)
        {
            return EEPROM_ERR_I2C;
        }

        // Wait for internal write cycle to complete (typical 5-10 ms)
        if (eeprom_wait_write_complete(timeout_ms) != EEPROM_OK) {
            return EEPROM_ERR_I2C;
        }

        // advance
        remaining -= chunk;
        offset += chunk;
        mem_address += chunk;
    }

    return EEPROM_OK;
}

int eeprom_erase(uint16_t mem_address, uint16_t len, uint8_t fill, uint32_t timeout_ms)
{
    if (len == 0) return EEPROM_OK;
    // allocate a small page buffer
    uint8_t tmp[64];
    if (eeprom_t.page_size > (int)sizeof(tmp)) return EEPROM_ERR_PARAM;
    memset(tmp, fill, eeprom_t.page_size);

    uint16_t remaining = len;
    while (remaining > 0)
    {
        uint16_t page_offset = mem_address % eeprom_t.page_size;
        uint16_t space_in_page = eeprom_t.page_size - page_offset;
        uint16_t chunk = (remaining < space_in_page) ? remaining : space_in_page;
        if (HAL_I2C_Mem_Write(eeprom_t.hi2c, eeprom_t.dev_addr, (uint16_t)mem_address, I2C_MEMADD_SIZE_16BIT,
                              tmp + page_offset, chunk, timeout_ms) != HAL_OK)
        {
            return EEPROM_ERR_I2C;
        }
        if (eeprom_wait_write_complete(timeout_ms) != EEPROM_OK) {
            return EEPROM_ERR_I2C;
        }
        remaining -= chunk;
        mem_address += chunk;
    }
    return EEPROM_OK;
}
