/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Logging.hh"
#include "Debug.h"
#include "ErrorHandler.h"
#include "Gpio.h"
#include "Usart.h"

namespace le::log {

void init ()
{
        Gpio debugUartGpios{GPIOA, GPIO_PIN_2 | GPIO_PIN_3, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF7_USART2};
        Usart debugUart{USART2, 115200};
        static Debug debug (&debugUart);
        ::debug = &debug;
}

} // namespace le::log

extern "C" int _write (int /* file */, char *ptr, int len)
{
        if (debug) {
                debug->print (reinterpret_cast<uint8_t *> (ptr), len);
        }

        return len;
}
