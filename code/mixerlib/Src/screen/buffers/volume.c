/** ================================================================
 * @file    screen/buffers/volume.c
 *
 * @brief   This file implement the functions to that draw the
 * 			volume number onto the buffer.
 *
 * @date    27-06-2025
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
// Header
#include "screen/buffers/buffers.h"

// Others header
#include "screen/data/icons.h"

// STD
#include <stdint.h>
#include <string.h>

/* -----------------------------------------------------------------
 * VARIABLES
 * -----------------------------------------------------------------
 */
extern const uint8_t decimals[10][32];
extern const uint8_t bigdecimals[64];

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */
int _draw_volume(struct BufferRequest* const cmd)
{
	// First, ensure the value is not 100:
	if (cmd->command.volume >= 100)
	{
		memcpy((void *)&cmd->buffer[VOLUME_START], (void *)&bigdecimals, (size_t)(VOLUME_END - VOLUME_START));
	}
	else
	{
		// First, fetch the caracters
		uint8_t ids[2][32] = { 0 };
		memcpy((void *)&ids[0], (void *)&decimals[cmd->command.volume / 10], (size_t)32);
		memcpy((void *)&ids[1], (void *)&decimals[cmd->command.volume % 10], (size_t)32);

		for (int k = 0; k < 16; k++)
		{
			// Copy into the buffer the correct bytes output
			cmd->buffer[(BYTES_PER_ROW * k) + VOLUME_START + 0] = (~ids[1][2 * k]);
			cmd->buffer[(BYTES_PER_ROW * k) + VOLUME_START + 1] = (~ids[1][(2 * k) + 1]) << 1;
			cmd->buffer[(BYTES_PER_ROW * k) + VOLUME_START + 2] = (~ids[0][2 * k]);
			cmd->buffer[(BYTES_PER_ROW * k) + VOLUME_START + 3] = (~ids[0][(2 * k) + 1]);
		}
	}
	return 0;
}
