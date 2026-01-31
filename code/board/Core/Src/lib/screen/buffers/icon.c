/** ================================================================
 * @file    screen/buffers/icon.c
 *
 * @brief   This file implement the functions to that fill an icon
 * 			into the main graphical buffer.
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
#include "lib/screen/buffers/buffers.h"

// Others header
#include "lib/screen/data/icons.h"

// STD
#include <stdint.h>
#include <string.h>

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

inline int _draw_icon(struct BufferRequest* const cmd)
{
	// This function only copy the buffer from the output into the right place
	memcpy((void *)&cmd->buffer[ICON_START], (void *)cmd->command.icon, (size_t)(ICON_END - ICON_START));
	return 0;
}



