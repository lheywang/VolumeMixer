/** ================================================================
 * @file    mixer/screen/sreen.c
 *
 * @brief   This file implement the functions to command the screen
 *
 * @date    30-05-2025
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
#include "screen/screen.h"

// Others headers
#include "screen/buffers/buffers.h"

// STD
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* -----------------------------------------------------------------
 * BUFFER VARIABLES
 * -----------------------------------------------------------------
 */
/*
 * Buffer for the output type
 */
struct BufferRequest __rval = {
		.status = SCREEN_ERROR,
		.address = 0x0000,
		.buffer = { 0 },
		.len = 0x0000,
};


/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

struct BufferRequest* draw_buffer(struct ScreenOrder const *cmd)
{
	// input checks
	if (cmd == NULL)
	{
		__rval.status = SCREEN_ERROR;
		return &__rval;
	}

	// Clear the actual buffer
	memset((void *)&__rval.buffer, 0x00, (size_t)512);

	// Ensure the return struct is correctly filled
	memcpy((void *)&__rval.command, (void *)cmd, (size_t)sizeof(struct ScreenOrder));

	// Then, fill the buffer
	switch (cmd->type)
	{
	case FULL:
		_draw_icon(&__rval);
		_draw_volume(&__rval);
		_draw_status(&__rval);
		_draw_mute(&__rval);
		break;
	case VOLUME:
		_draw_volume(&__rval);
		_draw_status(&__rval);
		break;
	case STATUS:
		_draw_mute(&__rval);
		break;
	}

	// Finally, set flags and returns
	__rval.status = SCREEN_OK;
	return &__rval;
}





