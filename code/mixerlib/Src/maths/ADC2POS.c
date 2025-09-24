/** ================================================================
 * @file    maths/ADC2POS
 *
 * @brief   This file implement maths operations on the different buffers
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
#include "maths/maths.h"

// External libs
#include "eeprom/eeprom.h"

// STD
#include <math.h>

/* -----------------------------------------------------------------
 * EXTERNAL VARIABLES
 * -----------------------------------------------------------------
 */

extern struct EEPROM_Header header;

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

int ADC2POS(uint16_t value, int slider)
{
	float gain;
	float offset;

	// First, fetch channel parameters
	if ((5 > slider) & (slider > 0))
	{
		gain = header.chan_gain[slider - 1];
		offset = header.chan_offsets[slider - 1];
	}
	else
	{
		gain = 1.0000f;
		offset = 0.000f;
	}

	// Then, fetch the exact value
	float val;
	if(ADC2Double((int16_t)(value & 0x7FFF), &val) != 0)
	{
		return -1;
	}

	// Then, combine
	val += offset + header.offset;
	val *= gain;
	val *= header.gain;

	// Now, get the %
	return round((val * 100) / ADC_MAX_VOLTAGE);
}
