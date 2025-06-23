/** ================================================================
 * @file    mixer/parser/commands/uicon.c
 *
 * @brief   This file implement the payload parsing and building
 * 			for the ASYNC command.
 *
 * @date    23-06-2025
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
#include "parser/commands.h"

// STD
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

/* -----------------------------------------------------------------
 * DEFINES
 * -----------------------------------------------------------------
 */
#define UICON_PARSER_BUFFER 256

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/**
 * Example buffer (with formatting)
 * {
 *  "icon" : {
 *      "slider"    : "1",
 *      "app"       : "deadbeef", // This is the CRC32 of the APP name, to ensure it'll fit on the same length
 *      "icon"      : "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
 *      "store"     : true
 *  	}
 *  }
 *
 * Raw buffer :
 * {"icon":{"slider":"1","app":"deadbeef","icon":"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx","store":1}}
 */

int parse_uicon_payload(const char * buf, struct CMD_UICON_TX * const cmd)
{
	// Input checks
	if ((buf == NULL) | (cmd == NULL))
	{
		return -1;
	}

    // Create a working buffer
    uint8_t work[UICON_PARSER_BUFFER] = {0};

	// Search for "{"icon":{"slider":"" token.
    memcpy((void *)work, (void *)buf, (size_t)19);
    char *ref = "{\"icon\":{\"slider\":\"";
    if (strcmp((char *)work, (char *)ref) != 0)
    {
        return -20; // JSON Header not found
    }

	// Fetch the following target slider
	memset((void*)work, 0x00, UICON_PARSER_BUFFER);
	memcpy((void*)work, (void*)&buf[19], (size_t)1);

    // Attempt a conversion
	if (!isdigit(work[0]))
	{
		return -21;
	}
    cmd->posSlider = strtol((char *)work, NULL, 10); // We don't care about the base here, since < 10.
    if ((1 > cmd->posSlider) || (cmd->posSlider > 5))
    {
    	return -22; // Invalid value for slider position
    }

	// Fetch the following target app hash
	memset((void *)work, 0x00, UICON_PARSER_BUFFER);
	memcpy((void *)work, (void *)&buf[20], (size_t)9);
	char *ref2 = "\",\"app\":\"";
	if (strcmp((char *)work, (char *)ref2) != 0)
	{
		return -23; // App Name not found
	}

	// Fetch the following target slider
	memset((void *)work, 0x00, UICON_PARSER_BUFFER);
	memcpy((void *)work, (void *)&buf[29], (size_t)8);

	// Attempt a conversion and check for the errno
	for (int k = 0; k < 8; k++)
	{
	    // Attempt a conversion
		if (!isxdigit(work[k]))
		{
			return -24;
		}
	}
	cmd->appSlider = strtol((char *)work, NULL, 16);

	// Fetch the following target app hash
	memset((void *)work, 0x00, UICON_PARSER_BUFFER);
	memcpy((void *)work, (void *)&buf[37], (size_t)10);
	char *ref3 = "\",\"icon\":\"";
	if (strcmp((char *)work, (char *)ref3) != 0)
	{
		return -25; // Icon not found
	}

	// Fetch the following icon data
	memset((void *)work, 0x00, UICON_PARSER_BUFFER);
	memcpy((void *)work, (void *)&buf[47], (size_t)256);

	memset((void *)cmd->icon, 0x00, 128);

	// Convert the hex chain into the raw byte stream
	char temp_char[2] = {0};
	for (int k = 0; k < 128; k++)
	{
		// Higher nibble
		temp_char[0] = work[k * 2];
		if (!isxdigit(temp_char[0]))
		{
			return -26;
		}
		int c_tmp = (strtol(temp_char, NULL, 16) & 0x0F) << 4;

		// Lower nibble
		temp_char[0] = work[(k * 2) + 1];
		if (!isxdigit(temp_char[0]))
		{
			return -26;
		}
		c_tmp |= (strtol(temp_char, NULL, 16)  & 0x0F);

		// Store the value
		cmd->icon[k] = c_tmp & 0xFF;
	}

	// Fetch the following target store argument
	memset((void *)work, 0x00, UICON_PARSER_BUFFER);
	memcpy((void *)work, (void *)&buf[303], (size_t)10);
	char *ref4 = "\",\"store\":";
	if (strcmp((char *)work, (char *)ref4) != 0)
	{
		return -27; // Store not found
	}

	// Fetch the following target store argument
	memset((void *)work, 0x00, UICON_PARSER_BUFFER);
	memcpy((void *)work, (void *)&buf[313], (size_t)1);

    // Attempt a conversion and check for the errno
	if (!isdigit(work[0]))
	{
		return -28;
	}
    int tmp = strtol((char *)work, NULL, 16); // We don't care about the base here, since < 10.

    // Compare
    switch (tmp){
    case 1:
    	cmd->appStore = 1;
    	break;

    case 0:
    default: // Interpret any non 1 value as 1.
    	cmd->appStore = 0;
    	break;
    }

	// Parsing sucessfull
	return 0;
}
