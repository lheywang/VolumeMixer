/** ================================================================
 * @file    screen/data/bigdecimals.c
 *
 * @brief   This file implement define the variable that store
 * 			the big decimals drawings on screen.
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
#include "screen/data/icons.h"

// STD
#include <stdint.h>

/* -----------------------------------------------------------------
 * VARIABLES
 * -----------------------------------------------------------------
 */

/*
 * That's an already rendered image, since there's a single option
 */
const uint8_t bigdecimals[64] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xf1, 0xfe, 0x38,
	0x1f, 0xfb, 0xff, 0x38, 0x1e, 0x7b, 0xcf, 0x38,
	0x1e, 0x7b, 0xcf, 0x38, 0x1e, 0x7b, 0xcf, 0x38,
	0x1e, 0x7b, 0xcf, 0x38, 0x1e, 0x7b, 0xcf, 0x38,
	0x1f, 0xfb, 0xff, 0x38, 0x0f, 0xf1, 0xfe, 0x38,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


