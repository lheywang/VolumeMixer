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
 * DEFINES
 * -----------------------------------------------------------------
 */
#define SLPOS_ERROR_CODE(x) (-x - ...)

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

int build_slpos_payload(const struct CMD_SLPOS_RX * const cmd, const char *buf, const int * len)
{
	// Inputs safety
	if ((cmd == NULL) | (buf == NULL) | (len == NULL))
	{
		return -1;
	}

	// First, check the different arguments :
	if ((0 > cmd->slider1.Pos) | (cmd->slider1.Pos > 100))
	{
		return -101;
	}
	if ((0 > cmd->slider2.Pos) | (cmd->slider2.Pos > 100))
	{
		return -101;
	}
	if ((0 > cmd->slider3.Pos) | (cmd->slider3.Pos > 100))
	{
		return -101;
	}
	if ((0 > cmd->slider4.Pos) | (cmd->slider4.Pos > 100))
	{
		return -101;
	}
	if ((0 > cmd->slider5.Pos) | (cmd->slider5.Pos > 100))
	{
		return -101;
	}


	return 0;
}
