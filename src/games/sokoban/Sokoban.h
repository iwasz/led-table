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

        void reset () override { board.load (levels.at (currentLevel)); }

private:
        void gameOver () { reset (); }
        void draw () const;
        void move (Button b);
        void nextLevel ();
        void prevLevel ();

private:
        G &graphics;
        B const &buttons;
        Timer timer;
        bool runnuing{true};
        Board board;
        int currentLevel{};
};

/****************************************************************************/

template <typename G, typename B> Game<G, B>::Game (G &graphics, B const &buttons) : graphics (graphics), buttons (buttons)
{
        reset ();
        graphics.clear ();
        draw ();
}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::run ()
{
        if (!timer.isExpired () || !runnuing) {
                return;
        }

        if (auto pressed = buttons.getButton (); pressed) {
                move (*pressed);
                graphics.clear ();
                draw ();
        }

        timer.start (50); // 20 FPS
}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::draw () const
{
        Point off{std::max (0, (WIDTH - board.getWidth ()) / 2), std::max (0, (HEIGHT - board.getHeight ()) / 2)};
        Point p{};
        bool success = board.checkSuccess ();

        for (Element e : board.getElements ()) {
                if ((e & Element::wall) == Element::wall) {
                        graphics.set (p + off, (success) ? (LIGHT_BLUE) : (DARK_GRAY));
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

        case Button::Q:
                prevLevel ();
                break;

        case Button::O:
                nextLevel ();
                break;

        case Button::X:
                reset ();
                break;

        default:
                break;
        }
}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::nextLevel ()
{
        if (++currentLevel >= int (levels.size ()) - 1) {
                currentLevel = 0;
        }

        reset ();
}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::prevLevel ()
{
        if (--currentLevel < 0) {
                currentLevel = int (levels.size ()) - 1;
        }

        reset ();
}

} // namespace le::sokoban
