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
#include "UsbHidController.hh"
#include <deque>
#include <etl/string.h>
#include <optional>
#include <etl/list.h>

namespace le {

/**
 * Gets keys from whatever is conncted. This method is implemnted here, to make
 * use of the Button enum.
 */
template <typename Impl> std::optional<Button> getButtonFromUsb (Impl &impl)
{
        if (!impl.isReady ()) {
                return {};
        }

        le::usb::Type type = impl.getType ();
        Button ret{};

        if (type == le::usb::Type::KEYBOARD) {
        }
        else if (type == le::usb::Type::GAMEPAD) {
                HidGamepadInfo *info = impl.getGamepadInfo ();

                if (info == nullptr) {
                        return {};
                }

                if (info->x < -64) {
                        ret |= Button::A;
                }
                else if (info->x > 64) {
                        ret |= Button::D;
                }

                if (info->y < -64) {
                        ret |= Button::W;
                }
                else if (info->y > 64) {
                        ret |= Button::S;
                }

                if ((info->buttons & BUTTON_X) != 0) {
                        ret |= Button::I;
                }
                if ((info->buttons & BUTTON_A) != 0) {
                        ret |= Button::L;
                }
                if ((info->buttons & BUTTON_B) != 0) {
                        ret |= Button::K;
                }
                if ((info->buttons & BUTTON_Y) != 0) {
                        ret |= Button::J;
                }

                if ((info->buttons & BUTTON_SELECT) != 0) {
                        ret |= Button::Z;
                }
                if ((info->buttons & BUTTON_START) != 0) {
                        ret |= Button::X;
                }

                if ((info->buttons & BUTTON_L) != 0) {
                        ret |= Button::Q;
                }
                if ((info->buttons & BUTTON_R) != 0) {
                        ret |= Button::O;
                }
        }

        return ret;
}

/****************************************************************************/

void printButtons (Button b);

/****************************************************************************/

template <typename Impl> class ButtonQueue {
public:
        ButtonQueue (Impl &usb) : usb{usb} {}

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
        void onPress (Button b);
        void onRelease (Button b);
        void check (Button diff, Button current, Button button);

private:
        mutable etl::list <Button, 4> queue;
        Impl &usb;
        Button previous{};
};

/****************************************************************************/

template <typename Impl> void ButtonQueue<Impl>::run ()
{
        auto current = getButtonFromUsb (usb);

        if (!current || *current == previous) {
                return;
        }

        // printButtons (*current);

        Button diff = *current ^ previous;
        check (diff, *current, Button::Q);
        check (diff, *current, Button::W);
        check (diff, *current, Button::A);
        check (diff, *current, Button::S);
        check (diff, *current, Button::D);
        check (diff, *current, Button::Z);
        check (diff, *current, Button::X);
        check (diff, *current, Button::I);
        check (diff, *current, Button::J);
        check (diff, *current, Button::K);
        check (diff, *current, Button::L);
        check (diff, *current, Button::O);
        previous = *current;
}

/****************************************************************************/

template <typename Impl> void ButtonQueue<Impl>::check (Button diff, Button current, Button button)
{
        if ((diff & button) == button) {
                if ((current & button) == button) {
                        onPress (button);
                }
                else {
                        onRelease (button);
                }
        }
}

/****************************************************************************/

template <typename Impl> void ButtonQueue<Impl>::onPress (Button b)
{
        if (    b != Button::NONE) {
                if (queue.full ()) {
                        queue.pop_front ();
                }

                queue.push_back(b);
        }

}

/****************************************************************************/

template <typename Impl> void ButtonQueue<Impl>::onRelease (Button b)
{
        // printf ("Rl ");
        // printButtons (b);
}

/****************************************************************************/

template <typename Impl> Button ButtonQueue<Impl>::getButtons (Button which) const
{
        for (auto i = queue.begin (); i != queue.end (); ) {
                auto b = *i;
                auto j = i;
                ++j;

                if ((b & which) == b) {
                        queue.erase (i);
                        return b;
                }

                i = j;
        }

        return Button::NONE;
}

} // namespace le
