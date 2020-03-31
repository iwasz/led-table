/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include "Timer.h"
#include "UsbHidController.hh"
#include <deque>
#include <etl/cstring.h>
#include <etl/deque.h>
#include <optional>

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

/****************************************************************************/

class SingleButton {
public:
        void onPress ();
        void onRelease ();

        Button getButtons () const { return currentButton; }
        std::optional<Button> getButton () const { return getButtons (); }

private:
        mutable Button currentButton;
};

/****************************************************************************/

template <typename Impl> class ButtonQueue {
public:
        ButtonQueue (Impl &usb) : usb{usb} {}

        void onPress ();
        void onRelease ();

        Button getButtons () const;
        std::optional<Button> getButton () const { return getButtons (); }

private:
        mutable etl::deque<Button, 4> queue;
        Impl &usb;
};

/****************************************************************************/

template <typename Impl> void ButtonQueue<Impl>::onPress ()
{
        Button currentButton{};

        // if (sf::Keyboard::isKeyPressed (sf::Keyboard::Q)) {
        //         currentButton = Button::Q;
        // }
        // else if (sf::Keyboard::isKeyPressed (sf::Keyboard::W)) {
        //         currentButton = Button::W;
        // }
        // else if (sf::Keyboard::isKeyPressed (sf::Keyboard::A)) {
        //         currentButton = Button::A;
        // }
        // else if (sf::Keyboard::isKeyPressed (sf::Keyboard::S)) {
        //         currentButton = Button::S;
        // }
        // else if (sf::Keyboard::isKeyPressed (sf::Keyboard::D)) {
        //         currentButton = Button::D;
        // }

        if (queue.size () < 4) {
                queue.push_back (currentButton);
        }
}

/****************************************************************************/

template <typename Impl> void ButtonQueue<Impl>::onRelease () {}

/****************************************************************************/

template <typename Impl> Button ButtonQueue<Impl>::getButtons () const
{
        if (!queue.empty ()) {
                auto tmp = queue.front ();
                queue.pop_front ();
                return tmp;
        }

        return Button::NONE;
}

} // namespace le
