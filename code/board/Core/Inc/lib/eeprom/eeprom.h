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
 * DEFINES
 * -----------------------------------------------------------------
 */
#define HEADER_ADDR 	(0x00000000)
#define HEADER_LEN		128


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
	uint8_t gain;
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
 * FUNCTIONS TO PARSE AND BUILD THE EEPROM HEADER
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

/* -----------------------------------------------------------------
 * FUNCTIONS TO FETCH EEPROMS ADDRESSES
 * -----------------------------------------------------------------
 */

/**
 * @brief 	Get the address of an app within the eeprom from it's
 * 			reduced hash (got from crc).
 *
 * @details Mostly used internally to retrieve addresses of data
 * 			within the already known apps hash.
 *
 * @warning Any return value under 128 shall be considered as invalid.
 *
 * @param 	hash	hash value on 16 bits.
 * @return	int 	the address value.
 */
uint16_t get_eeprom_address_from_hash(uint16_t hash);

/**
 * @brief 	Get the address of an app within the eeprom from it's
 * 			crc (raw transmitted from app name).
 *
 * @details Mostly used with an unknown app crc is fetched.
 *
 * @warning Any return value under 128 shall be considered as invalid.
 *
 * @param 	hash	hash value on 32 bits.
 * @return	int 	the address value.
 */
uint16_t get_eeprom_address_from_app(uint32_t hash);

/**
 * @brief 	Get the address of an empty EEPROM slot.
 *
 * @details Used to allocate a new memory slot.
 *
 * @warning Any return value under 128 shall be considered as invalid.
 *
 * @param 	app 	the crc of the app where the data need to be allocated.
 * @return	int 	the address value.
 */
uint16_t get_new_eeprom_address_from_app(uint32_t app);

/**
 * @brief 	Get the address of an empty EEPROM slot.
 *
 * @details Used to allocate a new memory slot.
 *
 * @warning Any return value under 128 shall be considered as invalid.
 *
 * @param 	app 	the raw hash.
 * @return	int 	the address value.
 */
uint16_t get_new_eeprom_address_from_hash(uint16_t hash);

/**
 * @brief	Free the address of an existing EEPROM slot.
 *
 * @param 	hash	The app hash to be freed.
 * @return	int		0.
 */
int free_eeprom_address(uint16_t hash);

/**
 * @brief	Compute the hash of a crc to fit into 16 bit variables.
 *
 * @param 	input		32 bits crc
 * @return	uint16_t	the 16 bit "equivalent".
 */
uint16_t GetHashFromCRC(uint32_t input);

#ifdef __cplusplus
}
#endif
