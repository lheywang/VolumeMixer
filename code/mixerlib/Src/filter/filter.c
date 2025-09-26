/** ================================================================
 * @file    filter/filter.c
 *
 * @brief   This file implement a digital filter on the software
 * 			to even further smooth the noise of the ADC by itself.
 *
 * @date    30-06-2025
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
#include "filter/filter.h"

/* -----------------------------------------------------------------
 * DEFINES
 * -----------------------------------------------------------------
 */
#define __FILTER_ALPHA2 (1 - FILTER_ALPHA)
#define __CHANNEL_COUNT 16

/* -----------------------------------------------------------------
 * STATIC VARIABLES
 * -----------------------------------------------------------------
 */
static uint16_t tmp[__CHANNEL_COUNT] = {0};

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

uint16_t filter(uint16_t input, int channel)
{
	if ((0 > channel) | (channel > __CHANNEL_COUNT))
	{
		return -1;
	}

	tmp[channel - 1] = (FILTER_ALPHA * input) + (__FILTER_ALPHA2 * tmp[channel - 1]);
	return (tmp[channel - 1] > 4095) ? 4095 : tmp[channel - 1];
}
