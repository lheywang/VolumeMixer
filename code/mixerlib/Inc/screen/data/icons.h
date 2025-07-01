/** ================================================================
 * @file    screen/data/volume0.h
 *
 * @brief   This file define the icon for the volume 0
 *
 * @date    27-06-2025
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
 * INCLUDES
 * -----------------------------------------------------------------
 */
#include <stdint.h>

/* -----------------------------------------------------------------
 * VARIABLES
 * -----------------------------------------------------------------
 */

extern const uint8_t volumes[5][128]; // Volumes
extern const uint8_t letters[4][11]; // MUTE Letters
extern const uint8_t decimals[10][32]; // Numbers
extern const uint8_t bigdecimals[2][20]; // Smaller numbers (only 1 and 0)

#ifdef __cplusplus
}
#endif
