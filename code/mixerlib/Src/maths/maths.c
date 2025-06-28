/** ================================================================
 * @file    maths/maths.c
 *
 * @brief   This file implement maths operations on the different buffers
 *
 * @date    26-06-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */

/* -----------------------------------------------------------------
 * INCLUDING LIBS
 * -----------------------------------------------------------------
 */
// Header
#include "maths/maths.h"

// STD
#include <math.h>

/* -----------------------------------------------------------------
 * DEFINES
 * -----------------------------------------------------------------
 */
#define ADC_RESOLUTION (float)(ADC_MAX_VOLTAGE/ ((1 << ADC_RESOLUTION_BITS) - 1))

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/**
 * ADC Conversions
 */
inline int ADC2Double(uint16_t code, float *val)
{
	if (code > (1 << ADC_RESOLUTION_BITS))
	{
		return -1;
	}

	*val = (float)code * ADC_RESOLUTION;
	return 0;
}

inline int Double2ADC(float val, uint16_t *code)
{
	if ((0 > val) | (val > ADC_MAX_VOLTAGE))
	{
		return -1;
	}

	*code = ceil(val / ADC_RESOLUTION);
	return 0;
}

/**
 * S1.6 conversion
 */
inline int S162Double(int8_t code, float *val)
{
	*val = code / 64.0;
	return 0;
}

inline int Double2S16(float val, int8_t *code)
{
	if ((-1.5 > val) | (val > 1.5))
	{
		return -1;
	}
	*code = (int8_t)round(val * 64);
	return 0;
}




