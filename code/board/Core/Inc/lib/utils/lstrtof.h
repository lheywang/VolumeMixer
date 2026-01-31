/** ================================================================
 * @file    utils/lstrtof.h
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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------
 * DEFINES
 * -----------------------------------------------------------------
 */
/*
 * Use theses to configure the casting procedure.
 *
 * They configure the buffer size used for the casting, so choose a correct size.
 * Bigger buffers means larger numbers, but more RAM usage.
 * Smaller buffers means smaller numbers, but constrainted RAM usage.
 *
 * In our case, the numbers are +X.XXX or X.XX formatted, for buffer of 2.4 is perfectly fine.
 */
#define INTEGER_PART_BUFFER_SIZE 2
#define DECIMAL_PART_BUFFER_SIZE 4

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

/**
 * @brief 	Convert an char array terminated by a '\0' to it's floating
 *			point representation.
 *			Use the same signature as the original strtof function.
 *
 * @warning This is a custom implementation of the strtof function,
 * 			with heavily reduced capacibilites. Do not use if not
 * 			required.
 *
 * @details Support floats expression as [+-]X--X.X---X only.
 *
 * @param 	*input 	Pointer to a char array to be used as input
 * @param 	**end	Pointer to a char**, to be filled with the last character used by conversion.
 */
float lstrtof(char *input, char **end);

#ifdef __cplusplus
}
#endif
