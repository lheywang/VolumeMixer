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
#define ADC_RESOLUTION_BITS 12
#define ADC_MAX_VOLTAGE 	3.3

/* -----------------------------------------------------------------
 * MACROS
 * -----------------------------------------------------------------
 */
#define GET_INTERNAL_HASH(x) ((x & 0x0000FFFF) ^ ((x & 0xFFFF0000) >> 16))

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/*
 * Functions for the ADC conversion
 */

/**
 * @brief 	Convert a 12 bit value into it's floating point representation.
 *
 * @param 	code 	The raw 12 bit code
 * @param 	*val 	Pointer to the output buffer.
 *
 * @return 	int
 * @retval    0 	Operation successfull.
 * @retval 	- 1 	Out of range code (MAX = 4096).
 */
int ADC2Double(int16_t code, float *val);

/**
 * @brief 	Convert a double into it's raw ADC code representation.
 *
 * @param 	val		Value to be formatted
 * @param 	*code 	Pointer to the output buffer.
 *
 * @retval    0 	Operation successfull.
 * @retval 	- 1 	Out of range code (MIN = 0.0V, MAX = ADC_MAX_VOLTAGE);
 */
int Double2ADC(float val, int16_t *code);

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
int S162Double(int8_t code, float *val);

/**
 * @brief 	Convert a double into it's raw ADC code representation.
 *
 * @param 	val		Value to be formatted
 * @param 	*code 	Pointer to the output buffer.
 *
 * @return 	int
 * @retval    0 	Operation successfull.
 * @retval 	- 1 	Out of range code (MIN = -1.5, MAX = 1.5);
 */
int Double2S16(float val, int8_t *code);

/*
 * Functions for to get the real position of the slider in %
 */

/**
 * @brief	Convert, and correct the value of the slider into it's percentage representation.
 *
 * @param 	value	raw 12 bits value from the ADC
 * @param 	slider	position of the slider (1-5). If 0, it's correction is bypassed.
 *
 * @return	int		The position percentage
 */
int ADC2POS(uint16_t value, int slider);

#ifdef __cplusplus
}
#endif
