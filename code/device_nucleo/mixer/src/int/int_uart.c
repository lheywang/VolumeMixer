/** ================================================================
 * @file    mixer/int/int_uart.c
 *
 * @brief   This file implement the interrupts procedures for the
 * 			UART bus
 *
 * @date    21-06-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */
/* -----------------------------------------------------------------
 * INCLUDING LIBS
 * -----------------------------------------------------------------
 */
// Header
#include "int/int_uart.h"

// ST HAL
#include "main.h"

// STD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

volatile char RxBuffer[1024];
volatile char TxBuffer[1024];

/* -----------------------------------------------------------------
 * INT_UART
 * -----------------------------------------------------------------
 */

int isr_uart_init(UART_HandleTypeDef *huartX)
{
	// First, enable the RTO interrupt on the STM32
	// Timeout is set to 1000 bits, or about 2 ms.
	// huartX->Init.RTOEN = UART_RTOEN_ENABLE;
	// huartX->Init.ReceiverTimeout = 1000;
	if (HAL_UART_Init(huartX) != HAL_OK)
	{
	    Error_Handler();
	}

	// Enable the RTO interrupt in the UART peripheral
	__HAL_UART_ENABLE_IT(huartX, UART_IT_RTO);

	// Configure the interrupt priority of the UART bus
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void HAL_UART_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART2)
  {
    // Check if the event was an RTO (Receive Timeout)
    // In many HAL versions, Size will correspond to the bytes received before RTO.
    // You might also check huart->RxState == HAL_UART_STATE_READY if you want
    // to differentiate between RTO and full buffer, though Size is usually enough.

    // Process the received data up to 'Size' bytes.
    // The data is in RxBuffer.
    // For demonstration, let's echo the received message back to the serial terminal,
    // prepending it with "RTO RX: " or "Full RX: ".

    int len;
    if (Size > 0 && Size <= 1024) // Ensure size is valid
    {
        // Null-terminate the received data for string functions
        RxBuffer[Size] = '\0'; // Make sure the buffer has space for null terminator

        // Check if it was an RTO (implies partial buffer or end of message)
        // A common way to infer RTO is when the transfer stops before the full buffer is filled.
        // The HAL_UART_RxEventCallback is triggered for RTO.
        if (Size < 1024) { // If less than full buffer, it's likely an RTO or Idle Line
            len = snprintf((char*)TxBuffer, sizeof(TxBuffer), "RTO RX (%u bytes): %s\r\n", Size, RxBuffer);
        } else { // Full buffer received
            len = snprintf((char*)TxBuffer, sizeof(TxBuffer), "Full RX (%u bytes): %s\r\n", Size, RxBuffer);
        }

        // Transmit the response
        if (len > 0) {
            HAL_UART_Transmit(&huart2, TxBuffer, len, HAL_MAX_DELAY);
        }
    }
    else if (Size == 0)
    {
        // This might happen for some RTO/IDLE events if no data was actually received,
        // or other specific edge cases depending on HAL version.
        len = snprintf((char*)TxBuffer, sizeof(TxBuffer), "RTO/IDLE RX (0 bytes)\r\n");
        HAL_UART_Transmit(&huart2, TxBuffer, len, HAL_MAX_DELAY);
    }
    else
    {
        // Handle unexpected size values, perhaps an overflow detection
        len = snprintf((char*)TxBuffer, sizeof(TxBuffer), "Error: Invalid RX size %u\r\n", Size);
        HAL_UART_Transmit(&huart2, TxBuffer, len, HAL_MAX_DELAY);
    }


    // IMPORTANT: Restart the DMA reception after processing the current data.
    // The DMA transfer stops when an RTO or full buffer event occurs.
    if (HAL_UART_Receive_DMA(&huart2, RxBuffer, 1024) != HAL_OK)
    {
      Error_Handler(); // Handle error if DMA reception cannot be restarted
    }
  }
}
