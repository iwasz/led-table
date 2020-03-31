/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "Hal.h"
#include "usbh_conf.h"
#include "usbh_core.h"
#include "usbh_def.h"
#include "usbh_hid.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_parser.h"

namespace le::usb {

enum class Type {
        MOUSE = 0x01,
        KEYBOARD = 0x02,
        GAMEPAD = 0x03,
};

/**
 *
 */
class UsbHidController {
public:
        UsbHidController ();
        void run ();

        HID_MOUSE_Info_TypeDef *getMouseInfo () const { return USBH_HID_GetMouseInfo (&hUSBHost); }
        HID_KEYBD_Info_TypeDef *getKeyboardInfo () const { return USBH_HID_GetKeybdInfo (&hUSBHost); }

        /**
         * Returns info based on current HID report. This is tricky, because
         * reports arrive all the time, and even if the button is pressed
         * for a period of time, only one report in a few has an information
         * about it.
         */
        HidGamepadInfo *getGamepadInfo () const { return usbhHidGetGamepadInfo (&hUSBHost); }

        Type getType () const;
        bool isReady () const;

        // private:
        //         void processMouse ();
        //         void processKeyboard ();
        //         void processGamepad ();

private:
        mutable USBH_HandleTypeDef hUSBHost{};
};

} // namespace le::usb