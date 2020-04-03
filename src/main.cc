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

#include "games/snake/Snake.h"
#include "games/sokoban/Sokoban.h"
// #include "tinytetris/tinytetris.h"

int main ()
{
        using namespace le;
        using namespace le::system;

        system::init ();
        log::init ();
        printf ("Led table is alive\r\n");

        // tetrisConstructor ();
        // le::snake::Game game{getGraphics (), getButtonQueue ()};
        le::sokoban::Game game{getGraphics (), getButtonQueue ()};

        while (getWindow ().isOpen ()) {
#ifdef WITH_FIRMWARE
                getUsb ().run ();
#endif
                getButtonQueue ().run ();

#ifdef WITH_EMULATOR
                getWindow ().clear ();
#endif

                game.run ();
                getFrameBuffer ().display ();
        }

        return 0;
}
