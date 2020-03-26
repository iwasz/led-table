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

class Usb {
public:
        Usb ();
        void run ();

private:
        void processMouse ();
        void processKeyboard ();

private:
        USBH_HandleTypeDef hUSBHost{};
};

} // namespace le::usb