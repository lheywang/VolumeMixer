/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hal/init.hpp"
#include "hal/leds.hpp"

int main(void)
{

  init::full();

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  // uint8_t *data = (uint8_t*)"Hello World \r\n";

	  // HAL_UART_Transmit(&init::huart3, data, 15, 100);
	  leds::debug_Toggle_Led();
	  HAL_Delay(1000);
  }
  /* USER CODE END 3 */
}

