/** ================================================================
 * @file    handlers/leds.h
 *
 * @brief   Define leds related functions, to configure the big-
 * 	    	banged PWM.
 *
 * @date 	13/11/2025
 *
 * @version 1.0.0
 *
 * @author  l.heywang (leonard.heywang@proton.me)
 *
 *  ================================================================
 */
#pragma once

/*
 * -----------------------------------------------------------------
 * Includes
 * -----------------------------------------------------------------
 */
#include "stm32f3xx_hal.h"

/*
 * -----------------------------------------------------------------
 * Defines
 * -----------------------------------------------------------------
 */
// PWM Parameters
#define PWM_MAX_COUNT 	100
#define PWM_CHANNEL_NB 	9

// On status isn't full power
#define PWM_LED_ON		100

/*
 * -----------------------------------------------------------------
 * Types
 * -----------------------------------------------------------------
 */
typedef struct GPIO_PWM {
	// private values
	uint8_t _threshold;
	uint8_t _value;
	GPIO_TypeDef* _GPIO_BASE;
	uint16_t _GPIO_PIN;
} GPIO_PWMx;

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
// Store all the PWM as a fixed array, to not use malloc().
extern GPIO_PWMx PWMs[PWM_CHANNEL_NB];

// Defining some aliases
#define LED_MUTE_0 	&PWMs[0]
#define LED_MUTE_1  &PWMs[1]
#define LED_MUTE_2  &PWMs[2]
#define LED_MUTE_3  &PWMs[3]
#define LED_MUTE_4  &PWMs[4]

#define RGB_RED 	&PWMs[5]
#define RGB_GREEN 	&PWMs[6]
#define RGB_BLUE 	&PWMs[7]

#define LED_DEBUG 	&PWMs[8]

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
/**
 * @brief Initialize the PWM engines.
 *
 * @param GPIO_BASE 	GPIO port target of the PWM
 * @param GPIO_PIN 		GPIO pin of the target PWM
 * @param duty 			Duty cycles (0 | 100)
 * @param enable		Enable this PWM (otherwise, set to '0').
 */
int init_pwm(GPIO_PWMx *handle, GPIO_TypeDef* GPIO_BASE, uint16_t GPIO_PIN, uint8_t duty, uint8_t enable);

/**
 * @brief Change the PWM value, based on some config
 *
 * @param duty 			Duty cycles (0 | 100)
 * @param enable		Enable this PWM (otherwise, set to '0').
 */
int set_pwm(GPIO_PWMx *handle, uint8_t duty, uint8_t enable);

/**
 * @brief Trigger the PWM mechanism.
 */
void __update_PWM(GPIO_PWMx *handle);

