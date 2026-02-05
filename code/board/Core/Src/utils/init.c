/** ================================================================
 * @file    utils/CDC_helper.c
 *
 * @brief   Source file of the helper utils for the CDC_helper file
 *
 * @date 	24-09-2025
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
// Header
#include "utils/init.h"

// STD
#include <stdint.h>
#include <stdio.h>

// Lib
#include "fsm/fsm.h"

#include "drivers/tca9548.h"
#include "drivers/ssd1306.h"
#include "drivers/24cl64.h"

#include "handlers/timers.h"
#include "handlers/leds.h"
#include "handlers/tsc.h"

#include "lib/eeprom/eeprom.h"

#include "utils/logger.h"


// ST
#include "stm32f3xx_hal.h"
#include "main.h"

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart3;

extern uint8_t uart_message[96];

extern GPIO_PWMx PWMs[PWM_CHANNEL_NB];

extern float adc_utilities[3];

extern struct EEPROM_Header header;
extern struct EEPROM_hash hashs;

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
int board_init(void)
{
	int size = 0;

    // Initialize all PWMs
    init_pwm(LED_MUTE_0, GPIOA, GPIO_PIN_15, 25, 1); // Led mute 0
    init_pwm(LED_MUTE_1, GPIOA, GPIO_PIN_8,  25, 1); // Led mute 1
    init_pwm(LED_MUTE_2, GPIOB, GPIO_PIN_15, 25, 1); // Led mute 2
    init_pwm(LED_MUTE_3, GPIOB, GPIO_PIN_2,  25, 1); // Led mute 3
    init_pwm(LED_MUTE_4, GPIOB, GPIO_PIN_1,  25, 1); // Led mute 4
    init_pwm(RGB_RED,    GPIOC, GPIO_PIN_13, 100, 1); // RED
    init_pwm(RGB_GREEN,  GPIOC, GPIO_PIN_14, 50, 1); // GREEN
    init_pwm(RGB_BLUE,   GPIOC, GPIO_PIN_15, 0, 1); // BLUE
    init_pwm(LED_DEBUG,  GPIOA, GPIO_PIN_5,  33, 1); // Debug

	// Init the TCA9548 I2C mux (select ALL channels).
    tca9548_init(&hi2c2);
    tca9548_select_mask(0xFF);

    // Then, search for all devices
    scan_i2c();

    // Initialize all of the screens
    SSD1306_Init(&hi2c2);
    eeprom_init(&hi2c2, EEPROM_ADDR, EEPROM_PAGE_SIZE, EEPROM_SIZE);

    // If needed, overwrite the header
    uint8_t buf[128] = { 0 };
    if(HAL_GPIO_ReadPin (GPIOF, GPIO_PIN_1))
    {
    	size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "[  INIT  ] Loading the config from the header...\n\r");
		uart_log(uart_message, size);
		memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));

		eeprom_read(HEADER_ADDR, buf, HEADER_LEN, HAL_MAX_DELAY);
		int ret = parse_raw_eeprom_header(buf);

		if (ret != 0)
		{
			size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "[  INIT  ] Can't found the header on the EEPROM. Writing a new one...\n\r");
			uart_log(uart_message, size);
			memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));

			goto overwrite; // Share the same overwrite code.
		}
    }
    else
    {
    	size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "[  INIT  ] Writting a new config to the EEPROM...\n\r");
    	uart_log(uart_message, size);
		memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));

overwrite:

		/*
		 * We define the default header to that value.
		 * This header must be configured to enable the correct analog performances.
		 *
		 * Otherwise, large offsets nor elements can be wrong.
		 */
		header.SN[0] = 20;
		header.SN[1] = 26;
		header.SN[2] = 01;
		header.SN[3] = 01;
		header.SN[4] = 'H';
		header.SN[5] = 'A';
		header.SN[6] = 'D';
		header.SN[7] = 0;

		header.offset = 0.000f;
		header.gain = (uint8_t)25;

		header.chan_gain[0] = 1.000f;
		header.chan_gain[1] = 1.000f;
		header.chan_gain[2] = 1.000f;
		header.chan_gain[3] = 1.000f;
		header.chan_gain[4] = 1.000f;

		header.chan_offsets[0] = 0.000f;
		header.chan_offsets[1] = 0.000f;
		header.chan_offsets[2] = 0.000f;
		header.chan_offsets[3] = 0.000f;
		header.chan_offsets[4] = 0.000f;

		build_raw_eeprom_header(buf);
		eeprom_write(HEADER_ADDR, buf, HEADER_LEN, HAL_MAX_DELAY);
    }

    // Add a print to show the loaded config :
    show_loaded_header();

    // Finally, launch the timers
    timer_launch();

    // Wait for interrupts to happen, and ensure all voltages are normal
    HAL_Delay(150);
    check_supplies();

    // Set leds to pale green (OK)
    set_pwm(RGB_RED, 56/3, 1); // RED
    set_pwm(RGB_GREEN, 80/3, 1); // GREEN
    set_pwm(RGB_BLUE, 20/3, 1); // BLUE

    // Launch the first
    tsc_configure_next_channel();

	// Finally, init the fsm for the device
	fsm_init();

    return 0;
}

int scan_i2c(void)
{
    int ret;
    int dev_found = 0;
    int size = 0;

    for(int i=1; i<128; i++)
    {
	    ret = HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(i<<1), 3, 5);
	    if (ret == HAL_OK)
	    {
	    	dev_found += 1;
	    }
    }


    // Return 1 if a device was not found.
    if (dev_found != 3)
    {
    	size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "[  INIT  ] Some I2C devices are missing. Cannot continue...\n\r");
    	uart_log(uart_message, size);
	    memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));
	    Error_Handler();
    }

    size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "[  INIT  ] Found the required devices (0x50, 0x70, 0x3C)\n\r");
    uart_log(uart_message, size);
    memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));
    return 0;
}

int check_supplies(void)
{
	int size = 0;
	if (((3300 - 500) > adc_utilities[0]) | (adc_utilities[0] > (3300 + 500)))
	    {
	    	size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "[  INIT  ] 3.3V digital rail outside norms (3.3V +- 500mV) : %4d mV\n\r", (int)adc_utilities[0]);
	    	uart_log(uart_message, size);
			memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));
			Error_Handler();
	    }
	    if (((5000 - 500) > adc_utilities[1]) | (adc_utilities[1] > (5000 + 500)))
		{
			size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "[  INIT  ] 5V USB rail outside norms (5V +- 500mV) : %4d mV\n\r", (int)adc_utilities[1]);
			uart_log(uart_message, size);
			memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));
			Error_Handler();
		}
	    if (((3300 - 500) > adc_utilities[2]) | (adc_utilities[2] > (3300 + 500)))
		{
			size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "[  INIT  ] 3.3V analog rail outside norms (3.3V +- 500mV) : %4d mVrr\n\r", (int)adc_utilities[2]);
			uart_log(uart_message, size);
			memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));
			Error_Handler();
		}

	    size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "[  INIT  ] Power supply rails are within tolerances !\n\r");
	    uart_log(uart_message, size);
		memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));
		return 0;
}

int show_loaded_header(void)
{
	int size = 0;

	// Initial message
	size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "[  INIT  ] The header was loaded : \n\r");
	uart_log(uart_message, size);
	memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));

	// Serial number
	size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "           SN :       %c%c %c %c %c%c %c%c \n\r",
			header.SN[0],
			header.SN[1],
			header.SN[2],
			header.SN[3],
			header.SN[4],
			header.SN[5],
			header.SN[6],
			header.SN[7]
	);
	uart_log(uart_message, size);
	memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));

	// Offsets
	size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "           Offsets : %4d | %4d %4d %4d %4d %4d\n\r",
			(int)(header.offset * 1000),
			(int)(header.chan_offsets[0] * 1000),
			(int)(header.chan_offsets[1] * 1000),
			(int)(header.chan_offsets[2] * 1000),
			(int)(header.chan_offsets[3] * 1000),
			(int)(header.chan_offsets[4] * 1000)
	);
	uart_log(uart_message, size);
	memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));

	// Gains
	size = snprintf((char *)uart_message, (size_t)sizeof(uart_message), "           Gains :   %4d | %4d %4d %4d %4d %4d\n\r",
			(int)(header.gain),
			(int)(header.chan_gain[0] * 1000),
			(int)(header.chan_gain[1] * 1000),
			(int)(header.chan_gain[2] * 1000),
			(int)(header.chan_gain[3] * 1000),
			(int)(header.chan_gain[4] * 1000)
	);
	uart_log(uart_message, size);
	memset((void *)uart_message, 0x00, (size_t)sizeof(uart_message));

	return 0;
}


