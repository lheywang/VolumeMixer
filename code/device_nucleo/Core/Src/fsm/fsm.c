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
#include <screen/screen.h>
#include <screen/data/icons.h>

#include "utils/CDC_helper.h"

// HAL
#include "usbd_cdc_if.h"

// STD
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
uint8_t RxBuffer[CDC_BUFFER_SIZE] = { 0 }; 	// Buffer for reading messages
uint8_t TxBuffer[CDC_BUFFER_SIZE] = { 0 }; 	// Buffer for transmitting messages back
volatile uint16_t rx_len = 0;  				// Number of bytes currently in the buffer
volatile bool 	  msg_complete = false;

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
void fsm_init()
{
	return;
}

void fsm_update()
{
	// messages are received by the USB CDC Interrupt. We just need to check if a message has been received, and, it yes, handle it !
	if (msg_complete == true)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		CDC_Transmit_Long(RxBuffer, rx_len);

		memset(RxBuffer, 0x00, sizeof(RxBuffer));
		msg_complete = false;

	}
	return;
}

/*
  // here only to include the right symbols into the build system...
  struct CMD command;
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
  	/*

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
/*
    uint8_t TxBuffer[] = "Hello World from STM32 CDC VCOM !\n\r";
    uint8_t Len = sizeof(TxBuffer);

	  HAL_Delay(500);
	  int val = CDC_Transmit_FS(TxBuffer, Len) + 0x30;
*/


