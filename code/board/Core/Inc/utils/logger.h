/** ================================================================
 * @file    utils/logger.h
 *
 * @brief   Header file for the logger module, based on DMA transfers.
 *
 * @date 	21-11-2025
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

#include "stm32f3xx_hal.h"

/*
 * -----------------------------------------------------------------
 * Defines
 * -----------------------------------------------------------------
 */
#define LOG_BUF_SIZE 400

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
/**
 * @brief Create a log entry into the buffer, and if possible start the DMA transfer.
 */
void uart_log(const uint8_t *msg, uint16_t len);

/**
 * @brief ISR used when a transfer is done. Retrigger it if needed.
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
