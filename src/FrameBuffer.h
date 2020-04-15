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
#include "System.hh"
#ifdef WITH_EMULATOR
#include <SFML/Graphics.hpp>
#else
#include "Hal.h"
#endif
#include <algorithm>
#include <array>
#include <cmath>
#include <gsl/gsl>

#ifdef WITH_FIRMWARE
extern "C" {
#include "ws2812b/ws2812b.h"
}
#endif

namespace le::fb {

/**
 * BGR.
 */
template <int W, int H, typename Win> class FrameBuffer {
public:
        static constexpr int WIDTH = W;
        static constexpr int HEIGHT = W;
        static constexpr int PIXEL_COUNT = WIDTH * HEIGHT;
        static constexpr int BYTES_PER_PIXEL = 3;
        // static constexpr int FRAME_BUFFER_SIZEOF = PIXEL_COUNT ;

        FrameBuffer (Win &win);
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

        uint8_t *getData () { return reinterpret_cast<uint8_t *> (data.data ()); }
        uint8_t const *getData () const { return reinterpret_cast<uint8_t *> (data.data ()); }

private:
        std::array<Color, PIXEL_COUNT> data{};
        std::array<Color, PIXEL_COUNT> flipped{};
        Win &window;
};

/****************************************************************************/

template <int W, int H, typename Win> FrameBuffer<W, H, Win>::FrameBuffer (Win &w) : window{w}
{
#ifdef WITH_FIRMWARE
        // Set output channel/pin, GPIO_PIN_0 = 0, for GPIO_PIN_5 = 5 - this has to correspond to WS2812B_PINS
        ws2812b.item[0].channel = 0;
        // Your RGB framebuffer
        ws2812b.item[0].frameBufferPointer = reinterpret_cast<uint8_t *> (flipped.data ());
        // RAW size of framebuffer
        ws2812b.item[0].frameBufferSize = flipped.size ();

        ws2812b_init ();
#endif
}

/****************************************************************************/

template <int W, int H, typename Win> void FrameBuffer<W, H, Win>::postProcessBuffer ()
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
#ifdef WITH_FIRMWARE
template <int W, int H, typename Win> void FrameBuffer<W, H, Win>::display ()
{
        if (ws2812b.transferComplete != 0) {
                postProcessBuffer ();
                ws2812b.startTransfer = 1;
                ws2812b_handle ();
        }
}
#else
template <int W, int H, typename Win> void FrameBuffer<W, H, Win>::display ()
{
        using namespace sf;

        auto [winW, winH] = window.getSize ();
        auto pixW = winW / float (WIDTH);
        auto pixH = winH / float (HEIGHT);

        // auto const &[pixW, pixH] = pixelDimension;

        for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; ++x) {
                        RectangleShape shape ({pixW, pixH});
                        shape.setFillColor (get ({x, y}));
                        shape.move ({std::round (float (x) * pixW), std::round (float (y) * pixH)});
                        window.draw (shape);
                }
        }

        le::system::getWindow ().display ();
}
#endif

} // namespace le::fb
