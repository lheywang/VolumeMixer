/** ================================================================
 * @file    mixer/mixer.h
 *
 * @brief   This file describe the base principle of the volume
 * 			mixer application code. This tool is based on an HAL
 * 			abstraction using small functions to make changes from
 * 			one platform to the other easier.
 *
 * @date 	30-05-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */
#pragma once

/* -----------------------------------------------------------------
 * ENUMS
 * -----------------------------------------------------------------
 */

// Used for the main state machine
enum states
{
    // Bases states
    BOARD_INIT,
    WAIT_FOR_CONNECT,
    WAIT_FOR_COMMAND,

    // States for different commands
    // Icon sync
    SYNC_READ_EEPROM,
    SYNC_COMPARE,
    SYNC_ANSWERING,

    // Slider request
    SLIDER_MEASURE,
    SLIDER_FILTERING,
    SLIDER_ANSWERING,

    // Icon update
    ICON_UPDATE,
    ICON_STORE,
    ICON_ANSWERING
};

// Used to sort commands
enum command
{
    // Flow commands
    HARD_RESET,
    SOFT_RESET,
    SHUTDOWN,

    // Operation commands
    SYNC,
    GET_POS,
    UPDATE
};

/* -----------------------------------------------------------------
 * MIXER
 * -----------------------------------------------------------------
 */

/**
 * 	@brief      Mixer function. Function that is called to update the
 *              mixer status.
 *
 * @param       None
 *
 * @return      int
 * @retval      0 :     Sucessfully updated mixer status.
 * @retval     -1 :     Error while updated status.
 */
int mixer();
