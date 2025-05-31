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
#include "main.h"

// STD
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* -----------------------------------------------------------------
 * EXTERN VARIABLES
 * -----------------------------------------------------------------
 */
extern CRC_HandleTypeDef hcrc;

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

int parser(char *buf, struct CMD *const command)
{
    // Input checks (prevent from null pointer)
    if ((buf == NULL) | (command == NULL))
    {
        return -1; // Invalid pointer
    }

    // Create a working buffer
    uint8_t work[16] = {0};

    // Search for "START;" token.
    memset((void *)work, 0x00, 16);
    memcpy((void *)work, (void *)buf, (size_t)6);
    char *ref = "START;";
    if (strcmp((char *)work, (char *)ref) != 0)
    {
        return -10; // START token not found
    }

    // Search for the command name
    memset((void *)work, 0x00, 16);
    memcpy((void *)work, (void *)&buf[6], (size_t)6);
    char *refcmd[] = {
        "SHUTD;",
        "RESET;",
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
        }
    }
    switch (cmd_id)
    {
    case 0:
        command->type = SHUTD;
        break;
    case 1:
        command->type = RESET;
        break;
    case 2:
        command->type = DCONF;
        break;
    case 3:
        command->type = CONNC;
        break;
    case 4:
        command->type = ASYNC;
        break;
    case 5:
        command->type = UICON;
        break;
    case 6:
        command->type = SLPOS;
        break;
    default:
        return -11; // Unknown command
    }

    // Fetch the payload lenght and the data by itself
    memset((void *)work, 0x00, 16);
    memcpy((void *)work, (void *)&buf[12], (size_t)4);
    if (work[3] != ';')
    {
        return -12; // Malformed command
    }
    work[3] = '\0';
    command->len = atoi((char *)work);
    memcpy((void *)command->payload, (void *)&buf[16], (size_t)command->len);

    // Fetch the CRC32 of the whole message
    memset((void *)work, 0x00, 16);
    memcpy((void *)work, (void *)&buf[16 + command->len + 1], (size_t)5);
    if (work[4] != ';')
    {
        return -12; // Malformed command
    }
    work[3] = '\0';
    uint32_t readCRC = atoi((char *)work);

    // Calculate CRC for the whole received message
    uint8_t crc_buf[1024] = {0};
    memcpy((void *)crc_buf, (void *)buf, (size_t)(16 + command->len + 1));
    uint32_t calcCRC = HAL_CRC_Calculate(&hcrc, (uint32_t *)crc_buf, (((16 + command->len + 1) * sizeof(uint8_t)) / sizeof(uint32_t)));

    // Compare CRC
    if (calcCRC != readCRC)
    {
        return -13; // Invalid CRC.
    }

    // Search for "END" token.
    memset((void *)work, 0x00, 16);
    memcpy((void *)work, (void *)&buf[21 + command->len + 1], (size_t)3);
    char *ref2 = "END";
    if (strcmp((char *)work, (char *)ref2) != 0)
    {
        return -14; // END token not found
    }

    // Parser sucessfull
    return 0;
}
