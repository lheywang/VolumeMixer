/** ================================================================
 * @file    handlers/leds.c
 *
 * @brief   Implement the software PWM
 *
 * @date 	13/11/2025
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
#include "handlers/leds.h"

#include "stm32f3xx_hal.h"

#include <stdio.h>

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
GPIO_PWMx PWMs[PWM_CHANNEL_NB] = { 0 };

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
int init_pwm(GPIO_PWMx *handle, GPIO_TypeDef* GPIO_BASE, uint16_t GPIO_PIN, uint8_t duty, uint8_t enable)
{
	if (!handle) return 1;

	// First, copy some data into the structs :
	handle->_GPIO_BASE = GPIO_BASE;
	handle->_GPIO_PIN = GPIO_PIN;

	uint32_t value = (duty * duty) / 100;

	handle->_threshold = (enable != 0) ? (PWM_MAX_COUNT * value) / 100 : 0;
	handle->_value = 0;

	return 0;
}

int set_pwm(GPIO_PWMx *handle, uint8_t duty, uint8_t enable)
{
	uint32_t value = (duty * duty) / 100;

	handle->_threshold = (enable != 0) ? (PWM_MAX_COUNT * value) / 100 : 0;
	handle->_value = 0;

	return 0;
}

void __update_PWM(GPIO_PWMx *handle)
{
	// First, increment the counter
	handle->_value += 1;

	// Check if we need to reset the counter
	if (handle->_value >= PWM_MAX_COUNT)
	{
		handle->_value = 0;
	}

	// Apply the output based on the threshold.
	if (handle->_value < handle->_threshold)
	{
		HAL_GPIO_WritePin(handle->_GPIO_BASE, handle->_GPIO_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(handle->_GPIO_BASE, handle->_GPIO_PIN, GPIO_PIN_RESET);
	}

	return;
}


