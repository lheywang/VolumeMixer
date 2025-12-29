/** ================================================================
 * @file    handlers/timers.c
 *
 * @brief   Implement TIM related functions, and handlers
 *
 * @date 	12/11/2025
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
#include "handlers/timers.h"

#include "handlers/adc.h"
#include "handlers/tsc.h"
#include "handlers/leds.h"

#include "utils/CDC_helper.h"

#include "stm32f3xx_hal.h"

#include <stdio.h>

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
// Imported
// From main.c
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim15;
extern UART_HandleTypeDef huart3;

// Froms handler/leds.c
extern GPIO_PWMx PWMs[PWM_CHANNEL_NB];

// Public
uint8_t isr_message[ISR_MESSAGE_SIZE] = { 0 };
uint8_t msg_available = 0;
uint16_t value = 0;

// Private
static int count = 0;
static uint8_t first = 0;

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */

int timer_launch(void)
{
  // Start the timers
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim15);
  // Timer 16 is started on demand of the CDC helper

  first = 0;
  return 0;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		TIM2_ISR();
	}
	else if (htim->Instance == TIM15)
	{
		TIM15_ISR();
	}
	else if (htim->Instance == TIM16)
	{
		USB_IRQHandler();
	}
	return;
}

void TIM2_ISR(void)
{
	for (int k = 0; k < PWM_CHANNEL_NB; k++)
	{
		__update_PWM(&PWMs[k]);
	}
	return;
}

void TIM15_ISR(void)
{

	// read the data (while ignoring the first run).
	if (first != 0)
	{
		value = adc_read_channel();
	}

	// start the aquisition of the next channel
	adc_configure_next_channel();

	// Add a print message every seconds
	if (count >= 100)
	{
		count = 0;
		msg_available = 1;
	}

	count += 1;
	first = 1;

	return;
}

/*
 * The last ISR is defined on the CDC_helper.c file
 */
