/** ================================================================
 * @file    eeprom/eeprim.h
 *
 * @brief   This file implement the functions that handle fetching icons
 * 			on the eeprom, depending on their addresses stored into an
 * 			hash table.
 *
 * @date    26-06-2025
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
 * INCLUDING LIBS
 * -----------------------------------------------------------------
 */
#include <stdint.h>

/* -----------------------------------------------------------------
 * STRUCTS
 * -----------------------------------------------------------------
 */

/**
 * This first struct store the header once the buffer has been rode.
 * This store all of the parameters to be applied to the device.
 */
struct EEPROM_Header {
	float offset;
	float chan_offsets[5];
	float gain;
	float chan_gain[5];
	uint16_t default_apps[5];
	uint16_t preambule; // Constant to 0xbeef
	uint32_t crc;
	uint8_t SN[8];
};

/**
 * This second struct store the addresses for different hashes of data.
 */
struct EEPROM_hash {
	struct {
		uint16_t hash;
		uint16_t address;
	} Icons[15];
	uint32_t crc;
};

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */
/**
 * @brief 	Parse the raw input buffer from the EEPROM, and fill the
 * 			global configuration structures with the fetched data.
 *
 * @param  	buf 	128 bytes pointer of the pages (0 to 3 of the EEPROM).
 *
 * @return 	int
 * @retval    0 : 	Parsing done sucessfully.
 * @retval 	- 1 : 	Invalid pointer to buf passed.
 * @retval  - 2 : 	constant value not found.
 * @retval 	- 3 : 	Found invalid CRC.
 * @retval 	- 4 : 	Failed to convert offset(s)
 * @retval 	- 5 : 	Failed to convert gain(s)
 */
int parse_raw_eeprom_header(uint8_t buf[128]);

/**
 * @brief 	Build the buffer to be wrote to the pages 0-3 of the EEPROM,
 * 			to be used as a buffer.
 *
 * @note 	Typically, this function is called after a DCONF command has occured.
 *
 * @warning Even if the function return the whole 128 bytes, in most EEPROMS
 * 			you can't write more than the page size. Here, 32 bytes. So, you
 * 			may need to split the writes in multiples parts.
 *
 * @param  	buf 	128 bytes pointer of the pages (0 to 3 of the EEPROM).
 *
 * @return 	int
 * @retval    0 : 	Parsing done sucessfully.
 * @retval 	- 1 : 	Invalid pointer to buf passed.
 */
int build_raw_eeprom_header(uint8_t buf[128]);

#ifdef __cplusplus
}
#endif
