/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "Buttons.h"
#include "Timer.h"
#include <SFML/Graphics.hpp>
#include <deque>
#include <optional>

namespace le {

class ButtonQueue {
public:
        void run ();

        Button getButtons () const;
        std::optional<Button> getButton () const { return getButtons (); }

private:
        void onPress ();
        void onRelease ();

private:
        mutable std::deque<Button> queue;
};

} // namespace le
