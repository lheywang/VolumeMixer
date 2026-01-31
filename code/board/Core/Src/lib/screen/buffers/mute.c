/** ================================================================
 * @file    screen/buffers/icon.c
 *
 * @brief   This file implement the functions to that fill the
 * 			audio status (volume dependant, or mute) into the right
 * 			position.
 *
 * @date    30-06-2025
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
#include "lib/screen/buffers/buffers.h"

// Others header
#include "lib/screen/data/icons.h"

// STD
#include <stdint.h>
#include <string.h>

/* -----------------------------------------------------------------
 * VARIABLES
 * -----------------------------------------------------------------
 */
extern const uint8_t letters[4][11];

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */
int _draw_mute(struct BufferRequest* const cmd)
{
	// Clear buffer
	memset((void *)&cmd->buffer[MUTE_START], 0x00, (size_t)(MUTE_END - MUTE_START));

	// Add "MUTE" text
	if(cmd->command.status == MUTE)
	{
		for (int k = 0; k < 11; k++)
		{
			// Build a temporary word for the line
			uint32_t tmp;
			tmp = (letters[0][k] << 2) | (letters[1][k] << 9) | (letters[2][k] << 16) | (letters[3][k] << 23);

			// Copy into the buffer the correct bytes output
			cmd->buffer[(BYTES_PER_ROW * k) + MUTE_START + 0] = (tmp & 0xFF000000) >> 24;
			cmd->buffer[(BYTES_PER_ROW * k) + MUTE_START + 1] = (tmp & 0x00FF0000) >> 16;
			cmd->buffer[(BYTES_PER_ROW * k) + MUTE_START + 2] = (tmp & 0x0000FF00) >> 8;
			cmd->buffer[(BYTES_PER_ROW * k) + MUTE_START + 3] = (tmp & 0x000000FF);
		}
	}
	return 0;
}
