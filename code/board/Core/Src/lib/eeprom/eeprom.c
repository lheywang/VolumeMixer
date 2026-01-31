/** ================================================================
 * @file    eeprom/eeprom.c
 *
 * @brief   This file implement the functions to handle the hash and
 * 			fetching addresses
 *
 * @date    27-06-2025
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
// header
#include "lib/eeprom/eeprom.h"

// locals libs
#include "lib/maths/maths.h"
#include "lib/CRC/checksum.h"

// STD
#include <stdint.h>
#include <string.h>

/* -----------------------------------------------------------------
 * GLOBALS VARIABLES
 * -----------------------------------------------------------------
 */
/**
 * Access them with
 * - extern struct EEPROM_Header header;
 * - extern struct EEPROM_hash hashs;
 * respectively.
 */
struct EEPROM_Header 	header = { 0 };
struct EEPROM_hash 		hashs  = { 0 };

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

int parse_raw_eeprom_header(uint8_t buf[128])
{
	// Inputs checks
	if (buf == NULL)
	{
		return -1;
	}

	// Check for the common header
	if ((buf[0] != 0xbe) | (buf[1] != 0xef))
	{
		return -2;
	}

	// First, perform an CRC verification for both of the data elements
	uint32_t readcrc[2] = {
			(uint32_t)((buf[60] << 24) | (buf[61] << 16) | (buf[62] << 8) | buf[63]),
			(uint32_t)((buf[124] << 24) | (buf[125] << 16) | (buf[126] << 8) | buf[127])
	};
	uint32_t crc[2] =  {
			crc_32(&buf[0], (size_t)60),
			crc_32(&buf[64], (size_t)60),
	};

	if ((readcrc[0] != crc[0]) | (readcrc[1] != crc[1]))
	{
		return -3;
	}

	// Copy the crc
	header.crc = crc[0];
	hashs.crc = crc[1];

	// The preambule was checked, a static init is easier to perform.
	header.preambule = 0xbeef;

	// Add the SN
	memcpy((void *)&header.SN, (void *)&buf[2], (size_t)8);

	// ADC offsets corrections factors.
	uint16_t tmp = (buf[10] << 8) | buf[11];
	if (ADC2Double(tmp, &header.offset) != 0)
	{
		return -4;
	}

	for (int k = 0; k < 5; k++)
	{
		tmp = (buf[(2 * k) + 12] << 8) | buf[(2 * k) + 12 + 1];
		if (ADC2Double(tmp, &header.chan_offsets[k]) != 0)
		{
			return -4;
		}
	}

	// ADC gain correction factord
	header.gain = buf[22];

	for (int k = 0; k < 5; k++)
	{
		if (S162Double(buf[k + 23], &header.chan_gain[k]) != 0)
		{
			return -5;
		}
	}

	// Now, get the hashes for the apps
	for (int k = 0; k < 5; k ++)
	{
		header.default_apps[k] = (buf[(2 * k) + 32] << 8) | (buf[(2 * k) + 32 + 1]);
	}

	// Now, get app addresses and hashes
	for (int k = 0; k < 15; k++)
	{
		hashs.Icons[k].hash = (buf[(4 * k) + 64] << 8) | buf[(4 * k) + 64 + 1];
		hashs.Icons[k].address = (buf[(4 * k) + 64 + 2] << 8) | buf[(4 * k) + 64 + 3];
	}

	return 0;
}

int build_raw_eeprom_header(uint8_t buf[128])
{
	// Inputs checks
	if (buf == NULL)
	{
		return -1;
	}

	// First, fill the whole buffer with 0xFF (default value of bits on the eeprom, and also the one for the tests
	memset((void *)buf, 0xFF, (size_t)128);

	// Updated marker
	buf[0] = 0xbe;
	buf[1] = 0xef;

	// Add the SN
	memcpy((void *)&buf[2], header.SN, (size_t)8);

	// Temp variable
	uint16_t tmp;

	// Add ADC offsets
	if (Double2ADC(header.offset, &tmp) != 0)
	{
		return -2;
	}
	buf[10] = (tmp & 0xFF00) >> 8;
	buf[11] = (tmp & 0x00FF);

	for (int k = 0; k < 5; k++)
	{
		if (Double2ADC(header.chan_offsets[k], &tmp) != 0)
		{
			return -2;
		}

		buf[(2 * k) + 12] = (tmp & 0xFF00) >> 8;
		buf[(2 * k) + 12 + 1] = (tmp & 0x00FF);

	}

	// ADC gain correction factor
	buf[22] = header.gain;

	for (int k = 0; k < 5; k++)
	{
		if (Double2S16(header.chan_gain[k], (int8_t*)&buf[k + 23]) != 0)
		{
			return -3;
		}
	}

	// Now, get the hashes for the apps
	for (int k = 0; k < 5; k ++)
	{
		buf[(2 * k) + 32] = (header.default_apps[k] & 0xFF00) >> 8;
		buf[(2 * k) + 32 + 1] = header.default_apps[k] & 0x00FF;
	}

	// Now, get app addresses and hashes
	for (int k = 0; k < 15; k++)
	{
		buf[(4 * k) + 64] = (hashs.Icons[k].hash & 0xFF00) >> 8;
		buf[(4 * k) + 64 + 1] = hashs.Icons[k].hash & 0x00FF;
		buf[(4 * k) + 64 + 2] = (hashs.Icons[k].address & 0xFF00) >> 8;
		buf[(4 * k) + 64 + 3] = hashs.Icons[k].address & 0x00FF;
	}

	// Finally, add the CRCs
	// First, perform an CRC verification for both of the data elements
	uint32_t crc[2] =  {
			crc_32(&buf[0], (size_t)60),
			crc_32(&buf[64], (size_t)60),
	};

	// Add the CRC to the buffer
	buf[60] = (crc[0] & 0xFF000000) >> 24;
	buf[61] = (crc[0] & 0x00FF0000) >> 16;
	buf[62] = (crc[0] & 0x0000FF00) >> 8;
	buf[63] = crc[0] & 0x000000FF;

	buf[124] = (crc[1] & 0xFF000000) >> 24;
	buf[125] = (crc[1] & 0x00FF0000) >> 16;
	buf[126] = (crc[1] & 0x0000FF00) >> 8;
	buf[127] = crc[1] & 0x000000FF;

	return 0;
}
