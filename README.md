# What?
This is a firmware for [my 32x32 LED table](https://hackaday.io/project/170709-32x32-led-table-with-usb-hid) which is a interactive table that uses HID input devices for control. It accepts USb gamepad(s) and keyboards. 

Also, included in this project, are sources for an emulator, which les one to develop and test his games/animations in a friendly environment. 

It looks like this:
![alt text](https://github.com/iwasz/led-table/blob/master/led-table.jpg)

# To compile the emulator

# To compile the firmware
Unfortunately you have to modify the ```deps/umm_malloc/src/umm_malloc_cfg.h``` file. This should be at the beginning (replacing the old macros):

```cpp
extern char _end;
#define UMM_MALLOC_CFG_HEAP_ADDR (&_end)
#define UMM_MALLOC_CFG_HEAP_SIZE (0x10000)      
```

I didn't want to fork UMM_malloc, so this is required.
