/*-**************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for detailDirections.                        *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "Board.h"
#include "Buttons.h"
#include "GenericGraphics.h"
#include "Geometry.h"
#include "IGame.h"
#include "Timer.h"
#include <gsl/gsl>
#include <vector>

namespace le::sokoban {

/**
 * Game class.
 */
template <typename G, typename B> class Game : public IGame {
public:
        static constexpr int WIDTH = G::WIDTH;
        static constexpr int HEIGHT = G::HEIGHT;

        explicit Game (G &graphics, B const &buttons);

        void run () override;
        int getScore () const override { return 0; }

        void reset () override {}

private:
        void gameOver () { reset (); }
        void draw () const;
        void move (Button b);

private:
        G &graphics;
        B const &buttons;
        Timer timer;
        bool runnuing{true};
        Board board;
        size_t currentLevel{};
};

/****************************************************************************/

template <typename G, typename B> Game<G, B>::Game (G &graphics, B const &buttons) : graphics (graphics), buttons (buttons)
{
        board.load (levels.at (currentLevel));
}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::run ()
{
        if (!timer.isExpired () || !runnuing) {
                return;
        }

        graphics.clear ();
        draw ();

        if (auto pressed = buttons.getButton (); pressed) {
                move (*pressed);
        }

        timer.start (50); // 20 FPS
}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::draw () const
{
        Point off{std::max (0, (WIDTH - board.getWidth ()) / 2), std::max (0, (HEIGHT - board.getHeight ()) / 2)};

        Point p{};
        for (Element e : board.getElements ()) {
                if ((e & Element::wall) == Element::wall) {
                        graphics.set (p + off, DARK_GRAY);
                }

                if ((e & Element::goal) == Element::goal) {
                        graphics.set (p + off, LIGHT_GREEN);
                }

                if ((e & Element::crate) == Element::crate) {
                        if ((e & Element::goal) == Element::goal) {
                                graphics.set (p + off, ORANGE);
                        }
                        else {
                                graphics.set (p + off, BROWN);
                        }
                }

                Element onPlayer = board.get (board.getPlayer ());
                if (onPlayer != Element::empty) {
                        graphics.set (board.getPlayer () + off, PINK);
                }
                else {
                        graphics.set (board.getPlayer () + off, RED);
                }

                if (++p.x >= board.getWidth ()) {
                        ++p.y;
                        p.x = 0;
                }
        }
}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::move (Button b)
{
        switch (b) {
        case Button::I:
                board.movePlayer (Heading::UP);
                break;

        case Button::K:
                board.movePlayer (Heading::DOWN);
                break;

        case Button::J:
                board.movePlayer (Heading::LEFT);
                break;

        case Button::L:
                board.movePlayer (Heading::RIGHT);
                break;

        default:
                break;
        }
}

} // namespace le::sokoban
