/** ================================================================
 * @file    drivers/tca9548.c
 *
 * @brief   Source file of the TCA9548 I2C
 *
 * @date 	11-11-2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */

/*
 * -----------------------------------------------------------------
 * Includes
 * -----------------------------------------------------------------
 */
#include "drivers/tca9548.h"

#include <stdint.h>
#include <stddef.h>

#include "stm32f3xx_hal.h"

/*
 * -----------------------------------------------------------------
 * Private variables
 * -----------------------------------------------------------------
 */
static I2C_HandleTypeDef *tca9548_i2c;

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
int tca9548_init(I2C_HandleTypeDef *hi2c)
{
	tca9548_i2c = hi2c;

	HAL_Delay(100); // power-on delay

	// Then, call to disable all channels
	tca9548_disable_all();

	return 0;
}

int tca9548_select(uint8_t channel) {
    if (channel > 7) return -1;  // invalid channel
    uint8_t data = 1 << channel;
    return HAL_I2C_Master_Transmit(tca9548_i2c, TCA9548A_ADDR, &data, 1, HAL_MAX_DELAY);
}

int tca9548_select_mask(uint8_t channel) {
    return HAL_I2C_Master_Transmit(tca9548_i2c, TCA9548A_ADDR, &channel, 1, HAL_MAX_DELAY);
}

// Optionally disable all channels
int tca9548_disable_all(void) {
    uint8_t data = 0x00;
    return HAL_I2C_Master_Transmit(tca9548_i2c, TCA9548A_ADDR, &data, 1, HAL_MAX_DELAY);
}

uint8_t tca9548_get_active(void) {
    uint8_t data = 0;
    HAL_I2C_Master_Receive(tca9548_i2c, TCA9548A_ADDR, &data, 1, HAL_MAX_DELAY);
    return data;
}
