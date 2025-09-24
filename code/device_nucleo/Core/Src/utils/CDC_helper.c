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
#include "utils/CDC_helper.h"

// STD
#include <stdint.h>

// HAL
#include "usbd_cdc_if.h"

/*
 * -----------------------------------------------------------------
 * Functions
 * -----------------------------------------------------------------
 */
void CDC_Transmit_Long(uint8_t *buf, uint16_t len)
{
    uint16_t sent = 0;

    while(sent < len)
    {
        uint16_t chunk = (len - sent > 64) ? 64 : (len - sent);

        // Wait until previous transmission finishes
        while(CDC_Transmit_FS(&buf[sent], chunk) != USBD_OK)
        {
            // Optionally: timeout or yield
        }

        sent += chunk;
    }
}
