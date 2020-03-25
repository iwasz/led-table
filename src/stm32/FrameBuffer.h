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
#include "Hal.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <gsl/gsl>

extern "C" {
#include "ws2812b/ws2812b.h"
}

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

        FrameBuffer ();
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

/****************************************************************************/

template <int W, int H> FrameBuffer<W, H>::FrameBuffer ()
{
        // Set output channel/pin, GPIO_PIN_0 = 0, for GPIO_PIN_5 = 5 - this has to correspond to WS2812B_PINS
        ws2812b.item[0].channel = 0;
        // Your RGB framebuffer
        ws2812b.item[0].frameBufferPointer = data.data ();
        // RAW size of framebuffer
        ws2812b.item[0].frameBufferSize = data.size ();

        ws2812b_init ();
}

/**
 * Draw content onto the phyiscal screen of some sort.
 * @brief draw
 * @param frameBuffer
 * @param window
 */
template <typename Fb, typename Win> void display (Fb const &fb, Win &window, Dimension const &pixelDimension)
{
        if (ws2812b.transferComplete) {
                ws2812b.startTransfer = 1;
                ws2812b_handle ();
        }
}

} // namespace le::fb
