/** ================================================================
 * @file    config.h
 *
 * @brief   Configure project-wise settings
 *
 * @date 	11/11/2025
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
 * Defines
 * -----------------------------------------------------------------
 */
#define CDC_BUFFER_SIZE 1024 /* Buffer used for the USB frontend */

/*
 * -----------------------------------------------------------------
 * Build configuration
 * -----------------------------------------------------------------
 */
/*
 * Global uart log
 */
#define USE_UART_LOGS /* Used to test if the leds that were flickering came from the UART logs */

/*
 * Per type logs
 */
// Init logs
#define LOG_INIT

// Per commands logs
/*
 * Simple commands
 */
#define LOG_SHUTD
#define LOG_RESET
#define LOG_DCONF
#define LOG_CONNC
#define LOG_UICON

/*
 * Commands that produce a lot of logs :
 */
// #define LOG_ASYNC
// #define LOG_SLPOS



