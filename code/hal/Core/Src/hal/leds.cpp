/** ================================================================
 * @file    Core/Src/hal/leds.cpp
 *
 * @brief   leds.cpp Implement low level IO with the on-board LEDs
 *
 * @date    26-05-2025
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
#include "hal/leds.hpp"
#include "stm32f3xx_hal.h"

/* -----------------------------------------------------------------
 * INITIALIZATION FUNCTIONS
 * -----------------------------------------------------------------
 */

#include "main.h"

int leds::debug_Toggle_Led(){
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	return 0;
}
