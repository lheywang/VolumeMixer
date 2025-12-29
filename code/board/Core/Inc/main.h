/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define led_red_Pin GPIO_PIN_13
#define led_red_GPIO_Port GPIOC
#define led_green_Pin GPIO_PIN_14
#define led_green_GPIO_Port GPIOC
#define led_blue_Pin GPIO_PIN_15
#define led_blue_GPIO_Port GPIOC
#define dbg_bp_Pin GPIO_PIN_1
#define dbg_bp_GPIO_Port GPIOF
#define Slider0_Pin GPIO_PIN_0
#define Slider0_GPIO_Port GPIOA
#define Slider1_Pin GPIO_PIN_1
#define Slider1_GPIO_Port GPIOA
#define Slider2_Pin GPIO_PIN_2
#define Slider2_GPIO_Port GPIOA
#define Slider3_Pin GPIO_PIN_3
#define Slider3_GPIO_Port GPIOA
#define Slider4_Pin GPIO_PIN_4
#define Slider4_GPIO_Port GPIOA
#define dbg_led_Pin GPIO_PIN_5
#define dbg_led_GPIO_Port GPIOA
#define vref_dig_Pin GPIO_PIN_6
#define vref_dig_GPIO_Port GPIOA
#define vref_ana_Pin GPIO_PIN_7
#define vref_ana_GPIO_Port GPIOA
#define vref_usb_Pin GPIO_PIN_0
#define vref_usb_GPIO_Port GPIOB
#define led_mute4_Pin GPIO_PIN_1
#define led_mute4_GPIO_Port GPIOB
#define led_mute3_Pin GPIO_PIN_2
#define led_mute3_GPIO_Port GPIOB
#define dbg_tx_Pin GPIO_PIN_10
#define dbg_tx_GPIO_Port GPIOB
#define led_mute2_Pin GPIO_PIN_15
#define led_mute2_GPIO_Port GPIOB
#define led_mute1_Pin GPIO_PIN_8
#define led_mute1_GPIO_Port GPIOA
#define led_mute0_Pin GPIO_PIN_15
#define led_mute0_GPIO_Port GPIOA
#define dbg_io1_Pin GPIO_PIN_5
#define dbg_io1_GPIO_Port GPIOB
#define dbg_io0_Pin GPIO_PIN_7
#define dbg_io0_GPIO_Port GPIOB
#define power_on_Pin GPIO_PIN_9
#define power_on_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
