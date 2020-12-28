# Bugs
* [ ] one of the snake's segments was (9,1) - (9,-1). 
* [ ] 

# What?
This is a firmware for [my 32x32 LED table](https://hackaday.io/project/170709-32x32-led-table-with-usb-hid) which is an interactive table that uses HID input devices for control. It accepts USb gamepad(s) and keyboards. 

Also, included in this project, are sources for an emulator, which les one to develop and test his games/animations in a friendly environment. 

It looks like this:

![alt text](https://github.com/iwasz/led-table/blob/master/led-table.jpg)

# To compile the emulator
Simply use cmake:

```sh
mkdir build
cd build
cmake ..
ninja
```
Dependencies are:
* SFML

![alt text](https://github.com/iwasz/led-table/blob/master/emulator.png)

# To compile the firmware
## Modify a file
Unfortunately you have to modify the ```deps/umm_malloc/src/umm_malloc_cfg.h``` file. This should be at the beginning (replacing old macros):

```cpp
extern char _end;
#define UMM_MALLOC_CFG_HEAP_ADDR (&_end)
#define UMM_MALLOC_CFG_HEAP_SIZE (0x10000)      
```

I didn't want to fork UMM_malloc, so this is required.

## Use a toolchain file:
For example this is mine:
```cmake
SET (DEVICE "STM32F446xx")
SET (STARTUP_CODE "${CMAKE_SOURCE_DIR}/startup_stm32f446xx.s")
SET (LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F446.ld")

SET (CMAKE_SYSTEM_NAME Generic)
SET (CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_C_FLAGS "-std=c11 -fdata-sections -ffunction-sections -Wall -Werror=return-type" CACHE INTERNAL "c compiler flags")
SET(CMAKE_CXX_FLAGS "-std=c++17 -fno-rtti -fno-exceptions -Wall -fdata-sections -ffunction-sections -Wall -Werror=return-type -Wno-register" CACHE INTERNAL "cxx compiler flags")
SET (CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -specs=nosys.specs -Wl,--gc-sections" CACHE INTERNAL "exe link flags")

INCLUDE_DIRECTORIES(${SUPPORT_FILES})
LINK_DIRECTORIES(${SUPPORT_FILES})
ADD_DEFINITIONS(-D${DEVICE})
```

## Use a cross compiler
I use a compiler made with [these instructions](http://www.iwasz.pl/uncategorized/cross-compilation-with-gcc-qtcreator-for-arm-cortex-m/).