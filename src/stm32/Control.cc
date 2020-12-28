/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Buttons.h"
#include <cstdio>
#include <etl/string.h>
#include <gsl/gsl>

namespace le {

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
