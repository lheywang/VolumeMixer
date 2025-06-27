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
#include "eeprom/eeprom.h"

// locals libs
#include "maths/maths.h"
#include "CRC/checksum.h"

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
	if (ADC2Double(buf[22], &header.gain) != 0)
	{
		return -5;
	}

	for (int k = 0; k < 5; k++)
	{
		if (ADC2Double(buf[k + 23], &header.chan_offsets[k]) != 0)
		{
			return -5;
		}
	}

	// Now, get the hashes for the apps
	/*
	 * We can here perform a raw copy, which will indeed cast the data to their right storage
	 * types.
	 */
	memcpy((void *)&header.default_apps, (void *)&buf[32], (size_t)10);

	// Now, get app addresses and hashes
	/*
	 * Same here, we can also perform a raw copy
	 */
	memcpy((void *)&hashs.Icons, (void *)&buf[64], (size_t)60); // 15 images can be stored into the EEPROM

	return 0;
}

int build_raw_eeprom_header(uint8_t buf[128])
{
	// Inputs checks
	if (buf == NULL)
	{
		return -1;
	}

	// Updated marker
	buf[0] = 0xbe;
	buf[1] = 0xef;

	// Add the SN
	memcpy((void *)&buf[2], header.SN, (size_t)8);

	// Add ADC offsets
	if (Double2ADC(header.offset, (uint16_t *)&buf[10]) != 0)
	{
		return -2;
	}

	for (int k = 0; k < 5; k++)
	{
		if (Double2ADC(header.chan_offsets[k], (uint16_t *)&buf[(2 * k) + 12]) != 0)
		{
			return -4;
		}
	}

	// ADC gain correction factord
	if (Double2ADC(header.gain, (uint16_t *)&buf[22]) != 0)
	{
		return -5;
	}

	for (int k = 0; k < 5; k++)
	{
		if (Double2ADC(header.chan_offsets[k], (uint16_t *)&buf[k + 23]) != 0)
		{
			return -5;
		}
	}

	// Now, get the hashes for the apps
	/*
	 * We can here perform a raw copy, which will indeed cast the data to their right storage
	 * types.
	 */
	memcpy((void *)&buf[32], (void *)&header.default_apps, (size_t)10);

	// Now, get app addresses and hashes
	/*
	 * Same here, we can also perform a raw copy
	 */
	memcpy((void *)&buf[64], (void *)&hashs.Icons, (size_t)60); // 15 images can be stored into the EEPROM

	return 0;
}
