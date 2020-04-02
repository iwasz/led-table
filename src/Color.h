/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#ifdef WITH_EMULATOR
#include <SFML/Graphics.hpp>
#endif
#include <cstdint>

namespace le {

struct Color {
        uint8_t r{};
        uint8_t g{};
        uint8_t b{};

#ifdef WITH_EMULATOR
        operator ::sf::Color () const { return {r, g, b}; }
#endif
};

} // namespace le
