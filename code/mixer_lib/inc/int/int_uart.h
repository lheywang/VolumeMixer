/** ================================================================
 * @file    mixer/int/int_uart.h
 *
 * @brief   This file define the interrupts procedures for the
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
#pragma once

/* -----------------------------------------------------------------
 * INCLUDING LIBS
 * -----------------------------------------------------------------
 */

// ST HAL
#include "main.h"


/* -----------------------------------------------------------------
 * INT_UART
 * -----------------------------------------------------------------
 */

/**
 * @brief 	Initialiaze the ISR in RTO mode for the UART bus.
 *
 * @param 	handle to initialize
 *
 * @return 	int
 * @retval   0 : 	Everything suceeded
 */
int isr_uart_init(UART_HandleTypeDef *huartX);

/**
 * @brief 	Interrupt routine for the UART bus
 *
 * @param 	Handle of the peripheral that triggered interrupt
 * @param 	Size : Number of bytes to treat.
 *
 * @return 	void
 */
void HAL_UART_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);



