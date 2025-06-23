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

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

int parse_async_payload(const char * buf, struct CMD_ASYNC_TX * const cmd)
{
	return 0;
}

int build_async_payload(const struct CMD_ASYNC_RX * const cmd, const char *buf, const int * len)
{
	return 0;
}

