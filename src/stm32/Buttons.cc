/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Buttons.h"
#include <cstdio>
#include <gsl/gsl>

namespace le {

void Buttons::onPress ()
{
        // std::optional<Button> bf;

        // if (!currentButtons.empty ()) {
        //         bf = currentButtons.back ();
        // }

        // auto f = gsl::finally ([this, &bf] { fmt::print ("B : {}\n", static_cast<unsigned int> (bf)); });

        // TODO SORT THIS OUT. FOLLOWING IMPL. IS HILAROUS.

        // if (sf::Keyboard::isKeyPressed (sf::Keyboard::Q)) {
        //         currentButtons = currentButtons | Button::Q;
        // }
        // else if (sf::Keyboard::isKeyPressed (sf::Keyboard::W)) {
        //         currentButtons = currentButtons | Button::W;
        // }
        // else if (sf::Keyboard::isKeyPressed (sf::Keyboard::A)) {
        //         currentButtons = currentButtons | Button::A;
        // }
        // else if (sf::Keyboard::isKeyPressed (sf::Keyboard::S)) {
        //         currentButtons = currentButtons | Button::S;
        // }
        // else if (sf::Keyboard::isKeyPressed (sf::Keyboard::D)) {
        //         currentButtons = currentButtons | Button::D;
        // }
}

std::optional<Button> Buttons::getButton () const
{
        while (int (currentButtons) != 0) {
                if (int (currentButtons & Button::Q) != 0) {
                        currentButtons = currentButtons & ~Button::Q;
                        return Button::Q;
                }

                if (int (currentButtons & Button::W) != 0) {
                        currentButtons = currentButtons & ~Button::W;
                        return Button::W;
                }

                if (int (currentButtons & Button::A) != 0) {
                        currentButtons = currentButtons & ~Button::A;
                        return Button::A;
                }

                if (int (currentButtons & Button::S) != 0) {
                        currentButtons = currentButtons & ~Button::S;
                        return Button::S;
                }

                if (int (currentButtons & Button::D) != 0) {
                        currentButtons = currentButtons & ~Button::D;
                        return Button::D;
                }
        }

        return {};
}

void Buttons::onRelease ()
{ /* currentButtons = Button::NONE; */
}

void SingleButton::onPress ()
{
        if (currentButton != Button::NONE) {
                return;
        }

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
}

void SingleButton::onRelease () { currentButton = Button::NONE; }

/****************************************************************************/

void printButtons (Button b)
{
        etl::string<12> tmp = "            ";

        if ((b & Button::Q) == Button::Q) {
                tmp.at (0) = 'q';
        }
        if ((b & Button::W) == Button::W) {
                tmp.at (1) = 'w';
        }
        if ((b & Button::A) == Button::A) {
                tmp.at (2) = 'a';
        }
        if ((b & Button::S) == Button::S) {
                tmp.at (3) = 's';
        }
        if ((b & Button::D) == Button::D) {
                tmp.at (4) = 'd';
        }
        if ((b & Button::Z) == Button::Z) {
                tmp.at (5) = 'z';
        }
        if ((b & Button::X) == Button::X) {
                tmp.at (6) = 'x';
        }
        if ((b & Button::I) == Button::I) {
                tmp.at (7) = 'i';
        }
        if ((b & Button::J) == Button::J) {
                tmp.at (8) = 'j';
        }
        if ((b & Button::K) == Button::K) {
                tmp.at (9) = 'k';
        }
        if ((b & Button::L) == Button::L) {
                tmp.at (10) = 'l';
        }
        if ((b & Button::O) == Button::O) {
                tmp.at (11) = 'o';
        }

        printf ("%s\r\n", tmp.c_str ());
}

} // namespace le
