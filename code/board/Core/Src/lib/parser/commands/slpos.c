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
#include "lib/parser/commands.h"

// STD
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

/* -----------------------------------------------------------------
 * DEFINES
 * -----------------------------------------------------------------
 */
#define SLPOS_ERROR_CODE(x) (-x - 70)

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/**
 * {
 *  "sliders" : [
 *      {
 *          "1"     : "082",
 *          "1M"    : "0"
 *      },
 *              {
 *          "2"     : "029",
 *          "2M"    : "0"
 *      },
 *              {
 *          "3"     : "050",
 *          "3M"    : "1"
 *      },
 *              {
 *          "4"     : "100",
 *          "4M"    : "1"
 *      },
 *              {
 *          "5"     : "000",
 *          "5M"    : "1"
 *      }
 *  ]
 *}
 *
 * {"sliders":[{"1":082","1M":"1"},{"2":"029","2M":"0"},{"3":"050","3M":"1"},{"4":"100","4M":"1"},{"5":"000","5M":"1"}]}
 *
 */

int build_slpos_payload(const struct CMD_SLPOS_RX * const cmd, char *buf, int * len)
{
	// First, check for the different parameters
	if ((cmd == NULL) | (buf == NULL) | (len == NULL))
	{
		return -1; // Invalid pointer
	}

	if (*len < 120)
	{
		return -2; // Buffer too small
	}

	*len = snprintf(buf,
			120,
			"{\"sliders\":[{\"1\":\"%03ld\",\"1M\":\"%1ld\"},{\"2\":\"%03ld\",\"2M\":\"%1ld\"},{\"3\":\"%03ld\",\"3M\":\"%1ld\"},{\"4\":\"%03ld\",\"4M\":\"%1ld\"},{\"5\":\"%03ld\",\"5M\":\"%1ld\"}]}",
			(cmd->sliders[0].Pos > 0 ? cmd->sliders[0].Pos : 0),
			cmd->sliders[0].Mute,
			(cmd->sliders[1].Pos > 0 ? cmd->sliders[1].Pos : 0),
			cmd->sliders[1].Mute,
			(cmd->sliders[2].Pos > 0 ? cmd->sliders[2].Pos : 0),
			cmd->sliders[2].Mute,
			(cmd->sliders[3].Pos > 0 ? cmd->sliders[3].Pos : 0),
			cmd->sliders[3].Mute,
			(cmd->sliders[4].Pos > 0 ? cmd->sliders[4].Pos : 0),
			cmd->sliders[4].Mute
			);
	return 0;
}
