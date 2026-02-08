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
#include "lib/parser/parser.h"
#include "lib/eeprom/eeprom.h"
#include "lib/filter/filter.h"
#include "lib/screen/screen.h"
#include "lib/maths/maths.h"
#include "lib/screen/data/icons.h"

#include "handlers/leds.h"
#include "handlers/tsc.h"
#include "handlers/timers.h"

#include "utils/CDC_helper.h"
#include "utils/logger.h"
#include "utils/init.h"

#include "drivers/ssd1306.h"
#include "drivers/24cl64.h"
#include "drivers/tca9548.h"

// HAL
#include "stm32f3xx_hal.h"
#include "usbd_cdc_if.h"

// STD
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Import externs from the HAL
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart3;

// Import extern variables.
extern struct EEPROM_Header header;
extern struct EEPROM_hash hashs;

// UART logs
extern uint8_t isr_message[ISR_MESSAGE_SIZE];
extern uint8_t msg_available;

/*
 * -----------------------------------------------------------------
 * Private prototypes
 * -----------------------------------------------------------------
 */
/*
 * Handler for the generic "more advanced commands"
 */
static void fsm_handle_command(void);

/*
 * Check if a refresh of the screen are needed (IE : the volumes have changed / icon and so on).
 */
static uint32_t fsm_refresh_needed(void);

/*
 * Handler for each commands
 */
static void __fsm_handle_shutd(void);
static void __fsm_handle_reset(void);
static void __fsm_handle_async(void);
static void __fsm_handle_uicon(void);
static void __fsm_handle_slpos(void);
static void __fsm_handle_dconf(void);
static void __fsm_handle_connc(void);
static void __fsm_refresh_screen(uint32_t refresh);


/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
/*
 * Shared variables from and to the USB CDC device.
 * Declared as volatile because the ISR can fill / change them !
 */
uint8_t  RxBuffer[CDC_BUFFER_SIZE] = { 0 };
uint8_t  TxBuffer[CDC_BUFFER_SIZE] = { 0 };
volatile uint16_t rx_len = 0;
volatile bool 	  msg_complete = false;

/*
 * FSM Control variables
 */
static struct CMD command;
static int pval;

// FSM State
static enum fsm_state state = BOARD_INIT;
static bool device_connected = false;

/*
 * Data variables
 */
int slider_positions[5] = { 0 };
float adc_utilities[3] = { 0 };
uint8_t tsc_buttons = 0;

// Audio values to be returned
int output_volumes[5] = { 0 };
int output_status[5] = { 0 };

// Actually displayed apps :
uint32_t active_apps[5] = { 0 };

// Struct to store the screen update logic
struct ScreenOrder screen_cmd = { 0 };
struct BufferRequest *br = (void *)0; // This pointer will be filled by after

// Array to store the actually displayed images :
uint8_t images[5][128] = { 0 };

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
	tsc_buttons = 0x1F; // Load all channels enabled

	br = (void *)0;

	// Initialise static variables
	memset((void *)slider_positions, 0x00, (size_t)sizeof(slider_positions));
	memset((void *)adc_utilities, 0x00, (size_t)sizeof(adc_utilities));
	memset((void *)output_volumes, 0x00, (size_t)sizeof(output_volumes));
	memset((void *)output_status, 0x00, (size_t)sizeof(output_status));
	memset((void *)active_apps, 0x00, (size_t)sizeof(active_apps));
	memset((void *)images, 0x00, (size_t)sizeof(images));

	// Initialize the screencmd struct
	memset((void *)&screen_cmd, 0x00, (size_t)sizeof(screen_cmd));

	// Finally, enable the parsing of message since the board is initialized.
	state = WAIT_FOR_CONNECT;
	device_connected = false;

	/*
	 * Force a screen refresh on the init
	 */
	__fsm_refresh_screen(0xFFFFFFFF);
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


		if (pval != 0)
		{
			/*
			 * Log the message to the UART console
			 */
			int size = 0;
			size = snprintf((char *)isr_message, (size_t)sizeof(isr_message), "[  FSM   ] An error occured during the parsing of the command : %d (ID = %d)\n\r", pval, command.type);
			uart_log(isr_message, size);
			memset((void *)isr_message, 0x00, (size_t)sizeof(isr_message));

			/*
			 * Jump to the board update routine, since we don't want
			 * to handle an incorrectly parsed command !
			 */
			goto board_update;
		}

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
		int size = 0;
		size = snprintf((char*)TxBuffer, sizeof(TxBuffer), "VolumeMixer-Waiting_for_connection...\n\r");

		if (size > 0)
			CDC_Transmit_Long(TxBuffer, size);

		// 500 ms of delay to not spam the host
		HAL_Delay(500);
	}

board_update:

	/*
	 * Update the leds status
	 */
	set_pwm(LED_MUTE_0, (tsc_status & TACTILE_BUTTON_1) ? PWM_LED_ON : 0, 1);
	set_pwm(LED_MUTE_1, (tsc_status & TACTILE_BUTTON_2) ? PWM_LED_ON : 0, 1);
	set_pwm(LED_MUTE_2, (tsc_status & TACTILE_BUTTON_3) ? PWM_LED_ON : 0, 1);
	set_pwm(LED_MUTE_3, (tsc_status & TACTILE_BUTTON_4) ? PWM_LED_ON : 0, 1);
	set_pwm(LED_MUTE_4, (tsc_status & TACTILE_BUTTON_5) ? PWM_LED_ON : 0, 1);

	/*
	 * Update the volumes
	 */
	output_volumes[0] = slider_positions[0];
	output_volumes[1] = slider_positions[1];
	output_volumes[2] = slider_positions[2];
	output_volumes[3] = slider_positions[3];
	output_volumes[4] = slider_positions[4];

	/*
	 * Update the output status
	 */
	output_status[0] = (tsc_status & TACTILE_BUTTON_1);
	output_status[1] = (tsc_status & TACTILE_BUTTON_2);
	output_status[2] = (tsc_status & TACTILE_BUTTON_3);
	output_status[3] = (tsc_status & TACTILE_BUTTON_4);
	output_status[4] = (tsc_status & TACTILE_BUTTON_5);

	/*
	 * Check if we need to refresh the screens
	 */
	uint32_t refresh = fsm_refresh_needed();

	/*
	 * Refresh the screen, if needed.
	 *
	 * This function call may be long, depending on the refresh types
	 * and the number of screen to be refreshed.
	 */
	if (refresh != 0)
	{
		__fsm_refresh_screen(refresh);
	}

	/*
	 * Exit the main fsm_update function
	 */
	return;
}

/*
 * -----------------------------------------------------------------
 * Private functions
 * -----------------------------------------------------------------
 */
static void fsm_handle_command()
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
	int size = 0;
	size = builder(&command, (char*)TxBuffer);
	CDC_Transmit_Long(TxBuffer, size);
	state = WAIT_FOR_COMMAND;

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
static void __fsm_handle_shutd()
{
	/*
	 * Send a message to log the received command to the serial port.
	 */
#ifdef LOG_SHUTD
	int size = 0;
	size = snprintf((char *)isr_message, (size_t)sizeof(isr_message), "[  FSM   ] SHUTD, responded with result : %4d\n\rDevice won't answer from now...\n\r", command.result);
	uart_log(isr_message, size);
	memset((void *)isr_message, 0x00, (size_t)sizeof(isr_message));

	// Add delay to ensure the message will be sent
	HAL_Delay(100);
#endif

	/*
	 * Setting the maximal amount of peripherals to the off state
	 */

	// Set leds off
	set_pwm(LED_MUTE_0, 0, 1);
	set_pwm(LED_MUTE_1, 0, 1);
	set_pwm(LED_MUTE_2, 0, 1);
	set_pwm(LED_MUTE_3, 0, 1);
	set_pwm(LED_MUTE_4, 0, 1);

	set_pwm(RGB_RED,    0, 1); // RED
	set_pwm(RGB_GREEN,  0, 1); // GREEN
	set_pwm(RGB_BLUE,   0, 1); // BLUE
	set_pwm(LED_DEBUG,  0, 1); // Debug

	// Clearing the screens :
	SSD1306_Fill(false);
	for (int k = 0; k < 5; k++)
	{
		// Select the correct screen
		tca9548_select(k);

		// Update the screen (we don't need to redraw the buffer since it's all black now)
		SSD1306_UpdateScreen();
	}

	/*
	 * For this command, we'll perform action after logging :)
	 */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWR_EnterSTANDBYMode();

	return;
}

/*
 * Perform a soft reset of the device, to restart anything.
 * Since the code of USBDEVICE has been slightly modified, this will
 * also trigger an USB re-enumeration process on the host.
 */
static void __fsm_handle_reset()
{
	/*
	 * Send a message to log the received command to the serial port.
	 */
#ifdef LOG_RESET
	int size = 0;
	size = snprintf((char *)isr_message, (size_t)sizeof(isr_message), "[  FSM   ] RESET, responded with result : %4d\n\r", command.result);
	uart_log(isr_message, size);
	memset((void *)isr_message, 0x00, (size_t)sizeof(isr_message));

	// Add delay to ensure the message will be sent
	HAL_Delay(100);
#endif

	/*
	 * Performing the device reset !
	 */
	NVIC_SystemReset();

	return;
}

/*
 * Perform a sync between the shown apps on the device,
 * and the ones needed for the host.
 */
static void __fsm_handle_async()
{
	/*
	 * Compare the actual apps with the requested ones :
	 *
	 * To ensure memory coherency, we need to store the results in a temp variables, since
	 * the command struct define all the commands structs as unions -> When we edit one, we can't
	 * be assured that the remaining will be right.
	 */
	uint8_t sync = 1;

	uint32_t slider_match[5] = { 0 };
	uint32_t slider_save[5] = { 0 };

	for (int k = 0; k < 5; k++)
	{
		// Produce the comparison output
		if (active_apps[k] == command.ASYNC_TX.appSliders[k])
		{
			slider_match[k] = 1;
		}
		else
		{
			slider_match[k] = 0;
			sync = 0;
		}

		// Save the value to build the answer then
		slider_save[k] = command.ASYNC_TX.appSliders[k];
	}

	/*
	 * Then, build the response
	 */
	command.ASYNC_RX.sync = sync;

	for (int k = 0; k < 5; k++)
	{
		command.ASYNC_RX.sliders[k].appMatch = slider_match[k];
		command.ASYNC_RX.sliders[k].appSlider = slider_save[k];
	}

	/*
	 * Set the response bit
	 */
	command.result = OK;

	/*
	 * Send a message to log the received command to the serial port.
	 */
#ifdef LOG_ASYNC
	int size = 0;
	size = snprintf((char *)isr_message, (size_t)sizeof(isr_message), "[  FSM   ] ASYNC, responded with result : %4d\n\r", command.result);
	uart_log(isr_message, size);
	memset((void *)isr_message, 0x00, (size_t)sizeof(isr_message));
#endif

	return;
}

/*
 * Perform the changement of the icon shown on the OLED screen.
 */
static void __fsm_handle_uicon()
{

	/*
	 * Send a message to log the received command to the serial port.
	 */
#ifdef LOG_UICON
	int size = 0;
	size = snprintf((char *)isr_message, (size_t)sizeof(isr_message), "[  FSM   ] UICON, responded with result : %4d\n\r", command.result);
	uart_log(isr_message, size);
	memset((void *)isr_message, 0x00, (size_t)sizeof(isr_message));
#endif

	return;
}

/*
 * Return the position of the different sliders.
 */
static void __fsm_handle_slpos()
{
	/*
	 * First, fill the command buffer :
	 */
	for (int k = 0; k < 5; k++)
	{
		command.SLPOS_RX.sliders[k].Mute = output_status[k];
		command.SLPOS_RX.sliders[k].Pos = output_volumes[k];
	}

	/*
	 * Acknowledge the output
	 */
	command.result = OK;

	/*
	 * Send a message to log the received command to the serial port.
	 */
#ifdef LOG_SLPOS
	int size = 0;
	size = snprintf((char *)isr_message, (size_t)sizeof(isr_message), "[  FSM   ] SLPOS, responded with result : %4d\n\r", command.result);
	uart_log(isr_message, size);
	memset((void *)isr_message, 0x00, (size_t)sizeof(isr_message));
#endif

	return;
}

/*
 * Configure the device.
 */
static void __fsm_handle_dconf()
{
	if (device_connected == true)
	{
		/*
		 * Copy data from the command data into the header
		 */
		memcpy((void*)header.SN,(void*)command.DCONF_TX.SN, (size_t)sizeof(header.SN));
		header.gain = (uint8_t)((int)(command.DCONF_TX.adcGain) & 0xFF);
		header.offset = command.DCONF_TX.adcOffset;

		for (int k = 0; k < 5; k++)
		{
			header.chan_gain[k] = command.DCONF_TX.sliders[k].gain;
			header.chan_offsets[k] = command.DCONF_TX.sliders[k].offset;
		}

		/*
		 * Set the command output
		 */
		command.result = OK;

		/*
		 * Write into the EEPROM the new header
		 */
		uint8_t buf[128] = { 0 };
		build_raw_eeprom_header(buf);
		eeprom_write(HEADER_ADDR, buf, HEADER_LEN, HAL_MAX_DELAY);

		/*
		 * Add some delay to ensure the operation finish
		 */
		HAL_Delay(10);
	}
	else
	{
		command.result = NACK;
	}

	/*
	 * Send a message to log the received command to the serial port.
	 */
#ifdef LOG_DCONF
	int size = 0;
	size = snprintf((char *)isr_message, (size_t)sizeof(isr_message), "[  FSM   ] DCONF, responded with result : %4d\n\r", command.result);
	uart_log(isr_message, size);
	memset((void *)isr_message, 0x00, (size_t)sizeof(isr_message));
#endif
	/*
	 * In any case, we'll show the user the new values on the debug port
	 */
	show_loaded_header();

	return;
}

/*
 * Get the status of the device
 */
static void __fsm_handle_connc()
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
	}
	else
	{
		command.result = NACK;
	}

    /*
     * Send a message to log the received command to the serial port.
     */
#ifdef LOG_CONNC
	int size = 0;
    size = snprintf((char *)isr_message, (size_t)sizeof(isr_message), "[  FSM   ] CONNC, responded with result : %4d\n\r", command.result);
    uart_log(isr_message, size);
    memset((void *)isr_message, 0x00, (size_t)sizeof(isr_message));
#endif

	return;
}

/*
 * Utility functions
 */

static uint32_t fsm_refresh_needed(void)
{
	/*
	 * Store the previous volumes (Will indicate both the MUTE and VOLUME states)
	 */
	static int save_output_volumes[5] = { 0 };
	static int save_output_status[5] = { 0 };

	int refresh_vol = 0;
	int refresh_sta = 0;

	/*
	 * Compute the refresh type needed
	 */
	for (int k = 0; k < 5; k++)
	{
		refresh_vol = (save_output_volumes[k] == output_volumes[k]) ? refresh_vol << 1 : (refresh_vol + 1) << 1;
		refresh_sta = (save_output_status[k] == output_status[k]) ? (refresh_sta << 1) : (refresh_sta + 1) << 1;
	}

	/*
	 * Update the static variables
	 */
	for (int k = 0; k < 5; k ++)
	{
		save_output_volumes[k] = output_volumes[k];
		save_output_status[k] = output_status[k];
	}

	/*
	 * Use logic to return a 32 bit word that contain the data of which screen shall be refresh
	 */
	return (uint32_t)((refresh_vol << 8) | (refresh_sta << 16));
}

static void __fsm_refresh_screen(uint32_t refresh)
{
	/*
	 * First, split the input value to get the elements that need to be refresh
	 */
	uint8_t vols = (refresh >> 8) & 0xFF;
	uint8_t stat = (refresh >> 16) & 0xFF;

	/*
	 * Then, apply the refresh as needed
	 */
	uint8_t mask = 1 << 5;
	uint8_t vol_needed = 0;
	uint8_t sta_needed = 0;

	for (int k = 0; k < 5; k++)
	{
		vol_needed = vols & mask;
		sta_needed = stat & mask;

		// Update the mask
		mask = mask >> 1;

		/*
		 * Now, update the screens, if required (We do this to prevent for overutilizing the I2C bus)
		 */
		if ((vol_needed != 0) | (sta_needed != 0))
		{
			memset((void*)&screen_cmd, 0x00, (size_t)sizeof(screen_cmd));

			screen_cmd.type = VOLSTAT;
			screen_cmd.volume = output_volumes[k];
			screen_cmd.status = (output_status[k] == 0) ? MUTE : UNMUTE;

			// Fill the buffer
			br = draw_buffer(&screen_cmd);

			// Select the correct screen
			tca9548_select(k);

			// Update the screen
			SSD1306_SendBuffer(br->buffer);
			SSD1306_UpdateScreen();
		}
	}

	return;
}
