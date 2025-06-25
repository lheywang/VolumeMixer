/** ================================================================
 * @file    mixer/parser/parser.c
 *
 * @brief   This file implement the buffer parsing of the input
 *          command.
 *
 * @date    30-05-2025
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
#include "parser/parser.h"

// locals libs
#include "parser/commands.h"
#include "CRC/checksum.h"

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
#define PARSER_BUFFER_SIZE 16
#define PARSER_ERROR_CODE(x) (-x - 10)

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

int parser(char *buf, struct CMD * command, int ParsePayload)
{
    // Input checks (prevent from null pointer)
    if ((buf == NULL) | (command == NULL))
    {
        return -1; // Invalid pointer
    }

    // Create a working buffer
    uint8_t work[PARSER_BUFFER_SIZE] = {0};

    // Search for "START;" token.
    memcpy((void *)work, (void *)buf, (size_t)6);
    char *ref = "START;";
    if (strcmp((char *)work, (char *)ref) != 0)
    {
        return PARSER_ERROR_CODE(0); // START token not found
    }

    // Search for the command name
    memset((void *)work, 0x00, PARSER_BUFFER_SIZE);
    memcpy((void *)work, (void *)&buf[6], (size_t)6);
    char *refcmd[] = {
        "SHUTD;",
        "RINIT;",
        "DCONF;",
        "CONNC;",
        "ASYNC;",
        "UICON;",
        "SLPOS;",
    };
    uint32_t cmd_id = -1;
    for (uint8_t k = 0; k < 7; k++)
    {
        if (strcmp((char *)work, (char *)refcmd[k]) == 0)
        {
            cmd_id = k;
            break;
        }
    }
    switch (cmd_id)
    {
    case 0:
        command->type = SHUTD;
        command->direction = TX;
        command->result = INPUT;
        break;
    case 1:
        command->type = RINIT;
        command->direction = TX;
        command->result = INPUT;
        break;
    case 2:
        command->type = DCONF;
        command->direction = TX;
        command->result = INPUT;
        break;
    case 3:
        command->type = CONNC;
        command->direction = TX;
        command->result = INPUT;
        break;
    case 4:
        command->type = ASYNC;
        command->direction = TX;
        command->result = INPUT;
        break;
    case 5:
        command->type = UICON;
        command->direction = TX;
        command->result = INPUT;
        break;
    case 6:
        command->type = SLPOS;
        command->direction = TX;
        command->result = INPUT;
        break;
    default:
    	command->result = NACK;
        return PARSER_ERROR_CODE(1); // Unknown command
    }

    // Fetch the payload lenght and the data by itself
    memset((void *)work, 0x00, PARSER_BUFFER_SIZE);
    memcpy((void *)work, (void *)&buf[12], (size_t)4);
    if (work[3] != ';')
    {
        return PARSER_ERROR_CODE(2); // Malformed command
    }
    work[3] = '\0';

    // Cast to digit
    for (int k = 0; k < 3; k ++)
    {
        // Attempt a conversion and check for the errno
    	if (!isdigit(work[k]))
    	{
    		command->len = 0;
    		return PARSER_ERROR_CODE(3);
    	}
    }
    command->len = strtol((char *)work, NULL, 10);

    memset((void*)command->payload, 0x00, (size_t)PARSER_PAYLOAD_LENGTH);
    memcpy((void *)command->payload, (void *)&buf[16], (size_t)command->len);

    // Fetch the CRC32 of the whole message
    memset((void *)work, 0x00, PARSER_BUFFER_SIZE);
    memcpy((void *)work, (void *)&buf[16 + command->len + 1], (size_t)9);
    if (work[8] != ';')
    {
        return PARSER_ERROR_CODE(4); // Malformed command
    }
    work[8] = '\0';

    // Cast to digit
    for (int k = 0; k < 8; k ++)
    {
        // Attempt a conversion and check for the errno
    	if (!isxdigit(work[k]))
    	{
    		command->crc = 0x00000000UL;
    		return PARSER_ERROR_CODE(5);
    	}
    }
    command->crc = strtol((char *)work, NULL, 16);

    // Calculate CRC for the whole received message
    uint8_t crc_buf[384] = {0}; // The longest we may use it 316 bytes payload. Add the small 16 bytes, 384 is way more than needed.
    size_t len = (size_t)(16 + command->len + 1);
    memcpy((void *)crc_buf, (void *)buf, len);
    uint32_t crc = crc_32(crc_buf, len);

    // Comparing both CRC
    if (crc != command->crc)
    {
    	return PARSER_ERROR_CODE(6); // CRC does not match
    }

    // Search for "END" token.
    memset((void *)work, 0x00, PARSER_BUFFER_SIZE);
    memcpy((void *)work, (void *)&buf[25 + command->len + 1], (size_t)3);
    char *ref2 = "END";
    if (strcmp((char *)work, (char *)ref2) != 0)
    {
        return PARSER_ERROR_CODE(7); // END token not found
    }

    // If needed, parse the payload.
    if (ParsePayload == 1)
    {
		switch (command->type)
		{
		case DCONF:
			return parse_dconf_payload((char*)&command->payload, command->len, &command->DCONF_TX);
			break;

		case ASYNC:
			return parse_async_payload((char*)&command->payload, command->len, &command->ASYNC_TX);
			break;

		case UICON:
			return parse_uicon_payload((char*)&command->payload, command->len, &command->UICON_TX);
			break;

		default:
			// Parser sucessfull
			command->result = INPUT;
			return 0;
			break;
		}
    }
    else
    {
    	return 0;
    }

    return -32768; // We sould NEVER get here...
}
