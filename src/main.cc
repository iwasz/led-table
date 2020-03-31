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

#ifdef WITH_EMULATOR
        auto [winW, winH] = getWindow ().getSize ();
        auto pixW = winW / float (FrameBufferType::WIDTH);
        auto pixH = winH / float (FrameBufferType::HEIGHT);
#else
        int pixW = 0;
        int pixH = 0;
#endif
        // Timer timer{};

        // tetrisConstructor ();
        le::snake::Game snake{getGraphics (), getButtonQueue ()};

        bool running = true;
        while (running) {
#ifdef WITH_EMULATOR
                sf::Event event{};

                while (getWindow ().pollEvent (event)) {
                        if (event.type == sf::Event::Closed) {
                                getWindow ().close ();
                                running = false;
                                break;
                        }

                        if (event.type == sf::Event::KeyPressed) {
                                getButtonQueue ().onPress ();
                                // getButtons ().onPress ();
                                // getSingleButton ().onPress ();
                        }
                        else if (event.type == sf::Event::KeyReleased) {
                                getButtonQueue ().onRelease ();
                                // getButtons ().onRelease ();
                                // getSingleButton ().onRelease ();
                        }
                }
#else
                getUsb ().run ();
                getButtonQueue ().run ();
                // printButtons (getButtonQueue ().getButtons ());

                // auto b = getButtonFromUsb (getUsb ());

                // if (b) {
                //         printButtons (*b);
                // }
#endif

                // if (!timer.isExpired ()) {
                //         continue;
                // }

                // timer.start (FRAME_DURATION_MS);

#ifdef WITH_EMULATOR
                getWindow ().clear ();
#endif

                snake.run ();
                le::fb::display (getFrameBuffer (), getWindow (), {pixW, pixH});

#ifdef WITH_EMULATOR
                getWindow ().display ();
#endif
        }

        return 0;
}
