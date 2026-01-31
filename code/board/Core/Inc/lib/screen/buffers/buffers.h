/** ================================================================
 * @file    mixer/screen/buffers/buffers.h
 *
 * @brief   This file implement the functions that handle
 * 			the bytestream creation for the screen
 *
 * @date    30-05-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------
 * INCLUDING LIBS
 * -----------------------------------------------------------------
 */
// Others headers
#include "lib/screen/screen.h"

// STD
#include <stdint.h>

/* -----------------------------------------------------------------
 * DEFINES
 * -----------------------------------------------------------------
 */
/*
 * How many bytes are needed for each rows ?
 */
#define BYTES_PER_ROW 4
/*
 * We use them to configure the placement of the different elements on the screen
 */
#define ICON_START 32
#define ICON_LEN 32 // rows

#define SPEAKER_START 192
#define SPEAKER_LEN 32 // rows

#define VOLUME_START 352
#define VOLUME_LEN 16 // rows

#define MUTE_START 448
#define MUTE_LEN 8 // rows

/*
 * We use theses define to autocalculate the end of the block
 */
#define ICON_END (ICON_START + (BYTES_PER_ROW * ICON_LEN))
#define SPEAKER_END (SPEAKER_START + (BYTES_PER_ROW * SPEAKER_LEN))
#define VOLUME_END (VOLUME_START + (BYTES_PER_ROW * VOLUME_LEN))
#define MUTE_END (MUTE_START + (BYTES_PER_ROW * MUTE_LEN))

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/**
 * @brief	Draw the mute symbol, if needed on the output buffer.
 *
 * @param 	cmd		Pointer to the BufferRequest struct.
 * @return	int
 */
int _draw_mute(struct BufferRequest* const cmd);

/**
 * @brief	Draw the icon.
 *
 * @param 	cmd		Pointer to the BufferRequest struct.
 * @return	int
 */
int _draw_icon(struct BufferRequest* const cmd);

/**
 * @brief	Draw the volume symbol, if needed on the output buffer.
 *
 * @param 	cmd		Pointer to the BufferRequest struct.
 * @return	int
 */
int _draw_volume(struct BufferRequest* const cmd);

/**
 * @brief	Draw the status symbol, if needed on the output buffer.
 *
 * @param 	cmd		Pointer to the BufferRequest struct.
 * @return	int
 */
int _draw_status(struct BufferRequest* const cmd);

#ifdef __cplusplus
}
#endif
