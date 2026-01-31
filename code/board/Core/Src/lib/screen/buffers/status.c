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
#include <math.h>

/* -----------------------------------------------------------------
 * VARIABLES
 * -----------------------------------------------------------------
 */
extern const uint8_t volumes[5][128];

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */
int _draw_status(struct BufferRequest* const cmd)
{
	if (cmd->command.status == MUTE)
	{
		memcpy((void *)&cmd->buffer[SPEAKER_START], (void *)&volumes[0], (size_t)(SPEAKER_END - SPEAKER_START));
		return 0;
	}

	int vol = ceil((cmd->command.volume / 40) + 1.5);
	memcpy((void *)&cmd->buffer[SPEAKER_START], (void *)&volumes[vol], (size_t)(SPEAKER_END - SPEAKER_START));
	return 0;
}
