/** ================================================================
 * @file    handlers/adc.h
 *
 * @brief   Handle the ADC by defining functions about reading a channels, and so on.
 *
 * @date 	12/11/2025
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
#include "stm32f3xx_hal.h"

#include <stdint.h>

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
extern int adc_active_channel;

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
/**
 * @brief Configure an ADC channel, and launch an acquisition.
 */
int adc_configure_next_channel(void);

/**
 * @brief Fetch a result from the ADC. May wait if called too fast !
 */
uint16_t adc_read_channel(void);

