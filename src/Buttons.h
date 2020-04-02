/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once

namespace le {

enum class Button : unsigned int {
        NONE = 0x0000,
        Q = 0x0001, // L
        W = 0x0002, // Joystick up 100%
        A = 0x0004, // Joystick left 100%
        S = 0x0008, // Joystick down 100%
        D = 0x0010, // Joystick right 100%
        Z = 0x0020, // SELECT
        X = 0x0040, // START
        I = 0x0080, // X
        J = 0x0100, // Y
        K = 0x0200, // B
        L = 0x0400, // A
        O = 0x0800, // R
};

inline le::Button operator| (le::Button l, le::Button r) { return static_cast<Button> (static_cast<unsigned> (l) | static_cast<unsigned> (r)); }
inline le::Button operator& (le::Button l, le::Button r) { return static_cast<Button> (static_cast<unsigned> (l) & static_cast<unsigned> (r)); }
inline le::Button operator^ (le::Button l, le::Button r) { return static_cast<Button> (static_cast<unsigned> (l) ^ static_cast<unsigned> (r)); }
inline le::Button operator~ (le::Button r) { return static_cast<Button> (~static_cast<unsigned> (r)); }
inline le::Button &operator|= (le::Button &l, le::Button r)
{
        l = l | r;
        return l;
}
inline le::Button &operator&= (le::Button &l, le::Button r)
{
        l = l & r;
        return l;
}

} // namespace le
