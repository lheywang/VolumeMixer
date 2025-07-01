/** ================================================================
 * @file    mixer/screen/sreen.h
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
#include <stdint.h>

/* -----------------------------------------------------------------
 * ENUMS
 * -----------------------------------------------------------------
 */
enum BufferRval {
	OK,
	MEMREQUEST,
	ERROR
};

enum ChannelStatus {
	MUTE,
	UNMUTE
};

enum RefreshType {
	FULL,
	VOLUME,
	STATUS
};

/* -----------------------------------------------------------------
 * STRUCTS
 * -----------------------------------------------------------------
 */
struct ScreenOrder {

	enum RefreshType type;
	enum ChannelStatus status;

	int volume;
	uint8_t icon[128];
};

struct BufferRequest {

	enum BufferRval status;

	uint16_t address;
	uint16_t len;
	uint8_t buffer[512];

	struct ScreenOrder command;

};

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/**
 * @brief	Fill a buffer with the appropriate data to be displayed on the screen
 *
 * @param 	cmd	A struct that contain the different orders.
 * @return	BufferRequest : A struct that contain different infos about the buffer, including it.
 */
struct BufferRequest* draw_buffer(struct ScreenOrder const *cmd);

#ifdef __cplusplus
}
#endif
