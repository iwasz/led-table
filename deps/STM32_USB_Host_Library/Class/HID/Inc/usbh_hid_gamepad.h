/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "usbh_hid.h"

typedef struct {
        // uint8_t state;
        // uint8_t lctrl;
        // uint8_t lshift;
        // uint8_t lalt;
        // uint8_t lgui;
        // uint8_t rctrl;
        // uint8_t rshift;
        // uint8_t ralt;
        // uint8_t rgui;
        // uint8_t keys[8];
        uint32_t a;
        uint32_t b;
} HidGamepadInfo;

USBH_StatusTypeDef usbhHidGamepadInit (USBH_HandleTypeDef *phost);
HidGamepadInfo *usbhHidGetGamepadInfo (USBH_HandleTypeDef *phost);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
