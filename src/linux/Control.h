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
#include <etl/list.h>
#include <optional>

namespace le {

class ButtonQueue {
public:
        void run ();

        Button getButtons (Button which) const;

        std::optional<Button> getButton (Button which) const
        {
                auto b = getButtons (which);

                if (b != Button::NONE) {
                        return b;
                }

                return {};
        }

private:
        void onPress ();
        void onRelease ();

private:
        mutable etl::list <Button, 4> queue;

};

} // namespace le
