/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "System.hh"
#include <cstdlib>
#include <ctime>

namespace le::system {
void init () { srand (time (0)); }
} // namespace le::system