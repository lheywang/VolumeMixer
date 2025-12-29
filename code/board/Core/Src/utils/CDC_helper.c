/** ================================================================
 * @file    utils/CDC_helper.c
 *
 * @brief   Source file of the helper utils for the CDC_helper file
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
#include "utils/CDC_helper.h"

// STD
#include <stdint.h>

// HAL
#include "usbd_cdc_if.h"
#include "usb_device.h"
#include "usbd_cdc.h"
#include "stm32f3xx_hal.h"

/*
 * -----------------------------------------------------------------
 * Defines
 * -----------------------------------------------------------------
 */
#define MAX_TIMEOUT 1000

#define CDC_DATA_IN_EP 0x81  // Typical CDC IN endpoint address

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
uint8_t usb_tx_buf[USB_TX_BUF_SIZE];
volatile uint16_t tx_head = 0;
volatile uint16_t tx_tail = 0;
volatile uint8_t usb_tx_busy = 0;
volatile uint16_t last_sent_len = 0;

// Imported
// From main.c
extern TIM_HandleTypeDef htim16;

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
void CDC_Transmit_Long(uint8_t *buf, uint16_t len)
{
    for(uint16_t i = 0; i < len; i++)
	{
		usb_tx_buf[tx_head] = buf[i];
		tx_head = (tx_head + 1) % USB_TX_BUF_SIZE;

		if(tx_head == tx_tail)
		{
			break;
		}
	}
    /*
     * Start the transmission timer, which will poll the USB peripheral until all data has been sent
     */
    HAL_TIM_Base_Start_IT(&htim16);
    return;
}

void USB_IRQHandler(void)
{
    if(tx_tail == tx_head)
    {
        // Buffer empty -> stop timer to save CPU
    	HAL_TIM_Base_Stop(&htim16);
    	tx_head = 0;
    	tx_tail = 0;
        return;
    }

    uint16_t chunk_len = (tx_head >= tx_tail) ? (tx_head - tx_tail) : (USB_TX_BUF_SIZE - tx_tail);
    if(chunk_len > 64) chunk_len = 64;

    if(CDC_Transmit_FS(&usb_tx_buf[tx_tail], chunk_len) == USBD_OK)
    {
        tx_tail = (tx_tail + chunk_len) % USB_TX_BUF_SIZE;
    }
}

