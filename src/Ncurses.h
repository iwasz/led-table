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

namespace le::nc {

template <typename F> class NcursesState {
public:
        static constexpr int MAX_COLORS = 8;
        static constexpr int COLOR_BLACK = 0;
        static constexpr int COLOR_RED = 1;
        static constexpr int COLOR_GREEN = 2;
        static constexpr int COLOR_YELLOW = 3;
        static constexpr int COLOR_BLUE = 4;
        static constexpr int COLOR_MAGENTA = 5;
        static constexpr int COLOR_CYAN = 6;
        static constexpr int COLOR_WHITE = 7;

        explicit NcursesState (F &f) : frameBuffer (f) {}

        void move (Point const &p) { cursor = p; }
        void printw (gsl::not_null<gsl::czstring<>> s);
        void advanceCursor ();
        void attron (int attrs);
        void attroff (int attrs);
        void box ();
        void resizeTerm (DimensionT<int> const &d) { terminalDimension = d; }

private:
        F &frameBuffer;
        Point cursor{}; /// 0,0 is in the top left corner
        std::array<Color, MAX_COLORS> palette{Color{0, 0, 0},    Color{0xff, 0, 0},    Color{0, 0xff, 0},    Color{0xff, 0xff, 0},
                                              Color{0, 0, 0xff}, Color{0xff, 0, 0xff}, Color{0, 0xff, 0xff}, Color{0xff, 0xff, 0xff}};

        // gsl::not_null<Color> currentColor = &palette.at (1);
        Color currentColor = palette.at (COLOR_BLACK);
        DimensionT<int> terminalDimension{F::HEIGHT, F::WIDTH};
};

/*****************************************************************************/

template <typename F> void NcursesState<F>::printw (gsl::not_null<gsl::czstring<>> s)
{
        auto len = int(strlen (s));
        for (int i = 0; i < len; ++i) {
                frameBuffer.set (cursor, currentColor);
                advanceCursor ();
        }
}

/*****************************************************************************/

template <typename F> void NcursesState<F>::advanceCursor ()
{
        ++cursor.x;
        if (cursor.x >= F::WIDTH) {
                cursor.x = 0;
                ++cursor.y;
        }

        if (cursor.y >= F::HEIGHT) {
                cursor.y = F::HEIGHT - 1;
        }
}

/*****************************************************************************/

template <typename F> void NcursesState<F>::attron (int attrs)
{
        attrs &= ~262176;
        attrs >>= 8;
        Expects (attrs > 0);
        Expects (attrs < MAX_COLORS);

        currentColor = palette.at (attrs);
}

/*****************************************************************************/

template <typename F> void NcursesState<F>::attroff (int /*attrs*/) { currentColor = palette.at (COLOR_BLACK); }

/*****************************************************************************/

template <typename F> void NcursesState<F>::box ()
{
        for (int c = 0; c < terminalDimension.w; ++c) {
                for (int r = 0; r < terminalDimension.h; ++r) {
                        frameBuffer.set ({c, r}, Color{0xff, 0xff, 0xff});
                }
        }
}

} // namespace le::nc
