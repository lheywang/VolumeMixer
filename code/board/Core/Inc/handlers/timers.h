/** ================================================================
 * @file    handlers/timers.h
 *
 * @brief   Define TIM related functions, and handlers
 *
 * @date 	12/11/2025
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

/*
 * -----------------------------------------------------------------
 * Defines
 * -----------------------------------------------------------------
 */
#define ISR_MESSAGE_SIZE 256

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
/**
 * @brief Launch the timers. Will
 */
int timer_launch(void);

/**
 * @brief Handler for the timer interrupt. Call another helper function.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

/**
 *	@brief Timer 2 ISR routine
 */
void TIM2_ISR(void);

/**
 *	@brief Timer 15 ISR routine
 */
void TIM15_ISR(void);
