```dmesg``` output:

```
[33865.473820] usb 2-2: new low-speed USB device number 22 using xhci_hcd
[33865.657327] usb 2-2: New USB device found, idVendor=0810, idProduct=e501, bcdDevice= 1.06
[33865.657333] usb 2-2: New USB device strings: Mfr=0, Product=2, SerialNumber=0
[33865.657336] usb 2-2: Product: usb gamepad           
[33865.662110] input: usb gamepad            as /devices/pci0000:00/0000:00:14.0/usb2/2-2/2-2:1.0/0003:0810:E501.0010/input/input47
[33865.662507] hid-generic 0003:0810:E501.0010: input,hidraw7: USB HID v1.10 Joystick [usb gamepad           ] on usb-0000:00:14.0-2/input0
```

```lsusb``` output:

```
Bus 002 Device 022: ID 0810:e501 Personal Communication Systems, Inc. SNES Gamepad
Couldn't open device, some information will be missing
Device Descriptor:
  bLength                18
  bDescriptorType         1
  bcdUSB               1.00
  bDeviceClass            0 
  bDeviceSubClass         0 
  bDeviceProtocol         0 
  bMaxPacketSize0         8
  idVendor           0x0810 Personal Communication Systems, Inc.
  idProduct          0xe501 SNES Gamepad
  bcdDevice            1.06
  iManufacturer           0 
  iProduct                2 
  iSerial                 0 
  bNumConfigurations      1
  Configuration Descriptor:
    bLength                 9
    bDescriptorType         2
    wTotalLength       0x0022
    bNumInterfaces          1
    bConfigurationValue     1
    iConfiguration          0 
    bmAttributes         0x80
      (Bus Powered)
    MaxPower              500mA
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        0
      bAlternateSetting       0
      bNumEndpoints           1
      bInterfaceClass         3 Human Interface Device
      bInterfaceSubClass      0 
      bInterfaceProtocol      0 
      iInterface              0 
        HID Device Descriptor:
          bLength                 9
          bDescriptorType        33
          bcdHID               1.10
          bCountryCode           33 US
          bNumDescriptors         1
          bDescriptorType        34 Report
          wDescriptorLength      89
         Report Descriptors: 
           ** UNAVAILABLE **
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x81  EP 1 IN
        bmAttributes            3
          Transfer Type            Interrupt
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0008  1x 8 bytes
        bInterval              10

A mouse for comparison:

```
Bus 002 Device 023: ID 045e:0797 Microsoft Corp. Optical Mouse 200
Couldn't open device, some information will be missing
Device Descriptor:
  bLength                18
  bDescriptorType         1
  bcdUSB               2.00
  bDeviceClass            0 
  bDeviceSubClass         0 
  bDeviceProtocol         0 
  bMaxPacketSize0         8
  idVendor           0x045e Microsoft Corp.
  idProduct          0x0797 Optical Mouse 200
  bcdDevice            2.00
  iManufacturer           0 
  iProduct                2 
  iSerial                 0 
  bNumConfigurations      1
  Configuration Descriptor:
    bLength                 9
    bDescriptorType         2
    wTotalLength       0x0022
    bNumInterfaces          1
    bConfigurationValue     1
    iConfiguration          0 
    bmAttributes         0xa0
      (Bus Powered)
      Remote Wakeup
    MaxPower               98mA
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        0
      bAlternateSetting       0
      bNumEndpoints           1
      bInterfaceClass         3 Human Interface Device
      bInterfaceSubClass      1 Boot Interface Subclass
      bInterfaceProtocol      2 Mouse
      iInterface              0 
        HID Device Descriptor:
          bLength                 9
          bDescriptorType        33
          bcdHID               1.11
          bCountryCode            0 Not supported
          bNumDescriptors         1
          bDescriptorType        34 Report
          wDescriptorLength      64
         Report Descriptors: 
           ** UNAVAILABLE **
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x81  EP 1 IN
        bmAttributes            3
          Transfer Type            Interrupt
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0005  1x 5 bytes
        bInterval           10
```

On data:

USBH_LL_SubmitURB@0x080099d6 (/home/iwasz/workspace/led-table/src/stm32/usb/usbh_conf.c:317)
USBH_InterruptReceiveData@0x0800c7f6 (/home/iwasz/workspace/led-table/deps/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c:249)
USBH_HID_Process@0x0800d1ec (/home/iwasz/workspace/led-table/deps/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.c:343)
USBH_Process@0x0800b962 (/home/iwasz/workspace/led-table/deps/STM32_USB_Host_Library/Core/Src/usbh_core.c:522)
le::usb::Usb::run@0x08009678 (/home/iwasz/workspace/led-table/src/stm32/usb/Usb.cc:54)
main@0x08006612 (/home/iwasz/workspace/led-table/src/main.cc:55)
