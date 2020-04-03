/*-**************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for detailDirections.                        *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Board.h"
#include <cstring>
#include <gsl/gsl>

namespace le::sokoban {

void Board::movePlayer (Heading h)
{
        Point const &dest = destPoint (player, h);

        if (get (dest) == Element::wall) {
                return;
        }

        if ((get (dest) & Element::crate) == Element::crate) {
                if (!moveCrate (dest, h)) {
                        return;
                }
        }

        player = dest;
}

/****************************************************************************/

bool Board::moveCrate (Point const &crate, Heading h)
{
        Point dest = destPoint (crate, h);

        if (get (dest) == Element::wall) {
                return false;
        }

        clear (crate, Element::crate);
        set (dest, Element::crate);
        return true;
}

/****************************************************************************/

Point Board::destPoint (Point const &p, Heading h) const
{
        switch (h) {
        case Heading::DOWN:
                return p + Point{0, 1};

        case Heading::UP:
                return p - Point{0, 1};

        case Heading::LEFT:
                return p - Point{1, 0};

        case Heading::RIGHT:
                return p + Point{1, 0};
        }

        return p;
}

/****************************************************************************/

void Board::load (Level const &l)
{
        width = l.width;
        height = strlen (l.data) / width;
        elements.clear ();
        elements.resize (width * height);

        for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                        switch (*(l.data + y * width + x)) {
                        case '#':
                                set ({x, y}, Element::wall);
                                break;

                        case '@':
                                player = {x, y};
                                break;

                        case '$':
                                set ({x, y}, Element::crate);
                                break;

                        case '.':
                                set ({x, y}, Element::goal);
                                break;

                        case '+':
                                player = {x, y};
                                set ({x, y}, Element::goal);
                                break;

                        case '*':
                                set ({x, y}, Element::crate);
                                set ({x, y}, Element::goal);
                                break;

                        default:
                                break;
                        }
                }
        }
}

} // namespace le::sokoban