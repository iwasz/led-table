/*-**************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for detailDirections.                        *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "Geometry.h"
#include "Levels.h"
#include <gsl/gsl>
#include <vector>

namespace le::sokoban {

enum class Heading { RIGHT, LEFT, UP, DOWN };
enum class Element {
        empty = 0x00,
        wall = 0x01,
        crate = 0x02,
        goal = 0x04,
};

inline Element operator| (Element l, Element r) { return static_cast<Element> (static_cast<unsigned> (l) | static_cast<unsigned> (r)); }
inline Element operator& (Element l, Element r) { return static_cast<Element> (static_cast<unsigned> (l) & static_cast<unsigned> (r)); }
inline Element operator^ (Element l, Element r) { return static_cast<Element> (static_cast<unsigned> (l) ^ static_cast<unsigned> (r)); }
inline Element operator~ (Element r) { return static_cast<Element> (~static_cast<unsigned> (r)); }
inline Element &operator|= (Element &l, Element r)
{
        l = l | r;
        return l;
}
inline Element &operator&= (Element &l, Element r)
{
        l = l & r;
        return l;
}

/**
 *
 */
class Board {
public:
        void load (Level const &l);
        void movePlayer (Heading h);
        bool moveCrate (Point const &crate, Heading h);
        Point destPoint (Point const &p, Heading h) const;

        Element get (Point const &p) const { return elements.at (p.y * width + p.x); }
        Element &get (Point const &p) { return elements.at (p.y * width + p.x); }
        Element &set (Point const &p, Element e)
        {
                auto &t = get (p);
                t |= e;
                return t;
        }

        Element &clear (Point const &p, Element e)
        {
                auto &t = get (p);
                t &= ~e;
                return t;
        }

        auto &getElements () { return elements; }
        auto const &getElements () const { return elements; }
        auto getWidth () const { return width; }
        auto getHeight () const { return height; }
        auto const &getPlayer () const { return player; }

private:
        Point player;
        std::vector<Element> elements;
        int width{};
        int height{};
};

} // namespace le::sokoban