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
#define led_on_Pin GPIO_PIN_13
#define led_on_GPIO_Port GPIOC
#define led_connect_Pin GPIO_PIN_14
#define led_connect_GPIO_Port GPIOC
#define led_fail_Pin GPIO_PIN_15
#define led_fail_GPIO_Port GPIOC
#define dbg_bp_Pin GPIO_PIN_1
#define dbg_bp_GPIO_Port GPIOF
#define adc_slider0_Pin GPIO_PIN_0
#define adc_slider0_GPIO_Port GPIOA
#define adc_slider1_Pin GPIO_PIN_1
#define adc_slider1_GPIO_Port GPIOA
#define adc_slider2_Pin GPIO_PIN_2
#define adc_slider2_GPIO_Port GPIOA
#define adc_slider3_Pin GPIO_PIN_3
#define adc_slider3_GPIO_Port GPIOA
#define adc_slider4_Pin GPIO_PIN_4
#define adc_slider4_GPIO_Port GPIOA
#define dbg_led_Pin GPIO_PIN_5
#define dbg_led_GPIO_Port GPIOA
#define adc_vrefd_Pin GPIO_PIN_6
#define adc_vrefd_GPIO_Port GPIOA
#define adc_vrefa_Pin GPIO_PIN_7
#define adc_vrefa_GPIO_Port GPIOA
#define adc_verfu_Pin GPIO_PIN_0
#define adc_verfu_GPIO_Port GPIOB
#define led_mute3_Pin GPIO_PIN_1
#define led_mute3_GPIO_Port GPIOB
#define led_mute4_Pin GPIO_PIN_2
#define led_mute4_GPIO_Port GPIOB
#define dbg_tx_Pin GPIO_PIN_10
#define dbg_tx_GPIO_Port GPIOB
#define tsc_slider2_Pin GPIO_PIN_11
#define tsc_slider2_GPIO_Port GPIOB
#define tsc_slider3_Pin GPIO_PIN_12
#define tsc_slider3_GPIO_Port GPIOB
#define tsc_slider4_Pin GPIO_PIN_13
#define tsc_slider4_GPIO_Port GPIOB
#define tsc_sense1_Pin GPIO_PIN_14
#define tsc_sense1_GPIO_Port GPIOB
#define led_mute2_Pin GPIO_PIN_15
#define led_mute2_GPIO_Port GPIOB
#define led_mute1_Pin GPIO_PIN_8
#define led_mute1_GPIO_Port GPIOA
#define swclk_Pin GPIO_PIN_13
#define swclk_GPIO_Port GPIOA
#define swdio_Pin GPIO_PIN_14
#define swdio_GPIO_Port GPIOA
#define led_mute0_Pin GPIO_PIN_15
#define led_mute0_GPIO_Port GPIOA
#define tsc_slider1_Pin GPIO_PIN_3
#define tsc_slider1_GPIO_Port GPIOB
#define tsc_slider0_Pin GPIO_PIN_4
#define tsc_slider0_GPIO_Port GPIOB
#define dbg_io0_Pin GPIO_PIN_5
#define dbg_io0_GPIO_Port GPIOB
#define tsc_sense0_Pin GPIO_PIN_6
#define tsc_sense0_GPIO_Port GPIOB
#define dbg_io1_Pin GPIO_PIN_7
#define dbg_io1_GPIO_Port GPIOB
#define dbg_rx_Pin GPIO_PIN_8
#define dbg_rx_GPIO_Port GPIOB
#define bp_poweron_Pin GPIO_PIN_9
#define bp_poweron_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
