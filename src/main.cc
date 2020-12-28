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
#include "games/backlight/Backlight.hh"
#include "games/conway/Conway.hh"

// extern "C" {
// #include "gifdec.h"
// }

#include <AnimatedGIF.h>

// extern "C" unsigned char goblin1_gif[];
// extern "C" unsigned int goblin1_gif_len;

extern "C" unsigned char cat_gif[];
extern "C" unsigned int cat_gif_len;

extern "C" unsigned char formula1_gif[];
extern "C" unsigned int formula1_gif_len;

// extern "C" unsigned char bot_gif[];
// extern "C" unsigned int bot_gif_len;

namespace le::gif {
template <typename Fb> class Gif : public le::IGame {
public:
        explicit Gif (Fb &fb) : frameBuffer{fb} {}
        ~Gif ()
        { /* gd_close_gif (gif); */
                gif.close ();
        }
        void load (gsl::span<uint8_t> data);
        void run ();

        int getScore () const override { return 0; }
        void reset () override {}

private:
        static void GIFDraw (GIFDRAW *pDraw);

        static constexpr int DISPLAY_WIDTH = 32;
        AnimatedGIF gif;
        Fb &frameBuffer;
        Timer timer{};
};

template <typename Fb> void Gif<Fb>::GIFDraw (GIFDRAW *pDraw)
{
        uint8_t *s;
        int x, y, iWidth;
        // static uint8_t ucPalette[256]; // thresholded palette

        // if (pDraw->y == 0) // first line, convert palette to 0/1
        // {
        //         for (x = 0; x < 256; x++) {
        //                 uint16_t usColor = pDraw->pPalette[x];
        //                 int gray = (usColor & 0xf800) >> 8; // red
        //                 gray += ((usColor & 0x7e0) >> 2);   // plus green*2
        //                 gray += ((usColor & 0x1f) << 3);    // plus blue
        //                 ucPalette[x] = (gray >> 9);         // 0->511 = 0, 512->1023 = 1
        //         }
        // }
        uint16_t *ucPalette = pDraw->pPalette;

        y = pDraw->iY + pDraw->y; // current line
        iWidth = pDraw->iWidth;
        if (iWidth > DISPLAY_WIDTH) iWidth = DISPLAY_WIDTH;

        s = pDraw->pPixels;
        if (pDraw->ucDisposalMethod == 2) // restore to background color
        {
                for (x = 0; x < iWidth; x++) {
                        if (s[x] == pDraw->ucTransparent) s[x] = pDraw->ucBackground;
                }
                pDraw->ucHasTransparency = 0;
        }
        // Apply the new pixels to the main image
        if (pDraw->ucHasTransparency) // if transparency used
        {
                uint8_t c, ucTransparent = pDraw->ucTransparent;
                int x;
                for (x = 0; x < iWidth; x++) {
                        c = *s++;
                        if (c != ucTransparent) {
                                getFrameBuffer ().set ({pDraw->iX + x, y}, le::fromBGR565 (ucPalette[c]));
                                // DrawPixel (pDraw->iX + x, y, ucPalette[c]);
                        }
                }
        }
        else {
                s = pDraw->pPixels;
                // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
                for (x = 0; x < pDraw->iWidth; x++) {
                        getFrameBuffer ().set ({pDraw->iX + x, y}, le::fromBGR565 (ucPalette[*s++]));
                        // DrawPixel (pDraw->iX + x, y, ucPalette[*s++]);
                }
        }
} /* GIFDraw() */

/****************************************************************************/

template <typename Fb> void Gif<Fb>::load (gsl::span<uint8_t> data)
{
        if (!gif.open (data.data (), data.size (), GIFDraw)) {
                printf ("Error opening a gif\r\n");
        }
}

/****************************************************************************/

template <typename Fb> void Gif<Fb>::run ()
{
        if (!timer.isExpired ()) {
                return;
        }

        int delayMilliseconds{};
        gif.playFrame (true, &delayMilliseconds);
        timer.start (delayMilliseconds);
}

} // namespace le::gif

template <typename G, typename... Rst> auto createGameImpl (size_t num, G &game, Rst &...ret)
{
        if (num == 0) {
                return game ();
        }

        if constexpr (sizeof...(ret) > 0) {
                return createGameImpl (num - 1, ret...);
        }
        else {
                return std::unique_ptr<le::IGame>{};
        }
}

template <typename Games> auto createGame (Games &&games, size_t num)
{
        return std::apply ([num] (auto &...game) { return createGameImpl (num, game...); }, games);
}

int main ()
{
        using namespace le;
        using namespace le::system;

        system::init ();
        log::init ();
        printf ("Led table is alive\r\n");

        // tetrisConstructor ();

        auto games = std::make_tuple (
                [] {
                        return std::unique_ptr<le::IGame>{new le::sokoban::Game{getGraphics (), getButtonQueue ()}};
                },
                [] {
                        return std::unique_ptr<le::IGame>{new le::snake::Game{getGraphics (), getButtonQueue ()}};
                },
                [] {
                        return std::unique_ptr<le::IGame>{new le::conway::Game{getGraphics (), getButtonQueue ()}};
                },
                [] {
                        return std::unique_ptr<le::IGame>{new le::backlight::Game{getGraphics (), getButtonQueue ()}};
                },
                [] {
                        auto gif = new le::gif::Gif{getFrameBuffer ()};
                        gif->load ({cat_gif, cat_gif_len});
                        return std::unique_ptr<le::IGame>{gif};
                }/* ,
                [] {
                        auto gif = new le::gif::Gif{getFrameBuffer ()};
                        gif->load ({formula1_gif, formula1_gif_len});
                        return std::unique_ptr<le::IGame>{gif};
                } */);

        int currentGame = 0;
        int gamesNum = std::tuple_size<decltype (games)>::value;
        auto game = createGame (games, currentGame);

        Timer mainMenuKeyTimer{50};

        while (getWindow ().isOpen ()) {
#ifdef WITH_FIRMWARE
                getUsb ().run ();
#endif
                getButtonQueue ().run ();

#ifdef WITH_EMULATOR
                getWindow ().clear ();
#endif

                game->run ();
                getFrameBuffer ().display ();

                if (mainMenuKeyTimer.isExpired ()) {

                        if (auto b = getButtonQueue ().getButton (Button::A | Button::D); b) {
                                if (b == Button::A) {
                                        --currentGame;

                                        if (currentGame < 0) {
                                                currentGame = gamesNum - 1;
                                        }

                                        game = createGame (games, currentGame);
                                }
                                else {
                                        ++currentGame;

                                        if (currentGame >= gamesNum) {
                                                currentGame = 0;
                                        }

                                        game = createGame (games, currentGame);
                                }
                        }

                        mainMenuKeyTimer.start (50);
                }
        }

        return 0;
}
