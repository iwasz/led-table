/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "Color.h"
#include "Geometry.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <gsl/gsl>

namespace le::fb {

/**
 * BGR.
 */
template <int W, int H> class FrameBuffer {
public:
        static constexpr int WIDTH = W;
        static constexpr int HEIGHT = W;
        static constexpr int PIXEL_COUNT = WIDTH * HEIGHT;
        static constexpr int BYTES_PER_PIXEL = 3;
        static constexpr int FRAME_BUFFER_SIZEOF = PIXEL_COUNT * BYTES_PER_PIXEL;

        FrameBuffer () = default;
        FrameBuffer (FrameBuffer const &) = delete;
        FrameBuffer &operator= (FrameBuffer const &) = delete;
        FrameBuffer (FrameBuffer &&) = delete;
        FrameBuffer &operator= (FrameBuffer &&) = delete;
        ~FrameBuffer () = default;

        void randomize ()
        {
                std::generate (data.begin (), data.end (), [] () { return std::rand () % 256; });
        }

        Color get (Point const &p) const
        {
                gsl::index px = (p.y * WIDTH + p.x) * BYTES_PER_PIXEL;
                return Color{data.at (px + 2), data.at (px + 1), data.at (px)};
        }

        void set (Point const &p, Color const &c)
        {
                size_t px = (p.y * WIDTH + p.x) * BYTES_PER_PIXEL;
                data.at (px) = c.b;
                data.at (px + 1) = c.g;
                data.at (px + 2) = c.r;
        }

        void clear () { std::fill (data.begin (), data.end (), 0); }

private:
        /// Row after row. TODO Why not array of Colors?
        std::array<uint8_t, FRAME_BUFFER_SIZEOF> data{};
};

/**
 * Draw content onto the phyiscal screen of some sort.
 * @brief draw
 * @param frameBuffer
 * @param window
 */
template <typename Fb, typename Win> void display (Fb const &fb, Win &window, Dimension const &pixelDimension)
{
        using namespace sf;
        auto const &[pixW, pixH] = pixelDimension;

        for (int y = 0; y < fb.HEIGHT; ++y) {
                for (int x = 0; x < fb.WIDTH; ++x) {
                        RectangleShape shape ({pixW, pixH});
                        shape.setFillColor (fb.get ({x, y}));
                        shape.move ({std::round (float (x) * pixW), std::round (float (y) * pixH)});
                        window.draw (shape);
                }
        }
}

} // namespace le::fb
