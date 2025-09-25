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
#include <screen/data/icons.h>

#include "utils/CDC_helper.h"

// HAL
#include "usbd_cdc_if.h"

// STD
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Import extern variables.
extern struct EEPROM_Header header;
extern struct EEPROM_hash hashs;

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
				state = WAIT_FOR_COMMAND;
			}
			break;
		/*
		 * If we're waiting for command, accept only the commands that are not CONNC.
		 * Any CONNC command will end up being rejected.
		 */
		case WAIT_FOR_COMMAND:
			if (command.type == CONNC)
			{
				command.result = NACK;
			}
			else
			{
				state = WAIT_FOR_RESPONSE;
			}
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

}

/*
 * Perform a soft reset of the device, to restart anything.
 * Since the code un USBDEVICE has been slightly modified, this will
 * also trigger an USB reenumeration process on the host.
 */
void __fsm_handle_reset()
{

}

/*
 * Perform a sync between the shown apps on the device,
 * and the ones needed for the host.
 */
void __fsm_handle_async()
{

}

/*
 * Perform the changements of the icon shown on the OLED screen.
 */
void __fsm_handle_uicon()
{

}

/*
 * Return the position of the different sliders.
 */
void __fsm_handle_slpos()
{

}

/*
 * Configure the device.
 */
void __fsm_handle_dconf()
{

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


