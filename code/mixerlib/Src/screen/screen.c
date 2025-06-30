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

// STD
#include <stdint.h>
#include <stddef.h>

/* -----------------------------------------------------------------
 * BUFFER VARIABLES
 * -----------------------------------------------------------------
 */
/*
 * Theses store the raw bytestream out of the drawing functions.
 * There's one only one of them, since we're not supposed to get high
 * refresh rates, and we may use a simple partial refresh.
 */
uint8_t __video_buffer[512] = { 0 };

/*
 * Buffer for the output type
 */
struct BufferRequest __rval = {
		.status = ERROR,
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
		__rval.status = ERROR;
		return &__rval;
	}

	switch (cmd->type)
	{
	case FULL:
		break;
	case VOLUME:
		break;
	case STATUS:
		break;
	}
}





