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
#include "games/conway/Conway.hh"

extern "C" {
#include "gifdec.h"
}

// extern "C" unsigned char goblin1_gif[];
// extern "C" unsigned int goblin1_gif_len;

extern "C" unsigned char cat_gif[];
extern "C" unsigned int cat_gif_len;

extern "C" unsigned char formula1_gif[];
extern "C" unsigned int formula1_gif_len;

extern "C" unsigned char bot_gif[];
extern "C" unsigned int bot_gif_len;

namespace le::gif {
template <typename Fb> class Gif {
public:
        explicit Gif (Fb &fb) : frameBuffer{fb} {}
        ~Gif () { gd_close_gif (gif); }
        void load (gsl::span<uint8_t> data);
        void run ();

private:
        Fb &frameBuffer;
        // std::vector<uint8_t> frame;
        gd_GIF *gif{};
        Timer timer{};
};

/****************************************************************************/

template <typename Fb> void Gif<Fb>::load (gsl::span<uint8_t> data)
{
        gif = gd_open_gif (data.data (), data.size ());

        if (!gif) {
                printf ("Error during opening a GIF\r\n");
                return;
        }

        if (gif->width != 32 || gif->height != 32) {
                printf ("Only 32x32 gifs\r\n");
                return;
        }
}

/****************************************************************************/

template <typename Fb> void Gif<Fb>::run ()
{
        if (!timer.isExpired ()) {
                return;
        }

        if (gif == nullptr) {
                return;
        }

        auto ret = gd_get_frame (gif);

        if (ret < 0) {
                printf ("GIT get frame error\r\n");
                return;
        }

        timer.start (gif->gce.delay * 10);

        gd_render_frame (gif, frameBuffer.getData ());

        if (ret == 0) {
                gd_rewind (gif);
        }
}

} // namespace le::gif

int main ()
{
        using namespace le;
        using namespace le::system;

        system::init ();
        log::init ();
        printf ("Led table is alive\r\n");

        // tetrisConstructor ();
        // le::snake::Game game{getGraphics (), getButtonQueue ()};
        // le::sokoban::Game game{getGraphics (), getButtonQueue ()};
        le::conway::Game game{getGraphics (), getButtonQueue ()};

        // le::gif::Gif game{getFrameBuffer ()};
        // game.load ({cat_gif, cat_gif_len});
        // game.load ({goblin1_gif, goblin1_gif_len});
        // game.load ({formula1_gif, formula1_gif_len});

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
