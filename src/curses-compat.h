/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/// Mind the reversed order of params.
int move (int y, int x);

int attron (int attrs);
int attroff (int attrs);

int printw (const char *fmt);
int resize_term (int lines, int columns);
int refresh ();
int getch ();

/// initscr  is  normally the first curses routine to call when initializing a program.
void initscr ();
int endwin ();

/// To use these routines start_color must be called, usually right after initscr.
int start_color ();

/// A programmer initializes a  color-pair with the routine init_pair.
int init_pair (short pair, short f, short b);

/// The  function resizeterm resizes the standard and current windows to the specified dimensions.
int resizeterm (int lines, int columns);

/// The echo and noecho routines control whether characters typed by the user are echoed by getch.
int noecho ();

/// Something with input.
void timeout (int delay);

/// The curs_set routine sets the cursor state to invisible, normal, or very visible for visibility equal to 0, 1, or 2 respectively.
int curs_set (int visibility);

/// Draws a box???
int box (void *win, int verch, int horch);

#ifdef __cplusplus
}
#endif
