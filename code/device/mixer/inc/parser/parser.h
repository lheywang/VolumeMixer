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

/* -----------------------------------------------------------------
 * INCLUDING LIBS
 * -----------------------------------------------------------------
 */

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
 * @param   len     (int )      The length in byte of the buffer.
 * @param   command (int *)     A pointer to the storage of the command. Return the value.
 *
 * @return  int
 * @retval   0 :    Command sucessfully parsed.
 * @retval  -1 :    Unknown command name.
 * @retval  -2 :    Invalid CRC32.
 * @retval  -3 :    END token not found.
 * @retval  -4 :    Command too long.
 */
int parser(char *buf, const int len, int const *command);