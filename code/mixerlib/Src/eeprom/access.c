/** ================================================================
 * @file    eeprom/access.c
 *
 * @brief   This file implement the functions to access to some
 * 			specific data types on the eeprom outputs.
 *
 * @date    29-06-2025
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

// STD
#include <stdint.h>

/* -----------------------------------------------------------------
 * VARIABLES
 * -----------------------------------------------------------------
 */

extern struct EEPROM_hash hashs;

/* -----------------------------------------------------------------
 * PRIVATE FUNCTIONS
 * -----------------------------------------------------------------
 */

static uint16_t __seek_for_hash(uint16_t val)
{
	for (int k = 0; k < 15; k ++)
	{
		if (hashs.Icons[k].hash == val)
		{
			return k;
		}
	}
	return -1;
}

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/*
 * Access functions
 */
uint16_t get_eeprom_address_from_app(uint32_t hash)
{
	return get_eeprom_address_from_hash(GetHashFromCRC(hash));
}

uint16_t get_new_eeprom_address_from_app(uint32_t app)
{
	return get_new_eeprom_address_from_hash(GetHashFromCRC(app));
}

/*
 * Computations functions
 */
uint16_t GetHashFromCRC(uint32_t input)
{
	uint16_t upper = (input & 0xFFFF0000) >> 16;
	uint16_t lower = (input & 0x0000FFFF);
	return upper ^ lower;
}

/*
 * Getters
 */
uint16_t get_eeprom_address_from_hash(uint16_t hash)
{
	return hashs.Icons[__seek_for_hash(hash)].address;
}

/*
 * Allocation and freeing
 */
uint16_t get_new_eeprom_address_from_hash(uint16_t hash)
{
	int tmp = __seek_for_hash(0);
	hashs.Icons[tmp].hash = hash;
	return hashs.Icons[tmp].address;
}

int free_eeprom_address(uint16_t hash)
{
	hashs.Icons[__seek_for_hash(hash)].hash = 0x00000000;
	return 0;
}
