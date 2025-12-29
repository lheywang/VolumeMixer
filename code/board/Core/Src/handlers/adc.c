/** ================================================================
 * @file    handlers/adc.c
 *
 * @brief   Implement the ADC functions.
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
#include "stm32f3xx_hal.h"

#include "lib/filter/filter.h"
#include "lib/maths/maths.h"

/*
 * -----------------------------------------------------------------
 * Defines
 * -----------------------------------------------------------------
 */
#define ADC_NUM_CHANNELS 8

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
// Private
static uint32_t adc_channels[ADC_NUM_CHANNELS] = {
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
	ADC_CHANNEL_10,
	ADC_CHANNEL_11,
	ADC_CHANNEL_15
};

// Public
int adc_active_channel = 0;

// Imported
// From main.c
extern ADC_HandleTypeDef hadc1;

// From fsm/fsm.c
extern int slider_positions[5];
extern float adc_utilities[3];

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
int adc_configure_next_channel(void)
{
	// Configuring the ADC :
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = adc_channels[adc_active_channel];
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;

    // Update the ADC
    HAL_ADC_Stop(&hadc1);
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    HAL_ADC_Start(&hadc1); // To gain some time, we start the acquisition now, and we will fetch the value afterward.

    return 0;
}

uint16_t adc_read_channel(void)
{
	// Fetcht the value from the ADC
	uint16_t value = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	/*
	 * Decide where to store the data
	 */
	switch (adc_channels[adc_active_channel])
	{
	case ADC_CHANNEL_1 :
		slider_positions[0] = ADC2POS(
				filter(
						value,
						adc_active_channel
				),
				1
		);
		break;
	case ADC_CHANNEL_2 :
		slider_positions[1] = ADC2POS(
				filter(
						value,
						adc_active_channel
				),
				2
		);
		break;
	case ADC_CHANNEL_3 :
		slider_positions[2] = ADC2POS(
				filter(
						value,
						adc_active_channel
				),
				3
		);
		break;
	case ADC_CHANNEL_4 :
		slider_positions[3] = ADC2POS(
				filter(
						value,
						adc_active_channel
				),
				4);
		break;
	case ADC_CHANNEL_5 :
		slider_positions[4] = ADC2POS(
				filter(
						value,
						adc_active_channel
				),
				5
		);
		break;

	/*
	 * We calculate in mV rather than V, because this give us more error room for maths ceils / floor. And, then we
	 */
	case ADC_CHANNEL_10 :
		adc_utilities[0] = (float)(filter(value, adc_active_channel) * 3300 * 2) / 4095;
		break;
	case ADC_CHANNEL_11 :
		adc_utilities[1] = (float)(filter(value, adc_active_channel) * 3300 * 2) / 4095;
		break;
	case ADC_CHANNEL_15 :
		adc_utilities[2] = (float)(filter(value, adc_active_channel) * 3300 * 2) / 4095;
		break;
	}

    // Updating the active channel
    adc_active_channel = (adc_active_channel + 1) % ADC_NUM_CHANNELS;

	return value;
}

