/** ================================================================
 * @file    drivers/tca9548.h
 *
 * @brief   Header file of the TCA9548 I2C
 *
 * @date 	11-11-2025
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
#include <stdint.h>
#include <stddef.h>

#include "stm32f3xx_hal.h"

/*
 * -----------------------------------------------------------------
 * Defines
 * -----------------------------------------------------------------
 */
#define TCA9548A_ADDR  (0x70 << 1)

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
/**
 * Init the I2C mux
 */
int tca9548_init(I2C_HandleTypeDef *hi2c);

/**
 * Select a channel (1-7) on the I2C mux
 */
int tca9548_select(uint8_t channel);

/**
 * Select a channel based on the mask provided (used to set more than 1 channel).
 */
int tca9548_select_mask(uint8_t channel);

/**
 * Disable all enabled devices.
 */
int tca9548_disable_all(void);

/**
 * Return the enabled devices.
 */
uint8_t tca9548_get_active(void);
