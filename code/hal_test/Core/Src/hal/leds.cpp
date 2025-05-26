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

#include "main.h"

int debug_Toggle_Led(){
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);

	return 0;
}
