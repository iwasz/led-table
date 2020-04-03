/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "Color.h"
#include "FrameBuffer.h"
#include "Geometry.h"
#include <gsl/gsl>

namespace le {

template <typename F> class GenericGraphics {
public:
        static constexpr int WIDTH = F::WIDTH;
        static constexpr int HEIGHT = F::HEIGHT;

        explicit GenericGraphics (F &f) : frameBuffer (f) {}

        void move (Point const &p) { cursor = p; }

        void set (Point const &p, Color const &c) { frameBuffer.set (p, c); }
        void set (Point const &p) { frameBuffer.set (p, currentColor); }
        void set () { frameBuffer.set (cursor, currentColor); }

        auto getScreenDimension () const { return screenDimension; }
        void setColor (Color const &c) { currentColor = c; }

        void line (Point const &a, Point const &b);
        template <typename T> void path (T const &pointCollection);

        void clear () { frameBuffer.clear (); }

private:
        F &frameBuffer;
        Point cursor{}; /// 0,0 is in the top left corner
        Color currentColor{};
        DimensionT<int> screenDimension{F::HEIGHT, F::WIDTH};
};

/****************************************************************************/

template <typename F> template <typename T> void GenericGraphics<F>::path (T const &pointCollection)
{
        pathForEach (pointCollection, [this] (auto const &p1, auto const &p2) {
                line (p1, p2);
                return true;
        });
}

/****************************************************************************/

template <typename F> void GenericGraphics<F>::line (Point const &a, Point const &b)
{
        int minX, maxX, minY, maxY;

        if (a.x < b.x) {
                minX = a.x;
                maxX = b.x;
        }
        else {
                minX = b.x;
                maxX = a.x;
        }

        if (a.y < b.y) {
                minY = a.y;
                maxY = b.y;
        }
        else {
                minY = b.y;
                maxY = a.y;
        }

        Expects (minX < WIDTH);
        Expects (maxX < WIDTH);
        Expects (minY < HEIGHT);
        Expects (maxY < HEIGHT);

        // TODO support angled lines. This implementation will draw a rectangle instead of angled line.
        if (minX != maxX) {
                for (; minX <= maxX; ++minX) {
                        set ({minX, minY});
                }
        }
        else {
                for (; minY <= maxY; ++minY) {
                        set ({minX, minY});
                }
        }
}

} // namespace le
