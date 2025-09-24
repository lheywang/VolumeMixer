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

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------
 * INCLUDING LIBS
 * -----------------------------------------------------------------
 */

// Other headers
#include "parser/commands.h"

// STD
#include <stdint.h>

/* -----------------------------------------------------------------
 * DEFINES
 * -----------------------------------------------------------------
 */
#define PARSER_PAYLOAD_LENGTH 384UL

/* -----------------------------------------------------------------
 * ENUMS
 * -----------------------------------------------------------------
 */
enum COMMAND_TYPE
{
    // System management
    SHUTD,
    RINIT,

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
	INPUT, // Raw input buffer.
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
        struct CMD_ASYNC_TX ASYNC_TX;
        struct CMD_ASYNC_RX ASYNC_RX;

        // UICON
        struct CMD_UICON_TX UICON_TX;

        // SLPOS
        struct CMD_SLPOS_RX SLPOS_RX;

        // DCONF
        struct CMD_DCONF_TX DCONF_TX;
    };

    uint32_t len;
    uint32_t crc;
    uint8_t payload[PARSER_PAYLOAD_LENGTH]; // The longest payload used is going to be 316 bytes long. More than enough !
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
 * @warning Commands can embbed a payload of max 995  chars. After that, the command will be
 *          considered as invalid.
 *
 * @warning The input buffer is considered as been 1024 chars longs. Thus, no checks are done on this part.
 * 			The lib may then cause SegFault if used with mallocs.
 *
 * @param   buf     (char *)    The buffer that store the command buffer.
 * @param   command (int *)     A pointer to the storage of the command. Return the value.
 * @param 	ParsePayload (int) 	Shall we attempt to parse the payload ? (This make testing a bit easier)
 *
 * @return  int
 * @retval   0 :    Command sucessfully parsed.
 * @retval  -1 :    Invalid pointer passed
 * @retval -10 :    START token not found
 * @retval -11 :    Unknown command name.
 * @retval -12 :    Malformed command.
 * @retval -13 :	Could not convert properly len.
 * @retval -14 :	Invalid CRC32 structure.
 * @retval -15 :	Could not convert the CRC to it's value.
 * @retval -16 :   	CRC32 does not match the input string.
 * @retval -17 : 	END token not found.
 */
int parser(char *buf, struct CMD * const command, int ParsePayload);

/* -----------------------------------------------------------------
 * BUILDER
 * -----------------------------------------------------------------
 */

/**
 * @brief		Build serial commands to be sent on the serial bus. Handle all of the nasty for the user.
 *
 * @details		Used to build responses to a command, while ensuring it does match the specification.
 * 				Internally, the function check for different elements such as :
 * 					- Command direction
 * 					- Command status
 *
 * 				They're used to define which content shall be placed into the string before been sent.
 * 				Basically : in the payload there may be a payload if the command support it, or a
 * 					ACK / NACK bit. A valid JSON payload is handled as an ACK bit.
 *
 * @param 	command (CMD*)		A pointer to the command to be built.
 * @param 	buf (char *) 		A pointer to the output buffer.
 *
 * @return	int
 * @retval 	 0 :	Command successfully built.
 * @retval 	-1 : 	Invalid pointer.
 * @retval  -2 :	Command need to be treated before responding.
 * @retval  -3 : 	Wrong direction for the command.
 * @retval  -4 : 	Failed to build the payload
 * @retval  -5 :	Failed to add the length
 * @retval  -6 : 	Failed the final formatting.
 */
int builder(struct CMD * const command, char *buf);

#ifdef __cplusplus
}
#endif
