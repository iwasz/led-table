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

static constexpr Color BLACK{0, 0, 0};
static constexpr Color WHITE{255, 255, 255};
static constexpr Color LIGHT_GRAY{224, 224, 224};
static constexpr Color GRAY{128, 128, 128};
static constexpr Color DARK_GRAY{64, 64, 64};
static constexpr Color RED{255, 0, 0};
static constexpr Color PINK{255, 96, 208};
static constexpr Color PURPLE{160, 32, 255};
static constexpr Color LIGHT_BLUE{80, 208, 255};
static constexpr Color BLUE{0, 0, 255};
static constexpr Color LIGHT_GREEN{96, 255, 128};
static constexpr Color GREEN{0, 255, 0};
static constexpr Color YELLOW{255, 224, 32};
static constexpr Color ORANGE{255, 160, 16};
static constexpr Color BROWN{160, 128, 96};
static constexpr Color PALE_PINK{255, 208, 160};

} // namespace le
