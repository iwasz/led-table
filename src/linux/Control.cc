/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Control.h"
#include "System.hh"
#include <SFML/Graphics.hpp>
#include <gsl/gsl>
#include <optional>

namespace le {

void ButtonQueue::run ()
{
        sf::Event event{};

        while (le::system::getWindow ().pollEvent (event)) {
                if (event.type == sf::Event::Closed) {
                        le::system::getWindow ().close ();
                        // running = false;
                        break;
                }

                if (event.type == sf::Event::KeyPressed) {
                        onPress ();
                }
                else if (event.type == sf::Event::KeyReleased) {
                        onRelease ();
                }
        }
}

/****************************************************************************/

void ButtonQueue::onPress ()
{
        Button currentButton{};

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Q)) {
                currentButton = Button::Q;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::W)) {
                currentButton = Button::W;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::A)) {
                currentButton = Button::A;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::S)) {
                currentButton = Button::S;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::D)) {
                currentButton = Button::D;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::I)) {
                currentButton = Button::I;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::J)) {
                currentButton = Button::J;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::K)) {
                currentButton = Button::K;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::L)) {
                currentButton = Button::L;
        }
        else if (sf::Keyboard::isKeyPressed (sf::Keyboard::O)) {
                currentButton = Button::O;
        }

        if (    currentButton != Button::NONE) {
                if (queue.full ()) {
                        queue.pop_front ();
                }

                queue.push_back(currentButton);
        }
}

/****************************************************************************/

void ButtonQueue::onRelease () {}

/****************************************************************************/

Button ButtonQueue::getButtons (Button which) const
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
