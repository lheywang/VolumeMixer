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
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/**
 * Functions for the ADC conversion
 */

/**
 * @brief 	Convert a 12 bit value into it's floating point representation.
 *
 * @param 	code 	The raw 12 bit code
 * @param 	*val 	Pointer to the output buffer.
 *
 * @return 	int (0).
 */
int ADC2Double(uint16_t code, double *val);

/**
 * @brief 	Convert a double into it's raw ADC code representation.
 *
 * @param 	val		Value to be formatted
 * @param 	*code 	Pointer to the output buffer.
 *
 * @return 	int (0).
 */
int Double2ADC(double val, uint16_t *code);

/**
 * Functions for the GAIN storage (S1.6 format)
 */

/**
 * @brief 	Convert a 12 bit value into it's floating point representation.
 *
 * @param 	code 	The raw 12 bit code
 * @param 	*val 	Pointer to the output buffer.
 *
 * @return 	int (0).
 */
int S162Double(uint8_t code, double *val);

/**
 * @brief 	Convert a double into it's raw ADC code representation.
 *
 * @param 	val		Value to be formatted
 * @param 	*code 	Pointer to the output buffer.
 *
 * @return 	int (0).
 */
int Double2S16(double val, uint8_t *code);

#ifdef __cplusplus
}
#endif
