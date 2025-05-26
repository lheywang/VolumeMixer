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
#define vref_dig_Pin GPIO_PIN_6
#define vref_dig_GPIO_Port GPIOA
#define vref_ana_Pin GPIO_PIN_7
#define vref_ana_GPIO_Port GPIOA
#define vref_usb_Pin GPIO_PIN_0
#define vref_usb_GPIO_Port GPIOB
#define led_mute3_Pin GPIO_PIN_1
#define led_mute3_GPIO_Port GPIOB
#define led_mute4_Pin GPIO_PIN_2
#define led_mute4_GPIO_Port GPIOB
#define dbg_tx_Pin GPIO_PIN_10
#define dbg_tx_GPIO_Port GPIOB
#define mute5_Pin GPIO_PIN_11
#define mute5_GPIO_Port GPIOB
#define mute4_Pin GPIO_PIN_12
#define mute4_GPIO_Port GPIOB
#define mute3_Pin GPIO_PIN_13
#define mute3_GPIO_Port GPIOB
#define capsense1_Pin GPIO_PIN_14
#define capsense1_GPIO_Port GPIOB
#define led_mute2_Pin GPIO_PIN_15
#define led_mute2_GPIO_Port GPIOB
#define led_mute1_Pin GPIO_PIN_8
#define led_mute1_GPIO_Port GPIOA
#define scl_Pin GPIO_PIN_9
#define scl_GPIO_Port GPIOA
#define sda_Pin GPIO_PIN_10
#define sda_GPIO_Port GPIOA
#define dm_Pin GPIO_PIN_11
#define dm_GPIO_Port GPIOA
#define dp_Pin GPIO_PIN_12
#define dp_GPIO_Port GPIOA
#define swdio_Pin GPIO_PIN_13
#define swdio_GPIO_Port GPIOA
#define swclk_Pin GPIO_PIN_14
#define swclk_GPIO_Port GPIOA
#define led_mute0_Pin GPIO_PIN_15
#define led_mute0_GPIO_Port GPIOA
#define mute1_Pin GPIO_PIN_3
#define mute1_GPIO_Port GPIOB
#define mute0_Pin GPIO_PIN_4
#define mute0_GPIO_Port GPIOB
#define dbg_io1_Pin GPIO_PIN_5
#define dbg_io1_GPIO_Port GPIOB
#define capsense0_Pin GPIO_PIN_6
#define capsense0_GPIO_Port GPIOB
#define dbg_io0_Pin GPIO_PIN_7
#define dbg_io0_GPIO_Port GPIOB
#define dbg_rx_Pin GPIO_PIN_8
#define dbg_rx_GPIO_Port GPIOB
#define bp_power_Pin GPIO_PIN_9
#define bp_power_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
