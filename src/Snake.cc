/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Snake.h"

namespace le::snake {

void Body::advanceSnake ()
{
        Expects (!points.empty ());

        // advance the headDirection
        advancePoint (points.front (), headDirection);

        // advance the tailDirection
        advancePoint (points.back (), tailDirection);

        if (points.size () <= 2) {
                return;
        }

        auto last = points.end ();
        std::advance (last, -1);
        auto lastB1 = last;
        std::advance (lastB1, -1);
        auto lastB2 = lastB1;
        std::advance (lastB2, -1);

        // if tailDirection overlaps with a bend, change tailDirection
        if (*last == *lastB1) {
                points.erase (last);
                tailDirection = getDirection (*lastB1, *lastB2);
        }
}

/****************************************************************************/

void Body::bend (Heading h)
{
        points.push_front (points.front ());
        headDirection = h;
}

/****************************************************************************/

void Body::lenghten ()
{
        ++length;
        advancePoint (points.back (), oppositeHeading (tailDirection));
}

/****************************************************************************/

void advancePoint (Point &p, Heading h)
{
        switch (h) {
        case Heading::DOWN:
                ++p.y;
                break;

        case Heading::UP:
                --p.y;
                break;

        case Heading::RIGHT:
                ++p.x;
                break;

        case Heading::LEFT:
                --p.x;
                break;

        default:
                break;
        }
}

/****************************************************************************/

Heading getDirection (Point const &a, Point const &b)
{
        if (a.x < b.x) {
                return Heading::RIGHT;
        }

        if (a.x > b.x) {
                return Heading::LEFT;
        }

        if (a.y < b.y) {
                return Heading::DOWN;
        }

        return Heading::UP;
}

/****************************************************************************/

std::optional<Heading> buttonsToHeading (le::Button b)
{
        std::optional<Heading> heading{};

        switch (b) {
        case Button::W:
                heading = Heading::UP;
                break;

        case Button::S:
                heading = Heading::DOWN;
                break;

        case Button::A:
                heading = Heading::LEFT;
                break;

        case Button::D:
                heading = Heading::RIGHT;
                break;

        default:
                break;
        }

        return heading;
}

/****************************************************************************/

extern Heading oppositeHeading (Heading h)
{
        switch (h) {
        case Heading::DOWN:
                return Heading::UP;

        case Heading::UP:
                return Heading::DOWN;

        case Heading::RIGHT:
                return Heading::LEFT;

        case Heading::LEFT:
        default:
                break;
        }

        return Heading::RIGHT;
}

} // namespace le::snake