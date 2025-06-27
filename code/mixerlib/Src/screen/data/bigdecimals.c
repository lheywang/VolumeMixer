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

const uint8_t bigdecimals[2][20] = {
	/**
	 * 0 : 10 x 10 px
	 */
	{
		0xff, 0xc0, 0xff, 0xc0,
		0x80, 0x40, 0x8c, 0x40,
		0x8c, 0x40, 0x8c, 0x40,
		0x8c, 0x40, 0x8c, 0x40,
		0x80, 0x40, 0xff, 0xc0
	},
	/**
	 * 1 : 10 x 10 px
	 */
	{
		0xff, 0xc0, 0xf1, 0xc0,
		0xf1, 0xc0, 0xf1, 0xc0,
		0xf1, 0xc0, 0xf1, 0xc0,
		0xf1, 0xc0, 0xf1, 0xc0,
		0xf1, 0xc0, 0xff, 0xc0
	}
};


