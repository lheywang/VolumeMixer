/*
 * main.cpp
 *
 *  Created on: May 26, 2025
 *      Author: leona
 */
#define STM32F302x8 // Shall be defined by default, but why does this line solve the issue ?
#include <main.h>
#include <hal/leds.hpp>
#include <hal/init.hpp>

int main(void){

	init::full();

	while (1){

		// HAL_UART_Transmit(&huart2, data, 15, 100);
		leds::debug_Toggle_Led();
		HAL_Delay(1000);
	}
}




