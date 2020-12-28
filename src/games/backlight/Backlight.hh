/*-**************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for detailDirections.                        *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "Buttons.h"
#include "GenericGraphics.h"
#include "Geometry.h"
#include "IGame.h"
#include "Timer.h"
#include <array>
#include <gsl/gsl>
#include <list>

namespace le::backlight {

/**
 *
 */
template <typename G, typename B> class Game : public IGame {
public:
        static constexpr int WIDTH = G::WIDTH;
        static constexpr int HEIGHT = G::HEIGHT;
        static constexpr int INITIAL_LEN = 3;

        explicit Game (G &graphics, B const &buttons);

        void run () override;
        int getScore () const override { return 0; }
        void reset () override {}

private:
        // using Board = std::array<bool, HEIGHT * WIDTH>;
        // void turn (Board &oldBoard, Board &newBoard);
        // void printBoard (Board &board);

private:
        static constexpr int LOOP_FREQ{100};

        G &graphics;
        B const &buttons;
        Timer timer;

        // Board b1;
        // Board b2;
        // Board *board{&b1};
        bool runnuing{true};
        uint8_t comp = 0x80;
        // Color color{comp, comp, comp};
};

/****************************************************************************/

template <typename G, typename B> Game<G, B>::Game (G &graphics, B const &buttons) : graphics (graphics), buttons (buttons) { reset (); }

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::run ()
{
        if (!timer.isExpired () || !runnuing) {
                return;
        }

        if (auto pressed = buttons.getButton (Button::I | Button::K); pressed) {

                if (pressed == Button::I) {
                        comp += 0x10;
                }
                else {
                        comp -= 0x10;
                }

                // comp %= 256;
        }

        // graphics.clear ();

        for (int i = 0; i < HEIGHT; ++i) {
                for (int j = 0; j < WIDTH; ++j) {
                        graphics.set ({j, i}, Color{comp, comp, comp});
                }
        }

        timer.start (LOOP_FREQ);
}

/****************************************************************************/

// template <typename G, typename B> void Game<G, B>::turn (Board &oldBoard, Board &newBoard)
// {
//         for (unsigned i = 0; i < HEIGHT; i++) {
//                 for (unsigned j = 0; j < WIDTH; j++) {

//                         /* Find how many live neighbours this cell has. Note that we only use a finite board. */
//                         unsigned neighbours = 0;
//                         if (i > 0 && j > 0) neighbours += oldBoard[(i - 1) * WIDTH + j - 1];
//                         if (i > 0) neighbours += oldBoard[(i - 1) * WIDTH + j];
//                         if (i > 0 && j < WIDTH - 1) neighbours += oldBoard[(i - 1) * WIDTH + j + 1];
//                         if (j > 0) neighbours += oldBoard[i * WIDTH + j - 1];
//                         if (j < WIDTH - 1) neighbours += oldBoard[i * WIDTH + j + 1];
//                         if (i < HEIGHT - 1 && j > 0) neighbours += oldBoard[(i + 1) * WIDTH + j - 1];
//                         if (i < HEIGHT - 1) neighbours += oldBoard[(i + 1) * WIDTH + j];
//                         if (i < HEIGHT - 1 && j < WIDTH - 1) neighbours += oldBoard[(i + 1) * WIDTH + j + 1];

//                         /* Now the rules of the game itself. */
//                         if (oldBoard[i * WIDTH + j]) {
//                                 switch (neighbours) {
//                                 case 0 ... 1:
//                                 case 4 ... 8:
//                                         newBoard[i * WIDTH + j] = false;
//                                         break;
//                                 case 2 ... 3:
//                                         newBoard[i * WIDTH + j] = true;
//                                         break;
//                                 }
//                         }
//                         else if (neighbours == 3) {
//                                 newBoard[i * WIDTH + j] = true;
//                         }
//                         else {
//                                 newBoard[i * WIDTH + j] = false;
//                         }
//                 }
//         }
// }

/****************************************************************************/

// template <typename G, typename B> void Game<G, B>::printBoard (Board &board) {}

} // namespace le::backlight
