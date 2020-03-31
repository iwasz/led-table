/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "Buttons.h"
#include "FrameBuffer.h"
#include "GenericGraphics.h"
#include "Ncurses.h"
#include "System.hh"

namespace le {
using FrameBufferType = le::fb::FrameBuffer<32, 32>;
using NcursesStateType = le::nc::NcursesState<FrameBufferType>;

auto &getFrameBuffer ()
{
        static FrameBufferType fb;
        return fb;
}

auto &getNcurses ()
{
        static NcursesStateType ncurses (getFrameBuffer ());
        return ncurses;
}

auto &getGraphics ()
{
        static le::GenericGraphics graphics{getFrameBuffer ()};
        return graphics;
}

auto &getButtons ()
{
        static le::Buttons buttons;
        return buttons;
}

auto &getSingleButton ()
{
        static le::SingleButton singleButton;
        return singleButton;
}

auto &getButtonQueue ()
{
#ifdef WITH_EMULATOR
        static le::ButtonQueue bq;
#else
        static le::ButtonQueue bq (system::getUsb ());
#endif
        return bq;
}

} // namespace le