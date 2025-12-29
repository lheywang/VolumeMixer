/** ================================================================
 * @file    handlers/adc.h
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
 * Defines
 * -----------------------------------------------------------------
 */

// This save some memory by grouping all TSC channels into a single group.
#define TACTILE_BUTTON_1 (1 << 0)
#define TACTILE_BUTTON_2 (1 << 1)
#define TACTILE_BUTTON_3 (1 << 2)
#define TACTILE_BUTTON_4 (1 << 3)
#define TACTILE_BUTTON_5 (1 << 4)

// TSC deboucer parameters
#define TSC_TOUCH_SAMPLES 8
#define TSC_DEBOUNCE_COUNT 3
#define TSC_RELEASE_THRESHOLD 0.97f   // 97% of baseline

#define TSC_MOVING_AVERAGE_FACTOR 0.02f  // Speed to follow changes

#define TSC_NUM_CHANNELS 5

/*
 * -----------------------------------------------------------------
 * Typedefs
 * -----------------------------------------------------------------
 */
typedef struct {
    uint16_t baseline;
    uint16_t filtered;
    uint8_t pressed;
    uint8_t debounce;
} TSC_ChannelState;

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
extern int tsc_status;

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
/**
 * @brief Configure a TSC channel, and start the acquistion.
 */
int tsc_configure_next_channel(void);

/**
 * @brief Read the value of a TSC channel.
 * 		  Automatically perform the update of the TSC button register.
 */
uint8_t tsc_read_channel(void);

/**
 * @brief Interrupt that call the previous functions, when needed.
 */
void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef *htsc);
void EXTI2_TSC_IRQHandler(void);


