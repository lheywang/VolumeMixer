/** ================================================================
 * @file    mixer/parser/builder.c
 *
 * @brief   This file implement the buffer filling to handle the
 * 			command creation process.
 *
 * @date    25-06-2025
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
#include "lib/parser/parser.h"

// locals libs
#include "lib/parser/commands.h"
#include "lib/CRC/checksum.h"

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
#define PARSER_BUFFER_SIZE 16
#define PARSER_ERROR_CODE(x) (-x - 10)

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

int builder(struct CMD * const command, char *buf)
{
	// Inputs checks
	if ((command == NULL) | (buf == NULL))
	{
		return -1;
	}

	// Check that the command NEED to be built.
	if (command->result == INPUT)
	{
		return -2; // Command need to be treated before responding.
	}

	// Check that the direction is also correct
	if (command->direction == TX)
	{
		return -3; // Wrong direction for the command.
	}

	// Create a main buffer
	char tmp[200] = {0};

	// Define which payload to be added :
	if (command->result == OK) // ACK Command
	{
		/*
		 * Since we perfectly know the buffer at design time, the CRC was hard coded to save some instructions.
		 */
		switch (command->type)
		{
		case SHUTD:
			strcpy(tmp, "START;SHUTD;003;ACK;668281c8;END\r\n");
			break;

		case RINIT:
			strcpy(tmp, "START;RINIT;003;ACK;195df8ed;END\r\n");
			break;

		case DCONF:
			strcpy(tmp, "START;DCONF;003;ACK;2cb7e128;END\r\n");
			break;

		case CONNC:
			strcpy(tmp, "START;CONNC;003;ACK;ae4c7538;END\r\n");
			break;

		case UICON:
			strcpy(tmp, "START;UICON;003;ACK;3a0688a1;END\r\n");
			break;
		/**
		 * For the last two, we need to compute it on the target.
		 */
		case ASYNC:
			strcpy(tmp, "START;ASYNC;000;");
			int len = 183;

			// Add the payload to tmp
			if (build_async_payload(&command->ASYNC_RX, &tmp[16], &len) != 0)
			{
				return -4; // Failed to build the payload
			}

			// Add the lenght to tmp
			snprintf(&tmp[12], 4, "%03d", len);
			tmp[15] = ';'; // Replace the snprintf end character.


			// Add the separator
			int pos = 17 + len - 1;
			tmp[pos] = ';';
			pos += 1;

			// Get the CRC32 for the whole buffer
			int crc = crc_32((unsigned char*)tmp, pos);

			// Add the CRC and the END token to the buffer
			snprintf(&tmp[pos], 15, "%08x;END\r\n", crc);
			break;

		case SLPOS:
			strcpy(tmp, "START;SLPOS;000;");
			int len2 = 175;

			// Add the payload to tmp
			if (build_slpos_payload(&command->SLPOS_RX, &tmp[16], &len2) != 0)
			{
				return -4; // Failed to build the payload
			}

			// Add the lenght to tmp
			snprintf(&tmp[12], 4, "%03d", len2);
			tmp[15] = ';'; // Replace the snprintf end character.

			// Add the separator
			int pos2 = 17 + len2 - 1;
			tmp[pos2] = ';';
			pos2 += 1;

			// Get the CRC32 for the whole buffer
			int crc2 = crc_32((unsigned char*)tmp, pos2);

			// Add the CRC and the END token to the buffer
			snprintf(&tmp[pos2], 15, "%08x;END\r\n", crc2);
			break;

		default:
			return -32768; // We shall never get here...
		}
	}
	else // NACK command.
	{
		/*
		 * Since we perfectly know the buffer at design time, the CRC was hard coded to save some instructions.
		 */

		switch (command->type)
		{
		case SHUTD:
			strcpy(tmp, "START;SHUTD;004;NACK;3a028730;END\r\n");
			break;

		case RINIT:
			strcpy(tmp, "START;RINIT;004;NACK;71798c0e;END\r\n");
			break;

		case DCONF:
			strcpy(tmp, "START;DCONF;004;NACK;9a425028;END\r\n");
			break;

		case CONNC:
			strcpy(tmp, "START;CONNC;004;NACK;8777bbd8;END\r\n");
			break;

		case ASYNC:
			strcpy(tmp, "START;ASYNC;004;NACK;aac2078a;END\r\n");
			break;

		case UICON:
			strcpy(tmp, "START;UICON;004;NACK;0e30dac5;END\r\n");
			break;

		case SLPOS:
			strcpy(tmp, "START;SLPOS;004;NACK;c4c62382;END\r\n");
			break;

		default:
			return -32768; // We shall never get here...
		}
    }

	// Then, copy the output into the buffer
	memcpy((void *)buf, (void *)tmp, (size_t)sizeof(tmp));

	// Command building sucessfull. (return the number of bytes written).
	return strlen(tmp);
}
