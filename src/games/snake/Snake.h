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
#include <gsl/gsl>
#include <list>
#include <vector>

namespace le::snake {

enum class Heading { RIGHT, LEFT, UP, DOWN };

/**
 * A snake class.
 */
struct Body {
        explicit Body (std::list<Point> p) : points (std::move (p)) {}
        explicit Body (Point const &entryPoint, int l) : length{l}, points{{entryPoint.x + length, entryPoint.y}, entryPoint} {}

        void advanceSnake ();
        void bend (Heading h);
        void lenghten ();

        Heading headDirection{Heading::RIGHT};
        Heading tailDirection{Heading::RIGHT};
        int length{};
        /// Start, end and every corner of the snake.
        std::list<Point> points{};
};

/// Move a point H or V by 1.
extern void advancePoint (Point &p, Heading h);

/// What is the direction when going from A to B (only H and V are possible).
extern Heading getDirection (Point const &a, Point const &b);

/// Detect button presses
extern std::optional<Heading> buttonsToHeading (le::Button b);

/// For UP returns DOWN etc.
extern Heading oppositeHeading (Heading h);

/**
 * Game class.
 */
template <typename G, typename B> class Game : public IGame {
public:
        static constexpr int WIDTH = G::WIDTH;
        static constexpr int HEIGHT = G::HEIGHT;
        static constexpr int INITIAL_LEN = 3;

        explicit Game (G &graphics, B const &buttons);

        void run () override;
        int getScore () const override { return body.length - INITIAL_LEN; }

        void reset () override
        {
                body = Body{ENTRY_POINT, INITIAL_LEN};
                apples.clear ();
        }

private:
        void generateApplesIfNecessary ();
        void manageApples ();
        void drawWalls ();
        void drawSnake ();
        void drawApples ();

        void gameOver ()
        {
                // fmt::print ("GAME OVER\n");
                // runnuing = false;
                reset ();
        }

private:
        static constexpr int LOOP_FREQ{100};
        static constexpr Point ENTRY_POINT{0, HEIGHT / 2};

        Body body{ENTRY_POINT, INITIAL_LEN};
        G &graphics;
        B const &buttons;
        Timer timer;
        std::vector<Point> apples;
        bool runnuing{true};
};

/****************************************************************************/

template <typename G, typename B> Game<G, B>::Game (G &graphics, B const &buttons) : graphics (graphics), buttons (buttons)
{
        // auto screenDimension = graphics.getScreenDimension ();
        generateApplesIfNecessary ();
}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::run ()
{
        if (!timer.isExpired () || !runnuing) {
                return;
        }

        graphics.clear ();
        generateApplesIfNecessary ();
        // manageApples ();
        drawWalls ();
        drawSnake ();
        drawApples ();

        if (auto pressed = buttons.getButton (); pressed) {
                if (auto heading = buttonsToHeading (*pressed); heading) {
                        if (*heading == oppositeHeading (body.headDirection)) {
                                gameOver ();
                                return;
                        }

                        body.bend (*heading);
                }
        }

        body.advanceSnake ();
        Point &head = body.points.front ();

        // Collision with apples
        if (auto i = std::find (apples.cbegin (), apples.cend (), head); i != apples.end ()) {
                // static int j = 0;
                // fmt::print ("Score : {}\n", ++j);
                body.lenghten ();
                apples.erase (i);
        }

        // Collision with walls (screen boundarties)
        if (head.x < 0 || head.x >= G::WIDTH || head.y < 0 || head.y >= G::HEIGHT) {
                gameOver ();
                return;
        }

        // Collision with itself
        bool firstSkipped = false;
        pathForEach (body.points, [this, head, &firstSkipped] (auto const &a, auto const &b) {
                if (!firstSkipped) {
                        firstSkipped = true;
                        return true;
                }

                if (intersects (Line{a, b}, head)) {
                        gameOver ();
                        return false;
                }

                return true;
        });

        timer.start (LOOP_FREQ);
}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::generateApplesIfNecessary ()
{
        int numerApplesNeeded = body.length / 3;

        while (int (apples.size ()) < numerApplesNeeded) {
                Point newApple = {std::rand () % WIDTH, std::rand () % HEIGHT};

                bool appleOnTheBody = false;
                pathForEach (body.points, [this, newApple, &appleOnTheBody] (auto const &a, auto const &b) {
                        if (intersects (Line{a, b}, newApple)) {
                                appleOnTheBody = true;
                        }

                        return true;
                });

                if (!appleOnTheBody) {
                        apples.push_back (newApple);
                }
        }
}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::manageApples () {}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::drawWalls () {}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::drawSnake ()
{
        graphics.setColor ({0xff, 0xff, 0xff});
        graphics.path (body.points);
}

/****************************************************************************/

template <typename G, typename B> void Game<G, B>::drawApples ()
{
        for (auto &a : apples) {
                graphics.set (a, {0xff, 0, 0});
        }
}

} // namespace le::snake
