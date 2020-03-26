/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Usb.hh"
#include "Debug.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
        HID_DEMO_IDLE = 0,
        HID_DEMO_WAIT,
        HID_DEMO_START,
        HID_DEMO_MOUSE,
        HID_DEMO_KEYBOARD,
        HID_DEMO_REENUMERATE,
} HID_Demo_State;

typedef enum {
        HID_MOUSE_IDLE = 0,
        HID_MOUSE_WAIT,
        HID_MOUSE_START,
} HID_mouse_State;

typedef enum {
        HID_KEYBOARD_IDLE = 0,
        HID_KEYBOARD_WAIT,
        HID_KEYBOARD_START,
} HID_keyboard_State;

typedef struct _DemoStateMachine {
        HID_Demo_State state;
        HID_mouse_State mouse_state;
        HID_keyboard_State keyboard_state;
        uint8_t select;
        uint8_t lock;
} HID_DEMO_StateMachine;

typedef enum {
        APPLICATION_IDLE = 0,
        APPLICATION_DISCONNECT,
        APPLICATION_START,
        APPLICATION_READY,
        APPLICATION_RUNNING,
} HID_ApplicationTypeDef;

extern USBH_HandleTypeDef hUSBHost;

HID_ApplicationTypeDef Appli_state = APPLICATION_IDLE;

extern HID_MOUSE_Info_TypeDef mouse_info;
extern uint8_t *DEMO_MOUSE_menu[];
// extern HID_DEMO_StateMachine hid_demo;
HID_DEMO_StateMachine hid_demo;

extern uint8_t prev_select;

namespace le::usb {

void HID_SelectItem (uint8_t **menu, uint8_t item);
void HID_MenuInit (void);
void HID_MenuProcess (void);
void HID_MouseMenuProcess (void);
void HID_KeyboardMenuProcess (void);
void HID_MOUSE_ButtonReleased (uint8_t button_idx);
void HID_MOUSE_ButtonPressed (uint8_t button_idx);
void USR_MOUSE_ProcessData (HID_MOUSE_Info_TypeDef *data);
void USR_KEYBRD_ProcessData (uint8_t data);

extern "C" HCD_HandleTypeDef hhcd;
extern "C" void OTG_FS_IRQHandler (void) { HAL_HCD_IRQHandler (&hhcd); }

static void USBH_MouseDemo (USBH_HandleTypeDef *phost)
{
        HID_MOUSE_Info_TypeDef *m_pinfo;

        m_pinfo = USBH_HID_GetMouseInfo (phost);
        if (m_pinfo != NULL) {
                /* Handle Mouse data position */
                USR_MOUSE_ProcessData (&mouse_info);

                if (m_pinfo->buttons[0]) {
                        HID_MOUSE_ButtonPressed (0);
                }
                else {
                        HID_MOUSE_ButtonReleased (0);
                }

                if (m_pinfo->buttons[1]) {
                        HID_MOUSE_ButtonPressed (2);
                }
                else {
                        HID_MOUSE_ButtonReleased (2);
                }

                if (m_pinfo->buttons[2]) {
                        HID_MOUSE_ButtonPressed (1);
                }
                else {
                        HID_MOUSE_ButtonReleased (1);
                }
        }
}

/**
 * @brief  Processes Keyboard data.
 * @param  data: Keyboard data to be displayed
 * @retval None
 */
void USR_KEYBRD_ProcessData (uint8_t data) { debug->print (data); }

/**
 * @brief  Main routine for Keyboard application
 * @param  phost: Host handle
 * @retval None
 */
static void USBH_KeybdDemo (USBH_HandleTypeDef *phost)
{
        HID_KEYBD_Info_TypeDef *k_pinfo;
        char c;
        k_pinfo = USBH_HID_GetKeybdInfo (phost);

        if (k_pinfo != NULL) {
                c = USBH_HID_GetASCIICode (k_pinfo);
                if (c != 0) {
                        USR_KEYBRD_ProcessData (c);
                }
        }
}

void USBH_UserProcess (USBH_HandleTypeDef *phost, uint8_t id)
{
        switch (id) {
        case HOST_USER_SELECT_CONFIGURATION:
                break;

        case HOST_USER_DISCONNECTION:
                Appli_state = APPLICATION_DISCONNECT;
                break;

        case HOST_USER_CLASS_ACTIVE:
                Appli_state = APPLICATION_READY;
                break;

        case HOST_USER_CONNECTION:
                Appli_state = APPLICATION_START;
                break;

        default:
                break;
        }
}

Usb::Usb ()
{
        /* Init Host Library */
        USBH_Init (&hUSBHost, USBH_UserProcess, 0);

        /* Add Supported Class */
        USBH_RegisterClass (&hUSBHost, USBH_HID_CLASS);

        /* Start Host Process */
        USBH_Start (&hUSBHost);

        /*--------------------------------------------------------------------------*/
        // once

        if (USBH_HID_GetDeviceType (&hUSBHost) == HID_KEYBOARD) {
                hid_demo.keyboard_state = HID_KEYBOARD_IDLE;
                hid_demo.state = HID_DEMO_KEYBOARD;
        }
        else if (USBH_HID_GetDeviceType (&hUSBHost) == HID_MOUSE) {
                hid_demo.mouse_state = HID_MOUSE_IDLE;
                hid_demo.state = HID_DEMO_MOUSE;
        }
        else {
                // LCD_ErrLog ("No supported HID device!\n");
                hid_demo.state = HID_DEMO_WAIT;
        }

        /*--------------------------------------------------------------------------*/
        // once

        /* Force HID Device to re-enumerate */
        USBH_ReEnumerate (&hUSBHost);
        hid_demo.state = HID_DEMO_WAIT;

        /*--------------------------------------------------------------------------*/

        // demo run (loop)

        // if (Appli_state == APPLICATION_READY) {
        //         // HID_MouseMenuProcess ();
        //         USBH_MouseDemo (&hUSBHost);
        // }

        // if (Appli_state == APPLICATION_READY) {
        //         // HID_KeyboardMenuProcess ();
        //         USBH_KeybdDemo (&hUSBHost);
        // }
}

/****************************************************************************/

void Usb::run ()
{
        /* USB Host Background task */
        USBH_Process (&hUSBHost);

        // USBH_KeybdDemo (&hUSBHost);
        USBH_MouseDemo (&hUSBHost);
}

/****************************************************************************/
/* Keyboard                                                                 */
/****************************************************************************/

// void HID_KeyboardMenuProcess (void)
// {
//         switch (hid_demo.keyboard_state) {
//         case HID_KEYBOARD_IDLE:
//                 hid_demo.keyboard_state = HID_KEYBOARD_START;
//                 HID_SelectItem (DEMO_KEYBOARD_menu, 0);
//                 hid_demo.select = 0;
//                 prev_select = 0;
//                 break;

//         case HID_KEYBOARD_WAIT:
//                 if (hid_demo.select != prev_select) {
//                         prev_select = hid_demo.select;
//                         HID_SelectItem (DEMO_KEYBOARD_menu, hid_demo.select & 0x7F);
//                         /* Handle select item */
//                         if (hid_demo.select & 0x80) {
//                                 hid_demo.select &= 0x7F;
//                                 switch (hid_demo.select) {
//                                 case 0:
//                                         hid_demo.keyboard_state = HID_KEYBOARD_START;
//                                         break;

//                                 case 1: /* Return */
//                                         LCD_LOG_ClearTextZone ();
//                                         hid_demo.state = HID_DEMO_REENUMERATE;
//                                         hid_demo.select = 0;
//                                         break;

//                                 default:
//                                         break;
//                                 }
//                         }
//                 }
//                 break;

//         case HID_KEYBOARD_START:
//                 USR_KEYBRD_Init ();
//                 hid_demo.keyboard_state = HID_KEYBOARD_WAIT;
//                 break;

//         default:
//                 break;
//         }
// }

/**
 * @brief  Init Keyboard window.
 * @param  None
 * @retval None
 */
// static void USR_KEYBRD_Init (void)
// {
//         LCD_LOG_ClearTextZone ();
//         BSP_LCD_SetTextColor (LCD_COLOR_YELLOW);

//         BSP_LCD_DisplayStringAtLine (4,
//                                      (uint8_t *)"Use Keyboard to tape characters: ");
//         BSP_LCD_SetTextColor (LCD_COLOR_WHITE);

//         KeybrdCharXpos = KYBRD_FIRST_LINE;
//         KeybrdCharYpos = KYBRD_FIRST_COLUMN;
// }

/****************************************************************************/
/* Mouse                                                                    */
/****************************************************************************/

/* Left Button Report data */
#define HID_MOUSE_BUTTON1 0x01
/* Right Button Report data */
#define HID_MOUSE_BUTTON2 0x02
/* Middle Button Report data */
#define HID_MOUSE_BUTTON3 0x04

/* Mouse directions */
#define MOUSE_TOP_DIR 0x80
#define MOUSE_BOTTOM_DIR 0x00
#define MOUSE_LEFT_DIR 0x80
#define MOUSE_RIGHT_DIR 0x00

#define MOUSE_WINDOW_X 80
#define MOUSE_WINDOW_Y 70
#define MOUSE_WINDOW_X_MAX 181
#define MOUSE_WINDOW_Y_MIN 101
#define MOUSE_WINDOW_HEIGHT 70
#define MOUSE_WINDOW_WIDTH 145

#define HID_MOUSE_BUTTON_HEIGHT 10
#define HID_MOUSE_BUTTON_WIDTH 24
#define HID_MOUSE_BUTTON1_XCHORD 80
#define HID_MOUSE_BUTTON2_XCHORD 140
#define HID_MOUSE_BUTTON3_XCHORD 200
#define HID_MOUSE_BUTTON_YCHORD 150

#define MOUSE_LEFT_MOVE 1
#define MOUSE_RIGHT_MOVE 2
#define MOUSE_UP_MOVE 3
#define MOUSE_DOWN_MOVE 4

#define HID_MOUSE_HEIGHTLSB 2
#define HID_MOUSE_WIDTHLSB 2
#define HID_MOUSE_RES_X 4
#define HID_MOUSE_RES_Y 4

/**
 * @brief  Manages Mouse Menu Process.
 * @param  None
 * @retval None
 */
void HID_MouseMenuProcess (void)
{
        // switch (hid_demo.mouse_state) {
        // case HID_MOUSE_IDLE:
        //         hid_demo.mouse_state = HID_MOUSE_START;
        //         HID_SelectItem (DEMO_MOUSE_menu, 0);
        //         hid_demo.select = 0;
        //         prev_select = 0;
        //         break;

        // case HID_MOUSE_WAIT:
        //         if (hid_demo.select != prev_select) {
        //                 prev_select = hid_demo.select;
        //                 HID_SelectItem (DEMO_MOUSE_menu, hid_demo.select & 0x7F);

        //                 /* Handle select item */
        //                 if (hid_demo.select & 0x80) {
        //                         hid_demo.select &= 0x7F;
        //                         switch (hid_demo.select) {
        //                         case 0:
        //                                 hid_demo.mouse_state = HID_MOUSE_START;
        //                                 break;

        //                         case 1: /* Return */
        //                                 LCD_LOG_ClearTextZone ();
        //                                 hid_demo.state = HID_DEMO_REENUMERATE;
        //                                 hid_demo.select = 0;
        //                                 break;

        //                         default:
        //                                 break;
        //                         }
        //                 }
        //         }
        //         break;

        // case HID_MOUSE_START:
        //         USR_MOUSE_Init ();
        //         hid_demo.mouse_state = HID_MOUSE_WAIT;
        //         HID_MOUSE_UpdatePosition (0, 0);
        //         break;

        // default:
        //         break;
        // }
}

/**
 * @brief  Init Mouse window.
 * @param  None
 * @retval None
 */
static void USR_MOUSE_Init (void) {}

/**
 * @brief  Handles mouse scroll to update the mouse position on display window.
 * @param  x: USB HID Mouse X co-ordinate
 * @param  y: USB HID Mouse Y co-ordinate
 * @retval None
 */
static void HID_MOUSE_UpdatePosition (int8_t x, int8_t y)
{
        debug->print ("x = ");
        debug->println (x);
        debug->print ("y = ");
        debug->println (y);
}

/**
 * @brief  Processes Mouse data.
 * @param  data: Mouse data to be displayed
 * @retval None
 */
void USR_MOUSE_ProcessData (HID_MOUSE_Info_TypeDef *data)
{
        if ((data->x != 0) && (data->y != 0)) {
                HID_MOUSE_UpdatePosition (data->x, data->y);
        }
}

/**
 * @brief  Handles mouse button press.
 * @param  button_idx: Mouse button pressed
 * @retval None
 */
void HID_MOUSE_ButtonPressed (uint8_t button)
{
        debug->print ("Button pressed ");
        debug->println (button);
}

/**
 * @brief  Handles mouse button release.
 * @param  button_idx: Mouse button released
 * @retval None
 */
void HID_MOUSE_ButtonReleased (uint8_t button)
{
        debug->print ("Button released ");
        debug->println (button);
}

} // namespace le::usb