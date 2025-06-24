/** ================================================================
 * @file    mixer/parser/commands/dconf.c
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
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

/* -----------------------------------------------------------------
 * DEFINES
 * -----------------------------------------------------------------
 */

#define DCONF_PARSER_BUFFER 32

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/**
 * Full buffer :
 *{
 *  "cal" : [
 *      {
 *          "1OFF"  : "+0.082",
 *          "1G"    : "1.01"
 *      },
 *              {
 *          "2OFF"  : "+0.029",
 *          "2G"    : "1.09"
 *      },
 *              {
 *          "3OFF"  : "+0.050",
 *          "3G"    : "1.00"
 *      },
 *              {
 *          "4OFF"  : "+0.000",
 *          "4G"    : "1.12"
 *      },
 *              {
 *          "5OFF"  : "-0.012",
 *          "5G"    : "0.98"
 *      }
 *  ],
 *  "gain" : "1.09",
 *  "offset": "0.012",
 *  "device": "2025118H1D9",
 *}
 *
 * Raw buffer
 *{"cal":[{"1OFF":"+0.082","1G":"1.01"},{"2OFF":"+0.029","2G":"1.09"},{"3OFF":"+0.050","3G":"1.00"},{"4OFF":"+0.000","4G":"1.12"},{"5OFF":"-0.012","5G":"0.98"}],"gain":"1.09","offset":"+0.012","device":"25MYH0D0"}
 */

int parse_dconf_payload(const char * buf, const int len, struct CMD_DCONF_TX * const cmd)
{
	// Input checks
	if ((buf == NULL) | (cmd == NULL))
	{
		return -1;
	}
	if (len < 212)
	{
		return -2; // Not enough char to ensure a correct command
	}

    // Create a working buffer
    uint8_t work[DCONF_PARSER_BUFFER] = {0};

	// Search for "{\"cal\":[" token.
    memcpy((void *)work, (void *)buf, (size_t)8);
    char *ref = "{\"cal\":[";
    if (strcmp((char *)work, (char *)ref) != 0)
    {
        return -30; // JSON Header not found
    }

    // Then, try to split the different substrings
    for (int k = 0; k < 5; k++)
    {
    	// Clear and copy the next sub element
    	memset((void *)work, 0x00, (size_t)DCONF_PARSER_BUFFER);
    	memcpy((void *)work, (void *)&buf[(30* k) + 8], (size_t)29);

    	// Perform data extract and replacement :
    	uint8_t channel[4] = {0};
    	uint8_t offset[8] = {0};
    	uint8_t gain[4] = {0};

    	channel[0] = work[2];
    	channel[2] = work[18];
    	memcpy((void *)offset, (void *)&work[9], (size_t)6);
    	memcpy((void *)gain, (void *)&work[23], (size_t)4);

    	work[2] = '%';
    	work[18] = '%';
    	char tmp[] = "%%%%%%";
    	memcpy((void *)&work[9], (void *)tmp, (size_t)6);
    	memcpy((void *)&work[23], (void *)tmp, (size_t)4);

    	// Compare the new work buffer to check the syntax
    	char ref2[] = "{\"%OFF\":\"%%%%%%\",\"%G\":\"%%%%\"}\0\0";
        if (strcmp((char *)work, (char *)ref2) != 0)
        {
            return -31; // JSON Channel data invalid.
        }

        // Cast the channel identifier
        if ((!isdigit(channel[0])) | (!isdigit(channel[2])))
		{
			return -32; // At least a channel is not a number
		}
		int chan = strtol((char *)&channel[0], NULL, 10);
		int chan2 = strtol((char *)&channel[2], NULL, 10);
		if (chan != chan2)
		{
			return -33; // Two channel identifiers aren't the same.
		}
		if ((0 > chan) | (chan > 5))
		{
			return -34; // Invalid channel identifier
		}

		// Cast the data
		char *cmp;
		double chan_gain = strtod((char *)gain, &cmp);
		if (cmp <= (char*)&gain[3])
		{
			return -35; // Error while casting gain to double
		}

		double chan_offset = strtod((char*)offset, &cmp);
		if (cmp <= (char *)&offset[5])
		{
			return -36; // Error while casting offset to double
		}

		// Assign the data out :
		switch (chan)
		{
		case 1:
			cmd->slider1.gain = chan_gain;
			cmd->slider1.offset = chan_offset;
			break;

		case 2:
			cmd->slider2.gain = chan_gain;
			cmd->slider2.offset = chan_offset;
			break;

		case 3:
			cmd->slider3.gain = chan_gain;
			cmd->slider3.offset = chan_offset;
			break;

		case 4:
			cmd->slider4.gain = chan_gain;
			cmd->slider4.offset = chan_offset;
			break;

		case 5:
			cmd->slider5.gain = chan_gain;
			cmd->slider5.offset = chan_offset;
			break;
		}
    }

	// Clear and copy the next sub element
	memset((void *)work, 0x00, (size_t)DCONF_PARSER_BUFFER);
	memcpy((void *)work, (void *)&buf[156], (size_t)11);
    char ref3[] = "}],\"gain\":\"";
    if (strcmp((char *)work, (char *)ref3) != 0)
	{
		return -37; // JSON Gain invalid
	}

    memset((void *)work, 0x00, (size_t)DCONF_PARSER_BUFFER);
    memcpy((void *)work, (void *)&buf[167], (size_t)4);
    char *cmp;
    cmd->adcGain = strtod((char *)work, &cmp);
    if (cmp <= (char *)&work[3])
	{
		return -38; // Error while casting gain to double
	}

	// Clear and copy the next sub element
	memset((void *)work, 0x00, (size_t)DCONF_PARSER_BUFFER);
	memcpy((void *)work, (void *)&buf[171], (size_t)12);
	char ref4[] = "\",\"offset\":\"";
	if (strcmp((char *)work, (char *)ref4) != 0)
	{
		return -39; // JSON Offset invalid
	}

	memset((void *)work, 0x00, (size_t)DCONF_PARSER_BUFFER);
	memcpy((void *)work, (void *)&buf[183], (size_t)6);
	cmd->adcOffset = strtod((char *)work, &cmp);
	if (cmp <= (char *)&work[5])
	{
		return -40; // Error while casting offset to double
	}


	// Clear and copy the next sub element
	memset((void *)work, 0x00, (size_t)DCONF_PARSER_BUFFER);
	memcpy((void *)work, (void *)&buf[189], (size_t)12);
	char ref5[] = "\",\"device\":\"";
	if (strcmp((char *)work, (char *)ref5) != 0)
	{
		return -41; // JSON Device invalid
	}

	// Copy the ID
	memcpy((void *)cmd->SN, (void *)&buf[201], (size_t)11);

	// Final checks
	if ((buf[209] != '"') | (buf[210] != '}'))
	{
		return -42; // Invalid END JSON
	}

	return 0;
}
