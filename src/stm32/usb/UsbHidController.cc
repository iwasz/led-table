/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "UsbHidController.hh"
#include "Debug.h"

namespace le::usb {

static bool ready{};
static Type currentType{};
extern "C" void usbhUserProcess (USBH_HandleTypeDef * /* pHost */, uint8_t id);

/****************************************************************************/

UsbHidController::UsbHidController ()
{
        /* Init Host Library */
        USBH_Init (&hUSBHost, usbhUserProcess, 0);

        /* Add Supported Class */
        USBH_RegisterClass (&hUSBHost, USBH_HID_CLASS);

        /* Start Host Process */
        USBH_Start (&hUSBHost);

        /*--------------------------------------------------------------------------*/
        // once (can be run after USBH_Process has ben run)

        // if (USBH_HID_GetDeviceType (&hUSBHost) == HID_KEYBOARD) {
        //         debug->println ("Keyboard detected");
        // }
        // else if (USBH_HID_GetDeviceType (&hUSBHost) == HID_MOUSE) {
        //         debug->println ("Mouse detected");
        // }
        // else {
        //         debug->println ("Keyboard detected");
        // }

        /*--------------------------------------------------------------------------*/
        // once

        /* Force HID Device to re-enumerate */
        // USBH_ReEnumerate (&hUSBHost);
}

/****************************************************************************/

void UsbHidController::run ()
{
        /* USB Host Background task */
        USBH_Process (&hUSBHost);

        // processKeyboard ();
        // processMouse ();
        // processGamepad ();
}

/****************************************************************************/

// void UsbHidController::processGamepad ()
// {
//         HidGamepadInfo *info = usbhHidGetGamepadInfo (&hUSBHost);

//         if (info != nullptr) {
//                 debug->print (info->x);
//                 debug->print (" ");
//                 debug->println (info->y);
//         }
// }

/****************************************************************************/

// void UsbHidController::processMouse ()
// {
//         HID_MOUSE_Info_TypeDef *m = USBH_HID_GetMouseInfo (&hUSBHost);

//         if (m != nullptr) {
//                 /* Handle Mouse data position */
//                 if ((m->x != 0) && (m->y != 0)) {
//                         debug->print ("x=");
//                         debug->print (m->x);
//                         debug->print (", y=");
//                         debug->println (m->y);
//                 }

//                 if (m->buttons[0] != 0) {
//                         debug->print ("Button pressed 0");
//                 }
//                 else {
//                         // previous state should be used to asses if button was released or not pressed at all.
//                         // HID_MOUSE_ButtonReleased (0);
//                 }

//                 if (m->buttons[1] != 0) {
//                         debug->print ("Button pressed 1");
//                 }
//                 else {
//                         // HID_MOUSE_ButtonReleased (2);
//                 }

//                 if (m->buttons[2] != 0) {
//                         debug->print ("Button pressed 2");
//                 }
//                 else {
//                         // HID_MOUSE_ButtonReleased (1);
//                 }
//         }
// }

/****************************************************************************/

// void UsbHidController::processKeyboard ()
// {
//         HID_KEYBD_Info_TypeDef *k_pinfo = USBH_HID_GetKeybdInfo (&hUSBHost);

//         if (k_pinfo != nullptr) {

//                 char c = USBH_HID_GetASCIICode (k_pinfo);

//                 if (c != 0) {
//                         debug->println (c);
//                 }
//         }
// }

/****************************************************************************/

Type UsbHidController::getType () const { return currentType; }

/****************************************************************************/

bool UsbHidController::isReady () const { return ready; }

/****************************************************************************/

extern "C" HCD_HandleTypeDef hhcd;
extern "C" void OTG_FS_IRQHandler (void) { HAL_HCD_IRQHandler (&hhcd); }

/****************************************************************************/

extern "C" void usbhUserProcess (USBH_HandleTypeDef *pHost, uint8_t id)
{
        switch (id) {
        case HOST_USER_CLASS_SELECTED: {
                ready = true;
                HID_TypeTypeDef hidType = HID_UNKNOWN;

                if (pHost->device.CfgDesc.Itf_Desc[pHost->device.current_interface].bInterfaceProtocol == HID_KEYBRD_BOOT_CODE) {
                        currentType = Type::KEYBOARD;
                }
                else if (pHost->device.CfgDesc.Itf_Desc[pHost->device.current_interface].bInterfaceProtocol == HID_MOUSE_BOOT_CODE) {
                        currentType = Type::MOUSE;
                }
                else if (pHost->device.CfgDesc.Itf_Desc[pHost->device.current_interface].bInterfaceProtocol == HID_SNES_GAMEPAD_CODE) {
                        currentType = Type::GAMEPAD;
                }

        } break;

        case HOST_USER_DISCONNECTION:
                ready = false;
                break;

        case HOST_USER_SELECT_CONFIGURATION:
        case HOST_USER_CLASS_ACTIVE:
        case HOST_USER_CONNECTION:
        default:
                break;
        }
}

} // namespace le::usb