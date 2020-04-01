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
        // static constexpr int FRAME_BUFFER_SIZEOF = PIXEL_COUNT ;

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

        Color const &get (Point const &p) const
        {
                gsl::index px = (p.y * WIDTH + p.x);
                return data.at (px);
        }

        Color &get (Point const &p)
        {
                gsl::index px = (p.y * WIDTH + p.x);
                return data.at (px);
        }

        void set (Point const &p, Color const &c)
        {
                size_t px = (p.y * WIDTH + p.x);
                data.at (px) = c;
        }

        void clear () { std::fill (data.begin (), data.end (), Color{}); }

        /// Flips every odd line while copying from data to flipped. Remember, that 0 is even.
        void postProcessBuffer ();

        void display ();

private:
        /// Row after row. TODO Why not array of Colors?
        std::array<Color, PIXEL_COUNT> data{};
        std::array<Color, PIXEL_COUNT> flipped{};
};

/****************************************************************************/

template <int W, int H> FrameBuffer<W, H>::FrameBuffer ()
{
        // Set output channel/pin, GPIO_PIN_0 = 0, for GPIO_PIN_5 = 5 - this has to correspond to WS2812B_PINS
        ws2812b.item[0].channel = 0;
        // Your RGB framebuffer
        ws2812b.item[0].frameBufferPointer = reinterpret_cast<uint8_t *> (flipped.data ());
        // RAW size of framebuffer
        ws2812b.item[0].frameBufferSize = flipped.size ();

        ws2812b_init ();
}

/****************************************************************************/

template <int W, int H> void FrameBuffer<W, H>::postProcessBuffer ()
{
        for (int yi = 0; yi < H; ++yi) {
                for (int xi = 0; xi < W; ++xi) {
                        int yo = H - 1 - yi; // Output row number.
                        int xo = (yi % 2 == 1) ? (W - 1 - xi) : (xi);
                        flipped.at (W * yo + xo) = get ({xi, yi});
                }
        }
}

/**
 * Draw content onto the phyiscal screen of some sort.
 * @brief draw
 * @param frameBuffer
 * @param window
 */
template <int W, int H> void FrameBuffer<W, H>::display ()
{
        if (ws2812b.transferComplete != 0) {
                postProcessBuffer ();
                ws2812b.startTransfer = 1;
                ws2812b_handle ();
        }
}

} // namespace le::fb
