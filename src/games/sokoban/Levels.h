/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once
#include <array>
#include <cstdint>

namespace le::sokoban {

struct Level {
        const char *data{};
        int width{};
};

static constexpr std::array levels{

        Level{"  ####  "
              "###  #  "
              "#    ###"
              "# #  $@#"
              "#  #$*.#"
              "##   . #"
              " ###  ##"
              "   #### ",
              8},

        Level{"   #####"
              "####   #"
              "#   *  #"
              "#  . $##"
              "### * # "
              "  ##@## "
              "   ###  ",
              8}};

} // namespace le::sokoban
