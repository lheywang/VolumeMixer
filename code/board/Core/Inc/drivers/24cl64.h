/** ================================================================
 * @file    external_drivers/24cl64.c
 *
 * @brief   Header file of the 24CL64 EEPROM Driver (Thanks to chatGPT).
 *
 * @date 	25-09-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */
#pragma once

/*
 * -----------------------------------------------------------------
 * Includes
 * -----------------------------------------------------------------
 */
#include "stm32f3xx_hal.h"
#include <stdint.h>

/*
 * -----------------------------------------------------------------
 * Defines
 * -----------------------------------------------------------------
 */
#define EEPROM_OK           0
#define EEPROM_ERR_ARG     -1
#define EEPROM_ERR_I2C     -2
#define EEPROM_ERR_PARAM   -3

#define EEPROM_ADDR 		(0x50 << 1)

#define EEPROM_PAGE_SIZE 	32

#define EEPROM_SIZE			8192

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
/**
 * Initialize EEPROM driver instance.
 *
 * hi2c: HAL I2C handle
 * dev_addr_7bit: 7-bit I2C address of EEPROM (e.g. 0x50 for typical AT24Cxx)
 * page_size: page size in bytes (e.g. 32)
 * size_bytes: total capacity in bytes (optional, 0 if unknown)
 */
int eeprom_init(I2C_HandleTypeDef *hi2c, uint8_t dev_addr_7bit, uint16_t page_size, uint32_t size_bytes);

/**
 * Read 'len' bytes from EEPROM starting at 'mem_address' into 'buf'.
 * mem_address: 0..(size_bytes-1)
 */
int eeprom_read(uint16_t mem_address, uint8_t *buf, uint16_t len, uint32_t timeout_ms);

/**
 * Write 'len' bytes to EEPROM starting at 'mem_address' from 'buf'.
 * Will split into page writes automatically.
 *
 * NOTE: EEPROM write is slow. This function will wait for the internal write
 * cycle to finish after each page by polling the device with HAL_I2C_IsDeviceReady.
 */
int eeprom_write(uint16_t mem_address, const uint8_t *buf, uint16_t len, uint32_t timeout_ms);

/**
 * Erase region by writing 0xFF across len bytes starting at mem_address.
 */
int eeprom_erase(uint16_t mem_address, uint16_t len, uint8_t fill, uint32_t timeout_ms);
