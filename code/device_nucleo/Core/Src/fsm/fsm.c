/** ================================================================
 * @file    fsm/fsm.c
 *
 * @brief   Source file of the state machine that control the whole
 * 			device
 *
 * @date 	24-09-2025
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
// Header
#include "fsm/fsm.h"

// External libraries
#include <parser/parser.h>
#include <eeprom/eeprom.h>
#include <filter/filter.h>
#include <screen/screen.h>
#include <maths/maths.h>
#include <screen/data/icons.h>

#include "utils/CDC_helper.h"

// HAL
#include "stm32f3xx_hal.h"
#include "usbd_cdc_if.h"

// STD
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Import externs from the HAL
extern ADC_HandleTypeDef hadc1;

// Import extern variables.
extern struct EEPROM_Header header;
extern struct EEPROM_hash hashs;

/*
 * -----------------------------------------------------------------
 * Defines
 * -----------------------------------------------------------------
 */
/*
 * Configure the used ADC channels.
 */
#ifdef DEBUG
#define ADC_CHANNELS_NB 5
#define ADC_SLIDER_NB 2
static const uint32_t channels[ADC_CHANNELS_NB] = {
		ADC_CHANNEL_1,
		ADC_CHANNEL_2,
		ADC_CHANNEL_TEMPSENSOR,
		ADC_CHANNEL_VREFINT,
		ADC_CHANNEL_VBAT
};
#else
#define ADC_CHANNELS_NB 11
#define ADC_SLIDER_NB 5
static const uint32_t channels[ADC_CHANNELS_NB] = {
		ADC_CHANNEL_1,
		ADC_CHANNEL_2,
		ADC_CHANNEL_3,
		ADC_CHANNEL_4,
		ADC_CHANNEL_5,
		ADC_CHANNEL_10,
		ADC_CHANNEL_11,
		ADC_CHANNEL_15,
		ADC_CHANNEL_TEMPSENSOR,
		ADC_CHANNEL_VREFINT,
		ADC_CHANNEL_VBAT
};
#endif

/*
 * -----------------------------------------------------------------
 * Private prototypes
 * -----------------------------------------------------------------
 */
/*
 * Handler for the generic "more advanced commands"
 */
void fsm_handle_command();

/*
 * Handler for each commands
 */
void __fsm_handle_shutd();
void __fsm_handle_reset();
void __fsm_handle_async();
void __fsm_handle_uicon();
void __fsm_handle_slpos();
void __fsm_handle_dconf();
void __fsm_handle_connc();

/*
 * Acquisition functions
 */
void __acquire_next_channel();

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
// Shared variables (extern...)
uint8_t RxBuffer[CDC_BUFFER_SIZE] = { 0 }; 	// Buffer for reading messages
uint8_t TxBuffer[CDC_BUFFER_SIZE] = { 0 }; 	// Buffer for transmitting messages back
volatile uint16_t rx_len = 0;  				// Number of bytes currently in the buffer
volatile bool 	  msg_complete = false;

// Private variables
static struct CMD command;
static int pval;

// FSM State
static enum fsm_state state = BOARD_INIT;
static bool device_connected = false;

// Channel positions
static int slider_positions[5] = { 0 };
static float adc_utilities[6] = { 0 };
static uint8_t actual_slider = 0;

/*
 * -----------------------------------------------------------------
 * Public Functions
 * -----------------------------------------------------------------
 */
void fsm_init()
{
	// Clear the memory
	memset((void*)&command, 0x00, sizeof(command));

	// Clear the buffers for the CDC reception
	memset((void*)RxBuffer, 0x00, sizeof(RxBuffer));
	memset((void*)TxBuffer, 0x00, sizeof(TxBuffer));
	rx_len = 0;
	msg_complete = false;

	// Set the internal variables
	pval = 0;

	// Finally, enable the parsing of message since the board is initialized.
	state = WAIT_FOR_CONNECT;
	return;
}

void fsm_update()
{
	// Check if the system was initialized. If not, call the init and return (we'll act on the next cycle).
	if (state == BOARD_INIT)
	{
		fsm_init();
		return;
	}

	/*
	 * The USB CDC Interrupt is filling the buffer, and setting the msg_complete flag once it detect ";END" as the last
	 * four characters.
	 *
	 * Then, we know that the buffer is complete, and thus, can process it, here, in the main loop.
	 */
	if (msg_complete == true)
	{
		/*
		 *	We're already waiting for an answer, so we won't overwrite the previous parsing results.
		 */
		if (state == WAIT_FOR_RESPONSE)
		{
			memset((void*)RxBuffer, 0x00, sizeof(RxBuffer));
			rx_len = 0;
			msg_complete = false;
			return;
		}
		// Reset the command struct and parse the received message :
		memset((void*)&command, 0x00, sizeof(command));
		pval = parser((char*)RxBuffer, &command, 1);
		command.direction = RX;

		// In any cases, reset the memory buffer and the pointer to the different write locations.
		memset((void*)RxBuffer, 0x00, sizeof(RxBuffer));
		rx_len = 0;
		msg_complete = false;

		/*
		 * Then, assign the result variable if the parser was not sucessfull.
		 * We don't want to process incorrect data ! Otherwise, leave as before !
		 */
		command.result = (pval != 0) ? NACK : command.result;

		/*
		 * Finally, handle the state evolution
		 */
		switch(state)
		{
		/*
		 * If we're waiting for a connection, reject any other command.
		 * Else, update the variable to a new state
		 */
		case WAIT_FOR_CONNECT:
			if (command.type != CONNC)
			{
				command.result = NACK;
			}
			else
			{
				state = WAIT_FOR_RESPONSE;
			}
			break;
		/*
		 * If we're waiting for command, set us to get the response
		 */
		case WAIT_FOR_COMMAND:
			state = WAIT_FOR_RESPONSE;
			break;

		/*
		 * Any other state are not handled, because :
		 * - BOARD_INIT is removed by the fsm_init() function, and checked at the start
		 * 	 of the fsm_update() function. So, this is, in fact an impossible state.
		 */
		case BOARD_INIT:
		case WAIT_FOR_RESPONSE:
		default:
			break;
		}
	}

	/*
	 * if a command need an answer, call the command handler.
	 */
	if (state == WAIT_FOR_RESPONSE)
	{
		fsm_handle_command();
	}
	else if (state== WAIT_FOR_CONNECT)
	{
		// Send a default string, to enable the demon to identify the device
		snprintf((char*)TxBuffer, sizeof(TxBuffer), "VolumeMixer-Waiting_for_connection...\n");
		CDC_Transmit_Long(TxBuffer,sizeof(TxBuffer));

		// 500 ms of delay to not spam the host
		HAL_Delay(500);
	}

	/*
	 * Finally, trigger the acquisition of the sliders positions
	 *
	 * Note : this function require N calls to complete an aquisition, we're thus going to
	 */
	__acquire_next_channel();

#ifdef DEBUG
	/*
	 * This block if code is only shown in DEBUG binaries, and use to show the different values seen on the ADC.
	 */
	if (actual_slider == 0)
	{
		snprintf(	(char*)TxBuffer,
					sizeof(TxBuffer),
					"Values : \n\tCH1 : %3d \n\tCH2 : %3d \n\tCH3 : %3d \n\tCH4 : %3d \n\tCH5 : %3d \n\n\tREFD : %3d\n\tREFA : %3d \n\tREFU : %3d \n\tTEMP : %3d \n\tINT : %3d \n\tBAT : %3d\n",
					slider_positions[0], // CH1
					slider_positions[1], // CH2
					slider_positions[2], // CH3
					slider_positions[3], // CH4
					slider_positions[4], // CH5
					(int)adc_utilities[0] * 100, // REFD
					(int)adc_utilities[1] * 100, // REFA
					(int)adc_utilities[2] * 100, // REFU
					(int)adc_utilities[3] * 100, // TEMP
					(int)adc_utilities[4] * 100, // INT
					(int)adc_utilities[5] * 100);// BAT
		CDC_Transmit_Long(TxBuffer,sizeof(TxBuffer));
		HAL_Delay(500);
	}
#endif

	return;
}

/*
 * -----------------------------------------------------------------
 * Private functions
 * -----------------------------------------------------------------
 */
void fsm_handle_command()
{
	/*
	 * Depending on the command type, call the correct handler
	 */
	switch (command.type)
	{
	case SHUTD:
		__fsm_handle_shutd();
		break;

	case RINIT:
		__fsm_handle_reset();
		break;

	case ASYNC:
		__fsm_handle_async();
		break;

	case UICON:
		__fsm_handle_uicon();
		break;

	case SLPOS:
		__fsm_handle_slpos();
		break;

	case DCONF:
		__fsm_handle_dconf();
		break;

	case CONNC:
		__fsm_handle_connc();
		break;

	default:
		break;
	}

	/*
	 *	If the state changed before the fsm_handle_command function call and now,
	 *	we know that the handler has finished it's job (some may need multiple cycles).
	 *
	 *	Thus, we can safely call the builder to create a buffer, before to send it
	 *	on the USB bus as a response.
	 */
	if (state != WAIT_FOR_RESPONSE)
	{
		builder(&command, (char*)TxBuffer);
		CDC_Transmit_Long(TxBuffer,sizeof(TxBuffer));
	}

	/*
	 * Exit the command handler.
	 */
	return;
}

/*
 * -----------------------------------------------------------------
 * Dedicated command handlers
 * -----------------------------------------------------------------
 */
/*
 * Shutdown the device (how ?).
 */
void __fsm_handle_shutd()
{
	return;
}

/*
 * Perform a soft reset of the device, to restart anything.
 * Since the code un USBDEVICE has been slightly modified, this will
 * also trigger an USB reenumeration process on the host.
 */
void __fsm_handle_reset()
{
	return;
}

/*
 * Perform a sync between the shown apps on the device,
 * and the ones needed for the host.
 */
void __fsm_handle_async()
{
	return;
}

/*
 * Perform the changements of the icon shown on the OLED screen.
 */
void __fsm_handle_uicon()
{
	return;
}

/*
 * Return the position of the different sliders.
 */
void __fsm_handle_slpos()
{
	return;
}

/*
 * Configure the device.
 */
void __fsm_handle_dconf()
{
	return;
}

/*
 * Get the status of the device
 */
void __fsm_handle_connc()
{
	/*
	 * If the device is NOT connected : connect it, and answer OK.
	 * If we're already connected, return NACK.
	 *
	 * In any cases, we update the state to indicate that the command handling is done,
	 * and that we can send back the response.
	 */
	if (device_connected == false)
	{
		device_connected = true;
		command.result = OK;
		state = WAIT_FOR_COMMAND;
	}
	else
	{
		command.result = NACK;
		state = WAIT_FOR_COMMAND;
	}
	return;
}

/*
 * -----------------------------------------------------------------
 * Utility functions
 * -----------------------------------------------------------------
 */
void __acquire_next_channel()
{
	/*
	 * First, read back the ADC value
	 */
	ADC_ChannelConfTypeDef sConfig = {0};
	uint16_t value = 0;

	// Configure the desired channel
	sConfig.Channel      = channels[actual_slider];
	sConfig.Rank         = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_19CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	// Start conversion
	HAL_ADC_Start(&hadc1);
	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
	{
		value = (uint16_t)HAL_ADC_GetValue(&hadc1);
	}
	HAL_ADC_Stop(&hadc1);

	/*
	 * Applying a low pass filter on the data.
	 * This smooth the response to get something cleaner, and which won't move continously.
	 *
	 * The full step response of this filter is about 70 ms.
	 */
	value = filter(value, actual_slider);

	/*
	 * Then, convert the value into the position.
	 * And, we store it into the right tab, for further usage.
	 */
	if (actual_slider < ADC_SLIDER_NB)
	{
		int position = ADC2POS(value, actual_slider);
		slider_positions[actual_slider] = position;
	}
	else
	{
		ADC2Double((int16_t)value, &adc_utilities[actual_slider - ADC_SLIDER_NB]);
	}

	/*
	 * Increment the actual_slider value, to sample the next channel on the next call.
	 * If value shall overflow, reset it to 0.
	 */
	actual_slider = (actual_slider > (ADC_CHANNELS_NB - 1)) ? 0 : (actual_slider + 1);
	return;
}

/*
  // here only to include the right symbols into the build system...
   * 		builder(&command, (char*)TxBuffer);
		CDC_Transmit_Long(TxBuffer,sizeof(TxBuffer));

  command.direction = RX;
  parser(br->buffer, &command, 1);
  builder(&command, br->buffer);

  uint8_t buf[128] = { 0 };
  parse_raw_eeprom_header(buf);
  build_raw_eeprom_header(buf);

  struct ScreenOrder cmd = {
  		.type = FULL,
  		.status = MUTE,
  		.volume = 01,
  		.icon ={
  				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  				0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00,
  				0x00, 0x1e, 0x78, 0x00, 0x00, 0x03, 0xc0, 0x00,
  				0x80, 0xc1, 0x83, 0x01, 0xc0, 0xf8, 0x1f, 0x03,
  				0x60, 0x0c, 0x30, 0x06, 0x30, 0x06, 0x60, 0x0c,
  				0x10, 0x03, 0xc0, 0x08, 0x98, 0x01, 0x80, 0x19,
  				0x98, 0xc0, 0x03, 0x19, 0x88, 0x60, 0x06, 0x11,
  				0xc8, 0x30, 0x0c, 0x13, 0xc8, 0x10, 0x08, 0x13,
  				0xc8, 0x10, 0x08, 0x13, 0xc8, 0x30, 0x0c, 0x13,
  				0x88, 0x60, 0x06, 0x11, 0x98, 0xc0, 0x1b, 0x19,
  				0x98, 0x01, 0xb8, 0x19, 0x10, 0x03, 0x70, 0x08,
  				0x30, 0x06, 0xe0, 0x0c, 0x60, 0x0c, 0xd0, 0x05,
  				0xc0, 0xf8, 0x9f, 0x03, 0x80, 0xc1, 0x03, 0x07,
  				0x00, 0x03, 0xc0, 0x0e, 0x00, 0x1e, 0x78, 0x0c,
  				0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
  				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  			}
  		};

  	struct BufferRequest *br = draw_buffer(&cmd);

  	// Show the buffer on console
  	SSD1306_SendBuffer(br->buffer);
  	SSD1306_UpdateScreen();


    Infinite loop
     USER CODE BEGIN WHILE
    uint8_t TxBuffer[] = "Hello World from STM32 CDC VCOM !\n\r";
    uint8_t Len = sizeof(TxBuffer);

	  HAL_Delay(500);
	  int val = CDC_Transmit_FS(TxBuffer, Len) + 0x30;
*/


