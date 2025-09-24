/** ================================================================
 * @file    utils/CDC_helper.h
 *
 * @brief   Header for some utilities that are nice to have for the
 * 			CDC VCOM module
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
// STD
#include <stdint.h>

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
/**
 * Split long transmissions into smaller, 64 bytes call that are compliants with the
 * CDC USB Spec.
 */
void CDC_Transmit_Long(uint8_t *buf, uint16_t len);
