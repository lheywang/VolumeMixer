/** ================================================================
 * @file    maths/maths.c
 *
 * @brief   This file implement maths operations on the different buffers
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
// Header
#include "maths/maths.h"

/* -----------------------------------------------------------------
 * DEFINES
 * -----------------------------------------------------------------
 */

/* -----------------------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------------------
 */

int ADC2Double(uint16_t code, double *val);
int Double2ADC(double val, uint16_t *code);
int S162Double(uint8_t code, double *val);
int Double2S16(double val, uint8_t *code);



