/** ================================================================
 * @file    mixer/parser/commands/async.c
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
#define ASYNC_B_PARSER_BUFFER 32
#define ASYNC_P_PARSER_BUFFER 16

#define ASYNC_B_ERROR_CODE(x) (-x - 60)
#define ASYNC_P_ERROR_CODE(x) (-x - 50)

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/**
 * {
 *  "apps" : [
 *      {"1"    : CRC32["master"]},
 *      {"2"    : CRC32["discord.exe"]},
 *      {"3"    : CRC32["firefox.exe"]},
 *      {"4"    : CRC32["UNUSED"]},
 *      {"5"    : CRC32["Valorant.exe"]}
 *  ]
 * }
 *
 *{"apps":[{"1":deadbeef},{"2":deadbeef},{"3":deadbeef},{"4":deadbeef},{"5":deadbeef}]}
 */
int parse_async_payload(const char * buf, const int len, struct CMD_ASYNC_TX * const cmd)
{
	// Input checks
	if ((buf == NULL) | (cmd == NULL))
	{
		return -1; // Invalid pointer
	}

	if (len < 85)
	{
		return -2; // Buffer too short
	}

	// Create a working buffer
	uint8_t work[ASYNC_P_PARSER_BUFFER] = {0};

	// Search for "{\"cal\":[" token.
	memcpy((void *)work, (void *)buf, (size_t)9);
	char *ref = "{\"apps\":[";
	if (strcmp((char *)work, (char *)ref) != 0)
	{
		return ASYNC_P_ERROR_CODE(0); // JSON Header not found
	}

	for (int k = 0; k < 5; k++)
	{
		// Clear and copy the next sub element
		memset((void *)work, 0x00, (size_t)ASYNC_P_PARSER_BUFFER);
		memcpy((void *)work, (void *)&buf[(15* k) + 9], (size_t)14);

		// Perform data extract and replacement :
		uint8_t channel[2] = {work[2], 0};
		uint8_t app[9] = {0};

		memcpy((void *)app, (void *)&work[5], (size_t)8);

		work[2] = '%';
		char tmp[] = "%%%%%%%%";
		memcpy((void *)&work[5], (void *)tmp, (size_t)8);

		// Compare the new work buffer to check the syntax
		char ref2[] = "{\"%\":%%%%%%%%}";
		if (strcmp((char *)work, (char *)ref2) != 0)
		{
			return ASYNC_P_ERROR_CODE(1); // JSON Channel data invalid.
		}

		// Cast the channel identifier
		if (!isdigit(channel[0]))
		{
			return ASYNC_P_ERROR_CODE(2); // At least a channel is not a number
		}


		int chan = strtol((char *)&channel[0], NULL, 10);
		if ((0 > chan) | (chan > 5))
		{
			return ASYNC_P_ERROR_CODE(3); // Invalid channel identifier
		}

		for (int i = 0 ; i < 8; i++)
		{
			// Cast the channel identifier
			if (!isxdigit(app[i]))
			{
				return ASYNC_P_ERROR_CODE(4); // At least a channel is not a number
			}
		}
		int app_hash = strtol((char *)app, NULL, 16);

		// Assign the data out
		switch (chan)
		{
		case 1:
			cmd->appSlider1 = app_hash;
			break;

		case 2:
			cmd->appSlider2 = app_hash;
			break;

		case 3:
			cmd->appSlider3 = app_hash;
			break;

		case 4:
			cmd->appSlider4 = app_hash;
			break;

		case 5:
			cmd->appSlider5 = app_hash;
			break;
		}
	}

	// Final checks
	if ((buf[83] != ']') | (buf[84] != '}'))
	{
		return ASYNC_P_ERROR_CODE(5); // Invalid END JSON
	}


	return 0;
}

int build_async_payload(const struct CMD_ASYNC_RX * const cmd, const char *buf, const int * len)
{
	return 0;
}

