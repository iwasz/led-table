/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Container.h"
#include "Debug.h"
#include "ErrorHandler.h"
#include "Gpio.h"
#include "Hal.h"
#include "Timer.h"
#include "Usart.h"
#include <cerrno>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "Snake.h"
// #include "tinytetris/tinytetris.h"

/*****************************************************************************/

static void SystemClock_Config ();
extern "C" void initialise_monitor_handles ();

static constexpr int FRAME_DURATION_MS = 40;

/*****************************************************************************/

int main ()
{
        HAL_Init ();
        __HAL_RCC_SYSCFG_CLK_ENABLE (); //?
        __HAL_RCC_PWR_CLK_ENABLE ();    //?6
        SystemClock_Config ();
        // initialise_monitor_handles ();

        Gpio debugUartGpios{GPIOA, GPIO_PIN_2 | GPIO_PIN_3, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_AF7_USART2};
        Usart debugUart{USART2, 115200};
        static Debug debug (&debugUart);
        ::debug = &debug;
        debug.print ("test\r\n");

        Timer timer{};

        // tetrisConstructor ();
        le::snake::Game snake{getGraphics (), getSingleButton ()};

        while (true) {
                while (!timer.isExpired ()) {
                }

                timer.start (FRAME_DURATION_MS);

                debug.print ("#\r\n");
                snake.run ();

                int window{};
                le::fb::display (getFrameBuffer (), window, {0, 0});
        }

        return 0;
}

/*****************************************************************************/

void SystemClock_Config (void)
{
        RCC_OscInitTypeDef RCC_OscInitStruct = {0};
        RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
        RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

        /**Configure the main internal regulator output voltage
         */
        __HAL_RCC_PWR_CLK_ENABLE ();
        __HAL_PWR_VOLTAGESCALING_CONFIG (PWR_REGULATOR_VOLTAGE_SCALE1);
        /**Initializes the CPU, AHB and APB busses clocks
         */
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLM = 4;
        RCC_OscInitStruct.PLL.PLLN = 180;
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
        RCC_OscInitStruct.PLL.PLLQ = 3;
        RCC_OscInitStruct.PLL.PLLR = 2;
        if (HAL_RCC_OscConfig (&RCC_OscInitStruct) != HAL_OK) {
                Error_Handler ();
        }
        /**Activate the Over-Drive mode
         */
        if (HAL_PWREx_EnableOverDrive () != HAL_OK) {
                Error_Handler ();
        }
        /**Initializes the CPU, AHB and APB busses clocks
         */
        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

        if (HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
                Error_Handler ();
        }
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
        PeriphClkInitStruct.PLLSAI.PLLSAIM = 4;
        PeriphClkInitStruct.PLLSAI.PLLSAIN = 96;
        PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
        PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV4;
        PeriphClkInitStruct.PLLSAIDivQ = 1;
        PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLSAIP;

        if (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct) != HAL_OK) {
                Error_Handler ();
        }
}

/****************************************************************************/

namespace __gnu_cxx {
void __verbose_terminate_handler ()
{
        while (true) {
        }
}
} // namespace __gnu_cxx
