/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include <SFML/Graphics.hpp>

namespace le::system {
void init ();

inline auto &getWindow ()
{
        static sf::RenderWindow window (sf::VideoMode (800, 800), "led-emulator");
        window.setKeyRepeatEnabled (false);
        return window;
}

} // namespace le::system