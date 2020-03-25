/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#pragma "once"

namespace le {

/**
 * @brief The IGame struct
 */
struct IGame {

        IGame () = default;
        virtual ~IGame () = default;
        IGame (IGame const &) = delete;
        IGame &operator= (IGame const &) = delete;
        IGame (IGame &&) = delete;
        IGame &operator= (IGame &&) = delete;

        virtual void run () = 0;
        virtual int getScore () const = 0;
        virtual void reset () = 0;
};


} // namespace le
