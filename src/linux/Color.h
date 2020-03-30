/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>

namespace le {

struct Color {
        uint8_t r{};
        uint8_t g{};
        uint8_t b{};

        operator ::sf::Color () const { return {r, g, b}; }
};

} // namespace le
