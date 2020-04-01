/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "usbh_hid_gamepad.h"
#include "usbh_hid_parser.h"

/****************************************************************************/

#define HORIZONTAL_AXIS_OFFSET 3
#define VERTICAL_AXIS_OFFSET 4
#define BUTTON_BANK1_OFFSET 5
#define BUTTON_BANK2_OFFSET 6

static HidGamepadInfo gamepadInfo;
uint8_t gamepad_report_data[8];

/****************************************************************************/

USBH_StatusTypeDef usbhHidGamepadInit (USBH_HandleTypeDef *phost)
{
        HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *)phost->pActiveClass->pData;

        memset (gamepad_report_data, 0, sizeof (gamepad_report_data));

        if (HID_Handle->length > (sizeof (gamepad_report_data) / sizeof (uint32_t))) {
                HID_Handle->length = (sizeof (gamepad_report_data) / sizeof (uint32_t));
        }

        HID_Handle->pData = gamepad_report_data;
        fifo_init (&HID_Handle->fifo, phost->device.Data, HID_QUEUE_SIZE * sizeof (gamepad_report_data));

        return USBH_OK;
}

/****************************************************************************/

HidGamepadInfo *usbhHidGetGamepadInfo (USBH_HandleTypeDef *phost)
{
        HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *)phost->pActiveClass->pData;

        if (HID_Handle->length == 0) {
                return NULL;
        }

        static int lastX, lastY;

        if (fifo_read (&HID_Handle->fifo, &gamepad_report_data, HID_Handle->length) == HID_Handle->length) {
                gamepadInfo.x = ((int16_t)gamepad_report_data[HORIZONTAL_AXIS_OFFSET]) - 127;
                gamepadInfo.y = ((int16_t)gamepad_report_data[VERTICAL_AXIS_OFFSET]) - 127;
                gamepadInfo.buttons = *(uint16_t *)&gamepad_report_data[BUTTON_BANK1_OFFSET];
                return &gamepadInfo;
        }

        // if (gamepadInfo.x != lastX) {
        //         printf ("X:%d ", gamepadInfo.x);
        // }
        // if (gamepadInfo.y != lastY) {
        //         printf ("Y:%d ", gamepadInfo.y);
        // }

        // lastX = gamepadInfo.x;
        // lastY = gamepadInfo.y;

        return NULL;
}
