/** ================================================================
 * @file    Core/Src/hal/init.cpp
 *
 * @brief   init.cpp Implement low level initialization steps of the MCU.
 *
 * @date    26-05-2025
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
// Header file
#include "hal/init.hpp"

// Other headers
#include "hal/hal.hpp"

// Libs
#include "stm32f3xx_hal.h"
#include "main.h"
#include "core_cm4.h"

/* -----------------------------------------------------------------
 * VARIABLES
 * -----------------------------------------------------------------
 */

ADC_HandleTypeDef init::hadc1;
I2C_HandleTypeDef init::hi2c2;
TSC_HandleTypeDef init::htsc;
UART_HandleTypeDef init::huart3;
PCD_HandleTypeDef init::hpcd_USB_FS;

/* -----------------------------------------------------------------
 * INITIALIZATION FUNCTIONS
 * -----------------------------------------------------------------
 */

void init::SystemClock_Config(void){
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	init::Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		init::Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_I2C2
							  |RCC_PERIPHCLK_ADC1;
	PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_HSI;
	PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	PeriphClkInit.Adc1ClockSelection = RCC_ADC1PLLCLK_DIV1;

	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		init::Error_Handler();
	}

	/** Enables the Clock Security System
	*/
	HAL_RCC_EnableCSS();
}

void init::MX_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, led_on_Pin|led_connect_Pin|led_fail_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, dbg_led_Pin|led_mute1_Pin|led_mute0_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, led_mute3_Pin|led_mute4_Pin|led_mute2_Pin|dbg_io0_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : led_on_Pin led_connect_Pin led_fail_Pin */
	GPIO_InitStruct.Pin = led_on_Pin|led_connect_Pin|led_fail_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : dbg_bp_Pin */
	GPIO_InitStruct.Pin = dbg_bp_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(dbg_bp_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : dbg_led_Pin led_mute1_Pin led_mute0_Pin */
	GPIO_InitStruct.Pin = dbg_led_Pin|led_mute1_Pin|led_mute0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : led_mute3_Pin led_mute4_Pin led_mute2_Pin dbg_io0_Pin */
	GPIO_InitStruct.Pin = led_mute3_Pin|led_mute4_Pin|led_mute2_Pin|dbg_io0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : dbg_io1_Pin bp_power_Pin */
	GPIO_InitStruct.Pin = dbg_io1_Pin|bp_power_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}
void init::MX_ADC_Init(void){
	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */

	/** Common config
	*/
	init::hadc1.Instance = ADC1;
	init::hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	init::hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	init::hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	init::hadc1.Init.ContinuousConvMode = DISABLE;
	init::hadc1.Init.DiscontinuousConvMode = DISABLE;
	init::hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	init::hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	init::hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	init::hadc1.Init.NbrOfConversion = 1;
	init::hadc1.Init.DMAContinuousRequests = DISABLE;
	init::hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	init::hadc1.Init.LowPowerAutoWait = DISABLE;
	init::hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	if (HAL_ADC_Init(&init::hadc1) != HAL_OK)
	{
		init::Error_Handler();
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&init::hadc1, &sConfig) != HAL_OK)
	{
		init::Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */
}

void init::MX_I2C_Init(void){
	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	init::hi2c2.Instance = I2C2;
	init::hi2c2.Init.Timing = 0x00201D2B;
	init::hi2c2.Init.OwnAddress1 = 0;
	init::hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	init::hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	init::hi2c2.Init.OwnAddress2 = 0;
	init::hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	init::hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	init::hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&init::hi2c2) != HAL_OK)
	{
		init::Error_Handler();
	}

	/** Configure Analogue filter
	*/
	if (HAL_I2CEx_ConfigAnalogFilter(&init::hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		init::Error_Handler();
	}

	/** Configure Digital filter
	*/
	if (HAL_I2CEx_ConfigDigitalFilter(&init::hi2c2, 0) != HAL_OK)
	{
		init::Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */
}

void init::MX_TSC_Init(void){

	/* USER CODE BEGIN TSC_Init 0 */

	/* USER CODE END TSC_Init 0 */

	/* USER CODE BEGIN TSC_Init 1 */

	/* USER CODE END TSC_Init 1 */

	/** Configure the TSC peripheral
	*/
	init::htsc.Instance = TSC;
	init::htsc.Init.CTPulseHighLength = TSC_CTPH_2CYCLES;
	init::htsc.Init.CTPulseLowLength = TSC_CTPL_2CYCLES;
	init::htsc.Init.SpreadSpectrum = DISABLE;
	init::htsc.Init.SpreadSpectrumDeviation = 1;
	init::htsc.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
	init::htsc.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV4;
	init::htsc.Init.MaxCountValue = TSC_MCV_8191;
	init::htsc.Init.IODefaultMode = TSC_IODEF_OUT_PP_LOW;
	init::htsc.Init.SynchroPinPolarity = TSC_SYNC_POLARITY_FALLING;
	init::htsc.Init.AcquisitionMode = TSC_ACQ_MODE_NORMAL;
	init::htsc.Init.MaxCountInterrupt = DISABLE;
	init::htsc.Init.ChannelIOs = TSC_GROUP5_IO1|TSC_GROUP5_IO2|TSC_GROUP6_IO1|TSC_GROUP6_IO2
					|TSC_GROUP6_IO3;
	init::htsc.Init.ShieldIOs = 0;
	init::htsc.Init.SamplingIOs = TSC_GROUP5_IO3|TSC_GROUP6_IO4;
	if (HAL_TSC_Init(&init::htsc) != HAL_OK)
	{
		init::Error_Handler();
	}
	/* USER CODE BEGIN TSC_Init 2 */

	/* USER CODE END TSC_Init 2 */

}

void init::MX_USART_UART_Init(void){

	/* USER CODE BEGIN USART3_Init 0 */

	/* USER CODE END USART3_Init 0 */

	/* USER CODE BEGIN USART3_Init 1 */

	/* USER CODE END USART3_Init 1 */
	init::huart3.Instance = USART3;
	init::huart3.Init.BaudRate = 115200;
	init::huart3.Init.WordLength = UART_WORDLENGTH_8B;
	init::huart3.Init.StopBits = UART_STOPBITS_1;
	init::huart3.Init.Parity = UART_PARITY_NONE;
	init::huart3.Init.Mode = UART_MODE_TX_RX;
	init::huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	init::huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	init::huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	init::huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&init::huart3) != HAL_OK)
	{
		init::Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */

	/* USER CODE END USART3_Init 2 */

}

void init::MX_USB_PCD_Init(void){
	/* USER CODE BEGIN USB_Init 0 */

	/* USER CODE END USB_Init 0 */

	/* USER CODE BEGIN USB_Init 1 */

	/* USER CODE END USB_Init 1 */
	init::hpcd_USB_FS.Instance = USB;
	init::hpcd_USB_FS.Init.dev_endpoints = 8;
	init::hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
	init::hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
	init::hpcd_USB_FS.Init.low_power_enable = DISABLE;
	init::hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
	if (HAL_PCD_Init(&init::hpcd_USB_FS) != HAL_OK)
	{
		init::Error_Handler();
	}
	/* USER CODE BEGIN USB_Init 2 */

	/* USER CODE END USB_Init 2 */
}

void init::full(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	init::SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	init::MX_GPIO_Init();
	init::MX_ADC_Init();
	init::MX_I2C_Init();
	init::MX_TSC_Init();
	init::MX_USART_UART_Init();
	init::MX_USB_PCD_Init();
}

void init::Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  uint8_t *data = (uint8_t*)"ERROR ! \r\n";
  HAL_UART_Transmit(&init::huart3, data, 10, 100);
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

