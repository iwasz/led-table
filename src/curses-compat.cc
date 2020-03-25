/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "curses-compat.h"
#include "Globals.h"

extern "C" int move (int y, int x)
{
        ncurses.move ({x, y});
        return 0;
}

extern "C" void initscr () {}
extern "C" int start_color () { return 0; }
extern "C" int noecho () { return 0; }
extern "C" int curs_set (int /*visibility*/) { return 0; }
extern "C" int endwin () { return 0; }

extern "C" int attron (int attrs)
{
        ncurses.attron (attrs);
        return 0;
}
extern "C" int attroff (int attrs)
{
        ncurses.attroff (attrs);
        return 0;
}
extern "C" int printw (const char *fmt)
{
        ncurses.printw (fmt);
        return 0;
}

extern "C" int resize_term (int /*lines*/, int columns) { return 0; }
extern "C" int refresh () { return 0; }

extern "C" int getch ()
{
        auto b = buttons.getButtons ();
        // TODO this might be wrong implemntation, have to check.

        if (b == le::Button::Q) {
                return int ('q');
        }

        if (b == le::Button::W) {
                return int ('w');
        }

        if (b == le::Button::A) {
                return int ('a');
        }

        if (b == le::Button::S) {
                return int ('s');
        }

        if (b == le::Button::D) {
                return int ('d');
        }

        return -1;
}

extern "C" int init_pair (short pair, short f, short b) { return 0; }

extern "C" int resizeterm (int lines, int columns)
{
        ncurses.resizeTerm ({columns, lines});
        return 0;
}

extern "C" void timeout (int delay) {}

extern "C" int box (void *win, int verch, int horch)
{
        ncurses.box ();
        return 0;
}
