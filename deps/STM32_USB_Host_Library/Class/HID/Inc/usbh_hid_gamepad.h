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

// Button 0
#define BUTTON_X 0x0010
// Button 1
#define BUTTON_A 0x0020
// Button 2
#define BUTTON_B 0x0040
// Button 3
#define BUTTON_Y 0x0080
// Button 4
#define BUTTON_L 0x0100
// Button 5
#define BUTTON_R 0x0200
// Button 8
#define BUTTON_SELECT 0x0400
// Button 9
#define BUTTON_START 0x0800

typedef struct {
        int16_t x; // TODO int8_t ?
        int16_t y;
        uint16_t buttons;
} HidGamepadInfo;

USBH_StatusTypeDef usbhHidGamepadInit (USBH_HandleTypeDef *phost);
HidGamepadInfo *usbhHidGetGamepadInfo (USBH_HandleTypeDef *phost);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
