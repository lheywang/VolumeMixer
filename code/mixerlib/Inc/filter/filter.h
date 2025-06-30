/** ================================================================
 * @file    maths/maths.h
 *
 * @brief   This file describe the base principle of the volume
 * 			mixer application code. This tool is based on an HAL
 * 			abstraction using small functions to make changes from
 * 			one platform to the other easier.
 *
 * @date 	30-05-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------
 * LIBS
 * -----------------------------------------------------------------
 */
#include <stdint.h>

/* -----------------------------------------------------------------
 * DEFINES
 * -----------------------------------------------------------------
 */
/*
 * Got the value with a try and adapt method. Provide response in about 70 milliseconds,
 * which shall be fine for most, if not all usages.
 */
#define FILTER_ALPHA 0.045

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/**
 * @brief	Filter the raw ADC value to ensure smooth variations over the time.
 *
 * @param 	input	The raw ADC input value
 * @param 	channel The concerned channel.
 *
 * @return	uint16_t	The filtered value
 */
uint16_t filter(uint16_t input, int channel);

#ifdef __cplusplus
}
#endif
