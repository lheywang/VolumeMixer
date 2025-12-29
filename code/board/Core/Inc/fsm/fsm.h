/** ================================================================
 * @file    fsm/fsm.h
 *
 * @brief   Source file of the state machine that control the whole
 * 			device
 *
 * @date 	24-09-2025
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
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "config.h"

/*
 * -----------------------------------------------------------------
 * Enums
 * -----------------------------------------------------------------
 */
enum fsm_state {
	BOARD_INIT,
	WAIT_FOR_CONNECT,
	WAIT_FOR_COMMAND,
	WAIT_FOR_RESPONSE
};

/*
 * -----------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------
 */
extern uint8_t RxBuffer[CDC_BUFFER_SIZE];
extern uint8_t TxBuffer[CDC_BUFFER_SIZE];
extern volatile uint16_t rx_len;
extern volatile bool 	  msg_complete;

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
/**
 * Initialize the FSM of the system to it's default state.
 */
void fsm_init();

/*
 * Trigger an update of the FSM, with the new elements known.
 */
void fsm_update();
