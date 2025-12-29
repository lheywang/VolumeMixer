/** ================================================================
 * @file    utils/CDC_helper.h
 *
 * @brief   Header for some utilities that are nice to have for the
 * 			CDC VCOM module
 *
 * @date 	24-09-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */
#pragma once
/*
 * -----------------------------------------------------------------
 * Includes
 * -----------------------------------------------------------------
 */
// STD
#include <stdint.h>

// HAL
#include "stm32f3xx_hal.h"

/*
 * -----------------------------------------------------------------
 * Defines
 * -----------------------------------------------------------------
 */
#define USB_TX_BUF_SIZE 384

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
/**
 * Send the passed buffer, while splitting it.
 * Use TIM16 interrupt to check if we can send another 64 bytes chunk.
 * The timer is stopped once the buffer has been sent entirely.
 */
void CDC_Transmit_Long(uint8_t *buf, uint16_t len);

/**
 * Interrupt called by the timer to poll until all data has been sent
 */
void USB_IRQHandler(void);

