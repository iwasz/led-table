/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "Timer.h"
#include <SFML/Graphics.hpp>
#include <deque>

namespace le {

enum class Button : unsigned int { NONE = 0, Q = 0x01, W = 0x02, A = 0x04, S = 0x08, D = 0x10 };

class Buttons {
public:
        // enum class Button : unsigned int { Q = 0x01, W = 0x02, A = 0x04, S = 0x08, D = 0x10 };
        // static constexpr int BUTTON_LAG_MS = 20;

        void onPress ();
        void onRelease ();

        Button getButtons () const { return currentButtons; }
        std::optional<Button> getButton () const;

private:
        mutable Button currentButtons;
        // Timer timer{BUTTON_LAG_MS};
};

inline le::Button operator| (le::Button l, le::Button r) { return static_cast<Button> (static_cast<unsigned> (l) | static_cast<unsigned> (r)); }

inline le::Button operator& (le::Button l, le::Button r) { return static_cast<Button> (static_cast<unsigned> (l) & static_cast<unsigned> (r)); }

inline le::Button operator~ (le::Button r) { return static_cast<Button> (~static_cast<unsigned> (r)); }

// inline le::Button &operator|= (le::Button &l, le::Button r)
//{
//        l |= r;
//        return l;
//}

class SingleButton {
public:
        void onPress ();
        void onRelease ();

        Button getButtons () const { return currentButton; }
        std::optional<Button> getButton () const { return getButtons (); }

private:
        mutable Button currentButton;
};

class ButtonQueue {
public:
        void onPress ();
        void onRelease ();

        Button getButtons () const;
        std::optional<Button> getButton () const { return getButtons (); }

private:
        mutable std::deque<Button> queue;
};

} // namespace le
