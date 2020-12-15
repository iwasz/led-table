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
template <typename Fb> class Gif : public le::IGame {
public:
        explicit Gif (Fb &fb) : frameBuffer{fb} {}
        ~Gif () { gd_close_gif (gif); }
        void load (gsl::span<uint8_t> data);
        void run ();

        int getScore () const override { return 0; }
        void reset () override {}

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
                        auto gif = new le::gif::Gif{getFrameBuffer ()};
                        gif->load ({cat_gif, cat_gif_len});
                        return std::unique_ptr<le::IGame>{gif};
                },
                [] {
                        auto gif = new le::gif::Gif{getFrameBuffer ()};
                        gif->load ({formula1_gif, formula1_gif_len});
                        return std::unique_ptr<le::IGame>{gif};
                });

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
