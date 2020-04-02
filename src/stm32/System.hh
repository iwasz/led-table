/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "stm32/usb/UsbHidController.hh"

namespace le::system {
void init ();

inline auto &getUsb ()
{
        static le::usb::UsbHidController usb;
        return usb;
}

struct FakeWindow {
        static constexpr bool isOpen () { return true; }
};

inline FakeWindow &getWindow ()
{
        static FakeWindow i{};
        return i;
}

} // namespace le::system