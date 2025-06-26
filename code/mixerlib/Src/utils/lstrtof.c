/** ================================================================
 * @file    utils/lstrtof.c
 *
 * @brief   This file define a custom implementation of the
 * 			strtof function, since using it require TOO MUCH space
 * 			on the STM32 (like 4kB only for two cast...).
 * 			This implementation reduce the safety as well as the
 * 			functions, but anyway we don't use it.
 *
 * @warning Do not use for general purpose casting, please ensure
 * 			you known what you're doing precisely.
 *
 * @date    26-06-2025
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
#include "utils/lstrtof.h"

// STD
#include <stdlib.h>
#include <ctype.h>

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

float lstrtof(char *input, char **end)
{
	// Check for null terminated string.
	if (input[0] == '\0')
	{
		*end = &input[0];
		return 0.0f;
	}

	// Check for sign
	float sign;
	int intpos = 0;
	switch (input[0])
	{
	case '-':
		sign = -1.0f;
		intpos = 1;
		break;
	case '+':
		intpos = 1;
	default:
		sign = 1.0f;
		break;
	}

	// Create some buffers
	char intbuf[INTEGER_PART_BUFFER_SIZE + 1] = {0};
	char fpbuf[DECIMAL_PART_BUFFER_SIZE + 1] = {0};
	float out = 0.0f;

	// Set a variable to track the offset applied to intpos (to not goes over the bound of the buffers arrays
	int off = (intpos == 1) ? 1 : 0;

	// Fetch the integer part of the number
	while(isdigit(input[intpos])) // Check for separator or end of string
	{
		// Check if we risk an overflow of the buffer
		if (intpos > INTEGER_PART_BUFFER_SIZE)
		{
			*end = &input[0];
			return 0.0f;
		}

		// Copy the character into our temporary buffer.
		intbuf[intpos - off] = input[intpos];
		intpos ++;
	}
	// Goes above the '.' ',' or '\0'
	intpos ++;

	// Cast the output using strtol backend
	char *tmp;
	out += (float)strtol(intbuf, &tmp, 10);
	if (intbuf == tmp)
	{
		*end = &input[0];
		return 0.0f;
	}

	// Fetch the decimal part of the number
	int posbckp = intpos;
	while(isdigit(input[intpos])) // Check for separator or end of string
	{
		// Check if we risk an overflow of the buffer
		if ((intpos - posbckp) > DECIMAL_PART_BUFFER_SIZE)
		{
			*end = &input[0];
			return 0.0f;
		}

		// Copy the character into our temporary buffer.
		fpbuf[intpos - posbckp] = input[intpos];
		intpos ++;
	}

	// Compute the divier of the decimal part
	int div = 1;
	for (int k = (intpos - posbckp); k > 0; k--)
		div *= 10;

	// Cast the output using strtol backend
	out += ((float)strtol(fpbuf, &tmp, 10)) / div; // Add the decimal part of the number
	if (intbuf == tmp)
	{
		*end = &input[0];
		return 0.0f;
	}

	// Update end pointer
	if (posbckp == intpos)
		*end = &input[intpos - 2];
	else
		*end = &input[intpos - 1];

	// Return while applying sign
	return out * sign;
}
