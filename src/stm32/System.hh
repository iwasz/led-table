/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "stm32/usb/Usb.hh"

namespace le::system {
void init ();

inline auto &getUsb ()
{
        static le::usb::Usb usb;
        return usb;
}

} // namespace le::system