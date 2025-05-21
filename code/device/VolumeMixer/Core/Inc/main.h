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
#define PowerOnLed_Pin GPIO_PIN_13
#define PowerOnLed_GPIO_Port GPIOC
#define DeviceErrorLed_Pin GPIO_PIN_14
#define DeviceErrorLed_GPIO_Port GPIOC
#define DeviceConnectedLed_Pin GPIO_PIN_15
#define DeviceConnectedLed_GPIO_Port GPIOC
#define ClkIn_Pin GPIO_PIN_0
#define ClkIn_GPIO_Port GPIOF
#define DBG_BP_Pin GPIO_PIN_1
#define DBG_BP_GPIO_Port GPIOF
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
#define DBG_LED_Pin GPIO_PIN_5
#define DBG_LED_GPIO_Port GPIOA
#define Vrefdigital_Pin GPIO_PIN_6
#define Vrefdigital_GPIO_Port GPIOA
#define VrefSupply_Pin GPIO_PIN_7
#define VrefSupply_GPIO_Port GPIOA
#define VrefPot_Pin GPIO_PIN_0
#define VrefPot_GPIO_Port GPIOB
#define Mute3Led_Pin GPIO_PIN_1
#define Mute3Led_GPIO_Port GPIOB
#define Mute4Led_Pin GPIO_PIN_2
#define Mute4Led_GPIO_Port GPIOB
#define DBG_TX_Pin GPIO_PIN_10
#define DBG_TX_GPIO_Port GPIOB
#define Slider3Mute_Pin GPIO_PIN_11
#define Slider3Mute_GPIO_Port GPIOB
#define Slider4Mute_Pin GPIO_PIN_12
#define Slider4Mute_GPIO_Port GPIOB
#define Slider5Mute_Pin GPIO_PIN_13
#define Slider5Mute_GPIO_Port GPIOB
#define TSC_C2_Pin GPIO_PIN_14
#define TSC_C2_GPIO_Port GPIOB
#define Mute2Led_Pin GPIO_PIN_15
#define Mute2Led_GPIO_Port GPIOB
#define Mute1Led_Pin GPIO_PIN_8
#define Mute1Led_GPIO_Port GPIOA
#define SCL_Pin GPIO_PIN_9
#define SCL_GPIO_Port GPIOA
#define SDA_Pin GPIO_PIN_10
#define SDA_GPIO_Port GPIOA
#define D__Pin GPIO_PIN_11
#define D__GPIO_Port GPIOA
#define D_A12_Pin GPIO_PIN_12
#define D_A12_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define Mute0Led_Pin GPIO_PIN_15
#define Mute0Led_GPIO_Port GPIOA
#define Slider1Mute_Pin GPIO_PIN_3
#define Slider1Mute_GPIO_Port GPIOB
#define Slider2Mute_Pin GPIO_PIN_4
#define Slider2Mute_GPIO_Port GPIOB
#define DBG_IO1_Pin GPIO_PIN_5
#define DBG_IO1_GPIO_Port GPIOB
#define TSC_C1_Pin GPIO_PIN_6
#define TSC_C1_GPIO_Port GPIOB
#define DBG_IO0_Pin GPIO_PIN_7
#define DBG_IO0_GPIO_Port GPIOB
#define DBG_RX_Pin GPIO_PIN_8
#define DBG_RX_GPIO_Port GPIOB
#define PowerOnBP_Pin GPIO_PIN_9
#define PowerOnBP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
