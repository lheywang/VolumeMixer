/** ================================================================
 * @file    mixer/parser/parser.h
 *
 * @brief   This file define the buffer parsing of the input
 *          command.
 *
 * @date 	30-05-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */
#pragma once

/* -----------------------------------------------------------------
 * INCLUDING LIBS
 * -----------------------------------------------------------------
 */

// Other headers
#include "parser/commands.h"

// STD
#include <stdint.h>

/* -----------------------------------------------------------------
 * ENUMS
 * -----------------------------------------------------------------
 */
enum COMMAND_TYPE
{
    // System management
    SHUTD,
    RST,

    // Connect requirement
    CONNC,

    // Usual commands
    ASYNC,
    UICON,
    SLPOS,
    DCONF,

};

enum COMMAND_DIR
{
    RX,
    TX
};

enum COMMAND_RES
{
    OK,
    NACK
};

/* -----------------------------------------------------------------
 * UNIONS
 * -----------------------------------------------------------------
 */
/**
 * Using an union enable memory space saving, since all members are
 * stored into a single base address.
 *
 * This is even more easier since we're able to handle a single command
 * at a time.
 */
struct CMD
{
    enum COMMAND_TYPE type;
    enum COMMAND_DIR direction;
    enum COMMAND_RES result;

    union
    {
        // Async command
        struct CMD_ASYNC_RX ASYNC_RX;
        struct CMD_ASYNC_TX ASYNC_TX;

        // UICON
        struct CMD_UICON_RX UICON_RX;

        // SLPOS
        struct CMD_SLPOS_TX SLPOS_TX;

        // DCONF
        struct CMD_DCONF_RX DCONF_RX;
    };

    uint32_t len;
    uint8_t payload[1000];
};

/* -----------------------------------------------------------------
 * PARSER
 * -----------------------------------------------------------------
 */

/**
 * @brief      Parse commands sent over serial / USB bus to the device.
 *
 * @details    Commands are expressed in this way :
 *   START;<CMD_NAME>;<LEN>;<DATA>;<CRC32>;END
 *
 *      with :
 *      - CMD_NAME : The command name, on 8 bytes of data. Can be
 *          - SHUTD
 *          - RESET
 *          - ASYNC
 *          - UICON
 *          - SLPOS
 *      - LEN : The length of the data received, in bytes (char). Expressed as 3 integer (up to 999)
 *      - DATA : Custom data passed. Always formatted as JSON.
 *      - CRC32 : Checksum of the whole message up to now (CRC32 and END excluded).
 *
 *      The command is borned with START and END tokens to clearly identify the
 *      start and end of a said command.
 *
 * @warning Commands can embbed a payload of max 999 chars. After that, the command will be
 *          considered as invalid.
 *
 * @param   buf     (char *)    The buffer that store the command buffer.
 * @param   command (int *)     A pointer to the storage of the command. Return the value.
 *
 * @return  int
 * @retval   0 :    Command sucessfully parsed.
 * @retval  -1 :    Invalid pointer passed
 * @retval -10 :    START token not found
 * @retval -11 :    Unknown command name.
 * @retval -12 :    Malformed command.
 * @retval -13 :    Invalid CRC32.
 * @retval -14 :    END token not found.
 */
int parser(char *buf, struct CMD *const command);
