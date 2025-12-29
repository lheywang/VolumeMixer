/** ================================================================
 * @file    handlers/tsc.c
 *
 * @brief   Handle the TSC by defining functions about reading a channels, and so on.
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
#include "handlers/tsc.h"

#include "stm32f3xx_hal.h"

#include <stdint.h>

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
// Public
int tsc_status = 0; // Store the ON/OFF states

// Privates
static int tsc_ack = 0; // Store the buttons that already updated the states.

static uint32_t tsc_masks[TSC_NUM_CHANNELS] = {
	  TSC_GROUP5_IO2, // Mute 0
	  TSC_GROUP5_IO1, // Mute 1
	  TSC_GROUP6_IO1, // Mute 2
	  TSC_GROUP6_IO3, // Mute 3
	  TSC_GROUP6_IO2, // Mute 4
};
static uint32_t tsc_channels[TSC_NUM_CHANNELS] = {
	  TSC_GROUP5_IO3,
	  TSC_GROUP5_IO3,
	  TSC_GROUP6_IO4,
	  TSC_GROUP6_IO4,
	  TSC_GROUP6_IO4
};
int tsc_active_channel = 0;

TSC_ChannelState tsc_state[TSC_NUM_CHANNELS] = {0};

// Imported :
extern TSC_HandleTypeDef htsc;

extern uint8_t msg_available;

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */

int tsc_configure_next_channel(void)
{

  TSC_IOConfigTypeDef io = {0};

  io.ChannelIOs = tsc_masks[tsc_active_channel];
  io.SamplingIOs = tsc_channels[tsc_active_channel];

  // Configure the TSC module
  HAL_TSC_IOConfig(&htsc, &io);
  HAL_TSC_IODischarge(&htsc, ENABLE);
  for (int k = 0; k < 1000000; k++){}

  // Start the acquisition
  HAL_TSC_IODischarge(&htsc, DISABLE);
  HAL_TSC_Start_IT(&htsc);

  return 0;
}


uint8_t tsc_read_channel(void)
{
	/*
	 * First, ensure the TSC peripheral did finish it's job.
	 * We can directly read, because this function is called within the END of CONVERSION ISR context,
	 * so the resuls ARE available.
	 */
	uint16_t value = 0;
	switch (tsc_channels[tsc_active_channel])
	{
	case TSC_GROUP5_IO3:
		value = HAL_TSC_GroupGetValue(&htsc, TSC_GROUP5_IDX);
		break;
	case TSC_GROUP6_IO4:
		value = HAL_TSC_GroupGetValue(&htsc, TSC_GROUP6_IDX);
		break;
	default:
		value = 0;
		break;
	}
	HAL_TSC_Stop(&htsc);

	/*
	 * Performing some sort of filtering to remove any unwanted elements (Tempco, voltages...)
	 */
	if (tsc_state[tsc_active_channel].baseline == 0)
	{
		tsc_state[tsc_active_channel].baseline = value;
		tsc_state[tsc_active_channel].filtered = value;
	}

	tsc_state[tsc_active_channel].filtered = (tsc_state[tsc_active_channel].filtered + value) / 2;

	/*
	 * Using the ration between filtered and baseline (always < 1), we can
	 * safely decide if the pad was touched, or not.
	 */
    float ratio = 	(float)tsc_state[tsc_active_channel].filtered /
    				(float)tsc_state[tsc_active_channel].baseline;

    if (ratio < TSC_RELEASE_THRESHOLD)
    {
    	tsc_state[tsc_active_channel].pressed = 1;
    }
    else
    {
    	tsc_state[tsc_active_channel].pressed = 0;
    }

	/*
	 * Finally, update the status register
	 */
    uint8_t mask = 0x00;
    switch (tsc_masks[tsc_active_channel])
	{
    case TSC_GROUP5_IO2 : // Mute 0
    	mask = TACTILE_BUTTON_1;
    	break;
    case TSC_GROUP5_IO1 : // Mute 1
    	mask = TACTILE_BUTTON_2;
    	break;
    case TSC_GROUP6_IO1 : // Mute 2
    	mask = TACTILE_BUTTON_3;
    	break;
    case TSC_GROUP6_IO3 : // Mute 3
    	mask = TACTILE_BUTTON_4;
    	break;
    case TSC_GROUP6_IO2 :  // Mute 4
    	mask = TACTILE_BUTTON_5;
    	break;
	}

    // Update the status register
    if (tsc_state[tsc_active_channel].pressed)
    {
    	if ((tsc_ack & mask) == 0x00)
    	{
    		if ((tsc_status & mask) == 0x00)
    			tsc_status |= mask; // Set the bit
    		else
    			tsc_status &= ~mask; // Clear the bit

    		tsc_ack |= mask; // Set the ACK bit
    	}
    }
    else
    {
    	tsc_ack &= ~mask; // Clear the bit
    }

	// Update the baseline only when NOT pressed (slow enough to only follow low speed variations (temperature, voltages...)
	if (tsc_state[tsc_active_channel].pressed == 0)
	{
		tsc_state[tsc_active_channel].baseline = (uint16_t)(
				tsc_state[tsc_active_channel].baseline * TSC_MOVING_AVERAGE_FACTOR +
				tsc_state[tsc_active_channel].filtered * (1 - TSC_MOVING_AVERAGE_FACTOR)
		);
	}

    // Increment the active channel.
    tsc_active_channel = (tsc_active_channel + 1) % TSC_NUM_CHANNELS;

	return 0;
}

void EXTI2_TSC_IRQHandler(void)
{
    HAL_TSC_IRQHandler(&htsc);
}

void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef *htsc)
{
	UNUSED(htsc);

	tsc_read_channel();
	tsc_configure_next_channel();
	return;
}





