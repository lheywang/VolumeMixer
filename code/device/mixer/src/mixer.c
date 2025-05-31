/** ================================================================
 * @file    mixer/mixer.c
 *
 * @brief   This file implement the base principle of the volume
 *          mixer application code. This tool is based on an HAL
 *          abstraction using small functions to make changes from
 *          one platform to the other easier.
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
// header
#include "mixer.h"

// Other headers
#include "parser/parser.h"

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

int mixer()
{
    static int actual = BOARD_INIT;

    switch (actual)
    {

    /*
     * Cases for base operating point
     */
    case BOARD_INIT:
    {
        // End of board init
        actual = WAIT_FOR_CONNECT;
        break;
    }

    case WAIT_FOR_CONNECT:
    {
        // Connect command received.
        actual = WAIT_FOR_COMMAND;
        break;
    }

    case WAIT_FOR_COMMAND:
    {
        // Command received, lets's continue !
        char *buf = "TEST";
        int len = sizeof(buf);

        // parse commands
        // parser(buf);
        break;
    }

    /*
     * Cases for Application Sync
     */
    case SYNC_READ_EEPROM:
    {
        // Going to next step
        actual = SYNC_COMPARE;
        break;
    }

    case SYNC_COMPARE:
    {
        // Going to next step
        actual = SYNC_ANSWERING;
        break;
    }

    case SYNC_ANSWERING:
    {
        // End of command
        actual = WAIT_FOR_COMMAND;
        break;
    }

        /*
         * Cases for slider update
         */

    case SLIDER_MEASURE:
    {
        // Going to next step
        actual = SLIDER_FILTERING;
        break;
    }

    case SLIDER_FILTERING:
    {
        // Going to next step
        actual = SLIDER_ANSWERING;
        break;
    }

    case SLIDER_ANSWERING:
    {
        // End of command
        actual = WAIT_FOR_COMMAND;
        break;
    }

    /*
     * Cases for icon updates
     */
    case ICON_UPDATE:
    {
        // Going to next step
        actual = ICON_ANSWERING;
        break;
    }

    case ICON_STORE:
    {
        // Going to next step
        actual = ICON_ANSWERING;
        break;
    }

    case ICON_ANSWERING:
    {
        // End of code here
        actual = WAIT_FOR_COMMAND;
        break;
    }

        /*
         * Handling default case
         */

    default:
    {
        actual = BOARD_INIT;
        break;
    }
    }

    return 0;
}
