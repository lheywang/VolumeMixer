/** ================================================================
 * @file    mixer/parser/parser.c
 *
 * @brief   This file implement the buffer parsing of the input
 *          command.
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
#include "utils/screen_utils.hpp"

// STD
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

/* -----------------------------------------------------------------
 * EXAMPLE DATA
 * -----------------------------------------------------------------
 */

int show_icon(uint8_t *buf)
{
	for (int col = 0; col < 32; col ++)
	{
		// Fetch data
		int tmp = buf[4 * col]  << 24 | buf[(4 * col) + 1]  << 16 | buf[(4 * col) + 2]  << 8 | buf[(4 * col) + 3];

		for (int row = 0; row < 32; row ++)
		{
			if ((tmp & (1 << row)) != 0)
			{
				printf("o");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\r\n");
	}

	return 0;
}

int show_buffer(uint8_t *buf)
{
	for (int col = 0; col < 128; col ++)
	{
		// Fetch data
		int tmp = buf[4 * col]  << 24 | buf[(4 * col) + 1]  << 16 | buf[(4 * col) + 2]  << 8 | buf[(4 * col) + 3];

		for (int row = 0; row < 32; row ++)
		{
			if ((tmp & (1 << row)) != 0)
			{
				printf("o");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\r\n");
	}

	return 0;
}
