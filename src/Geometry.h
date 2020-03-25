/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include <cstdint>
#include <type_traits>

namespace le {

template <typename T> struct PointT {
        T x;
        T y;
};

using Point = PointT<int>;

template <typename T> bool operator== (PointT<T> const &a, PointT<T> const &b) { return a.x == b.x && a.y == b.y; }
template <typename T> bool operator!= (PointT<T> const &a, PointT<T> const &b) { return a.x != b.x || a.y != b.y; }

template <typename P> struct LineT {
        P a;
        P b;
};

using Line = LineT<Point>;

template <typename T> struct DimensionT {
        T w;
        T h;
};

using Dimension = DimensionT<float>;

/**
 *
 */
template <typename T, typename Act> void pathForEach (T const &pointCollection, Act act)
{
        static_assert (std::is_same<typename T::value_type, Point>::value, "Please provide a collection of Points.");

        if (pointCollection.size () < 2) {
                return;
        }

        Point prev;
        for (auto i = pointCollection.cbegin (); i != pointCollection.cend (); ++i) {
                if (i == pointCollection.cbegin ()) {
                        prev = *i;
                        continue;
                }

                act (prev, *i);
                prev = *i;
        }
}

// TODO
template <typename T> bool intersects (LineT<T> const &l1, LineT<T> const &l2) { return true; }

template <typename L> L normalize (L l)
{
        if (l.a.x > l.b.x) {
                std::swap (l.a.x, l.b.x);
        }
        if (l.a.y > l.b.y) {
                std::swap (l.a.y, l.b.y);
        }

        return l;
}

template <typename T> bool intersects (LineT<PointT<T>> const &l, PointT<T> const &p)
{
        auto ln = normalize (l);

        return ((p.y == ln.a.y && p.x >= ln.a.x && p.x <= ln.b.x)      // Horizontal line
                || (p.x == ln.a.x && p.y >= ln.a.y && p.y <= ln.b.y)); // Vertical line
}

} // namespace le
