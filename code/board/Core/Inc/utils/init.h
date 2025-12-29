/** ================================================================
 * @file    utils/init.h
 *
 * @brief   Header for the board init function
 *
 * @date 	13-11-2025
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
// STD
#include <stdint.h>

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
/**
 * @brief Handle the whole board initialisation procedure. Does return 0, or does never return.
 */
int board_init(void);

/**
 * @brief Scan the I2C bus to search for wired devices. Permit to ensure a correct operation of the device.
 */
int scan_i2c(void);

/**
 * @brief Verify that power supplies are within tolerance.
 */
int check_supplies(void);

/**
 * @brief Show the values from the loaded header
 */
int show_loaded_header(void);
