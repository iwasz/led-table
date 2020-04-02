CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET (CMAKE_VERBOSE_MAKEFILE OFF)
SET (CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

PROJECT (led-table)
ENABLE_LANGUAGE (ASM-ATT)

INCLUDE_DIRECTORIES("src/")
INCLUDE_DIRECTORIES("deps/etl/include/")
include_directories ("deps/GSL/include")

# DEVICE is defined in the toolchain file, so if it's present, we assume that the firmware is beeing built.
IF (DEFINED DEVICE)
    MESSAGE ("Building the firmware")
    INCLUDE_DIRECTORIES("src/stm32")

    SET (LIB_MICRO_ROOT "deps/libmicro")
    INCLUDE_DIRECTORIES ("${LIB_MICRO_ROOT}/src")

    SET (UMM_MALLOC_ROOT "deps/umm_malloc")
    include_directories ("${UMM_MALLOC_ROOT}/src" "${UMM_MALLOC_ROOT}/includes/c-helper-macros")

    INCLUDE_DIRECTORIES("deps/STM32F4xx_HAL_Driver/Inc/")
    INCLUDE_DIRECTORIES("deps/CMSIS/Device/ST/STM32F4xx/Include/")
    INCLUDE_DIRECTORIES("deps/CMSIS/Include/")

    SET (USBH_ROOT "deps/STM32_USB_Host_Library")
    INCLUDE_DIRECTORIES("${USBH_ROOT}/Core/Inc")
    INCLUDE_DIRECTORIES("${USBH_ROOT}/Class/HID/Inc")
    INCLUDE_DIRECTORIES("src/stm32/usb")
    
    add_definitions("-DUSE_USB_FS=1" 
                    "-DWITH_FIRMWARE=1"
                    )

    SET (SOURCES
        "src/stm32/Control.cc"    
        "src/stm32/usb/UsbHidController.cc"
        "src/stm32/usb/usbh_conf.c"
        "src/stm32/System.cc"
        "src/stm32/Logging.cc"
        "src/ws2812b/ws2812b.c"

        "${LIB_MICRO_ROOT}/src/ErrorHandler.c"
        "${LIB_MICRO_ROOT}/src/TimerF.cc"
        "${LIB_MICRO_ROOT}/src/Debug.cc"
        "${LIB_MICRO_ROOT}/src/GpioF.cc"
        "${LIB_MICRO_ROOT}/src/GpioF4.cc"
        "${LIB_MICRO_ROOT}/src/UsartF.cc"
        "${LIB_MICRO_ROOT}/src/UsartF4.cc"
        "${LIB_MICRO_ROOT}/src/Pwm.cc"

       "${UMM_MALLOC_ROOT}/src/umm_info.c"
       "${UMM_MALLOC_ROOT}/src/umm_integrity.c"
       "${UMM_MALLOC_ROOT}/src/umm_malloc.c"
       "${UMM_MALLOC_ROOT}/src/umm_poison.c"

        "${STARTUP_CODE}"
        "src/stm32/stm32f4xx_it.c"
        "src/stm32/system_stm32f4xx.c"

        "deps/STM32_USB_Host_Library/Core/Src/usbh_core.c"
        "deps/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c"
        "deps/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c"
        "deps/STM32_USB_Host_Library/Core/Src/usbh_pipes.c"

        "deps/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_keybd.c"
        "deps/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_gamepad.c"
        "deps/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_mouse.c"
        "deps/STM32_USB_Host_Library/Class/HID/Src/usbh_hid_parser.c"
        "deps/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.c"

        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cec.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dfsdm.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dsi.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_fmpi2c_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_fmpi2c.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_irda.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_lptim.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_mmc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nand.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nor.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pccard.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_qspi.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sai_ex.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sai.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_smartcard.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spdifrx.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_crc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dac.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma2d.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_i2c.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_lptim.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_pwr.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rng.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rtc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_spi.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_tim.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.c"
        "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c"
        # "deps/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c"
    )
ELSE()
    MESSAGE ("Building the emulator")
    
    include_directories (deps/GSL/include deps/fmt/include)
    find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

    INCLUDE_DIRECTORIES("src/linux")

    SET(CMAKE_C_FLAGS "-std=c11 -Wall -Werror=return-type" CACHE INTERNAL "c compiler flags")
    SET(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror=return-type -Wno-register" CACHE INTERNAL "cxx compiler flags")
    
    add_definitions("-DWITH_EMULATOR=1")

    SET (SOURCES
        "src/linux/Control.cc"
        "src/linux/Timer.cc"
        "src/linux/System.cc"
    )
ENDIF ()

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf
    "src/main.cc"
    "src/Snake.cc"

    ${SOURCES}
)

IF (DEFINED DEVICE)
    FIND_PROGRAM (OPENOCD openocd)
    ADD_CUSTOM_TARGET("upload" DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${OPENOCD} -f /home/iwasz/local/share/openocd/scripts/interface/stlink.cfg -f /home/iwasz/local/share/openocd/scripts/target/stm32f4x.cfg -c 'program ${CMAKE_PROJECT_NAME}.elf verify reset exit')
    ADD_CUSTOM_TARGET("debug" DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${OPENOCD} -f /home/iwasz/local/share/openocd/scripts/interface/stlink.cfg -f /home/iwasz/local/share/openocd/scripts/target/stm32f4x.cfg)

    ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.bin ALL DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)
ELSE ()
    target_link_libraries(${PROJECT_NAME}.elf sfml-graphics sfml-audio)
ENDIF ()