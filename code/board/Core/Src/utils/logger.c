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
/*
 * -----------------------------------------------------------------
 * Includes
 * -----------------------------------------------------------------
 */
#include "utils/logger.h"

#include "config.h"

#include <stdint.h>

#include "stm32f3xx_hal.h"

extern UART_HandleTypeDef huart3;
/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */

uint8_t log_buf[LOG_BUF_SIZE];
volatile uint16_t log_head = 0;
volatile uint16_t log_tail = 0;
volatile uint8_t dma_running = 0;

uint16_t current_chunk_len = 0;

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
void uart_log(const uint8_t *msg, uint16_t len)
{
#ifdef USE_UART_LOGS
    for (uint16_t i = 0; i < len; i++) {
        log_buf[log_head] = msg[i];
        log_head = (log_head + 1) % LOG_BUF_SIZE;
    }

    // If DMA idle, start transmission
	if (!dma_running) {
		dma_running = 1;
		current_chunk_len = (log_head >= log_tail) ? (log_head - log_tail) : (LOG_BUF_SIZE - log_tail);
		HAL_UART_Transmit_DMA(&huart3, &log_buf[log_tail], current_chunk_len);
	}
#else
	UNUSED(msg);
	UNUSED(len);
#endif
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
#ifdef USE_UART_LOGS
    log_tail = (log_tail + current_chunk_len) % LOG_BUF_SIZE;

    if (log_tail != log_head) {
        current_chunk_len = (log_head >= log_tail) ? (log_head - log_tail) : (LOG_BUF_SIZE - log_tail);
        HAL_UART_Transmit_DMA(&huart3, &log_buf[log_tail], current_chunk_len);
    } else {
        dma_running = 0;
    }
#endif
}
