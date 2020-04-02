/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Container.h"
#include "Logging.hh"
#include "System.hh"
#include "Timer.h"
#include <cerrno>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "Snake.h"
// #include "tinytetris/tinytetris.h"

static constexpr int FRAME_DURATION_MS = 40;

/*****************************************************************************/

int main ()
{
        using namespace le;
        using namespace le::system;

        system::init ();
        log::init ();
        printf ("Led table is alive\r\n");

        // tetrisConstructor ();
        le::snake::Game snake{getGraphics (), getButtonQueue ()};

        while (getWindow ().isOpen ()) {
#ifdef WITH_FIRMWARE
                getUsb ().run ();
#endif
                getButtonQueue ().run ();

#ifdef WITH_EMULATOR
                getWindow ().clear ();
#endif

                snake.run ();
                getFrameBuffer ().display ();
        }

        return 0;
}
