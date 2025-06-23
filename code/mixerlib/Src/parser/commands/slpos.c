/** ================================================================
 * @file    mixer/parser/commands/slpos.c
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

int build_slpos_payload(const struct CMD_SLPOS_RX * const cmd, const char *buf, const int * len)
{
	return 0;
}
