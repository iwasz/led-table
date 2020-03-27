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

static USBH_StatusTypeDef USBH_HID_KeybdDecode (USBH_HandleTypeDef *phost);

/****************************************************************************/

HidGamepadInfo keybd_info;
uint32_t keybd_report_data[2];

/****************************************************************************/

static const HID_Report_ItemTypedef imp_0_lctrl = {
        (uint8_t *)keybd_report_data + 0, /*data*/
        1,                                /*size*/
        0,                                /*shift*/
        0,                                /*count (only for array items)*/
        0,                                /*signed?*/
        0,                                /*min value read can return*/
        1,                                /*max value read can return*/
        0,                                /*min vale device can report*/
        1,                                /*max value device can report*/
        1                                 /*resolution*/
};
static const HID_Report_ItemTypedef imp_0_lshift = {
        (uint8_t *)keybd_report_data + 0, /*data*/
        1,                                /*size*/
        1,                                /*shift*/
        0,                                /*count (only for array items)*/
        0,                                /*signed?*/
        0,                                /*min value read can return*/
        1,                                /*max value read can return*/
        0,                                /*min vale device can report*/
        1,                                /*max value device can report*/
        1                                 /*resolution*/
};
static const HID_Report_ItemTypedef imp_0_lalt = {
        (uint8_t *)keybd_report_data + 0, /*data*/
        1,                                /*size*/
        2,                                /*shift*/
        0,                                /*count (only for array items)*/
        0,                                /*signed?*/
        0,                                /*min value read can return*/
        1,                                /*max value read can return*/
        0,                                /*min vale device can report*/
        1,                                /*max value device can report*/
        1                                 /*resolution*/
};
static const HID_Report_ItemTypedef imp_0_lgui = {
        (uint8_t *)keybd_report_data + 0, /*data*/
        1,                                /*size*/
        3,                                /*shift*/
        0,                                /*count (only for array items)*/
        0,                                /*signed?*/
        0,                                /*min value read can return*/
        1,                                /*max value read can return*/
        0,                                /*min vale device can report*/
        1,                                /*max value device can report*/
        1                                 /*resolution*/
};
static const HID_Report_ItemTypedef imp_0_rctrl = {
        (uint8_t *)keybd_report_data + 0, /*data*/
        1,                                /*size*/
        4,                                /*shift*/
        0,                                /*count (only for array items)*/
        0,                                /*signed?*/
        0,                                /*min value read can return*/
        1,                                /*max value read can return*/
        0,                                /*min vale device can report*/
        1,                                /*max value device can report*/
        1                                 /*resolution*/
};
static const HID_Report_ItemTypedef imp_0_rshift = {
        (uint8_t *)keybd_report_data + 0, /*data*/
        1,                                /*size*/
        5,                                /*shift*/
        0,                                /*count (only for array items)*/
        0,                                /*signed?*/
        0,                                /*min value read can return*/
        1,                                /*max value read can return*/
        0,                                /*min vale device can report*/
        1,                                /*max value device can report*/
        1                                 /*resolution*/
};
static const HID_Report_ItemTypedef imp_0_ralt = {
        (uint8_t *)keybd_report_data + 0, /*data*/
        1,                                /*size*/
        6,                                /*shift*/
        0,                                /*count (only for array items)*/
        0,                                /*signed?*/
        0,                                /*min value read can return*/
        1,                                /*max value read can return*/
        0,                                /*min vale device can report*/
        1,                                /*max value device can report*/
        1                                 /*resolution*/
};
static const HID_Report_ItemTypedef imp_0_rgui = {
        (uint8_t *)keybd_report_data + 0, /*data*/
        1,                                /*size*/
        7,                                /*shift*/
        0,                                /*count (only for array items)*/
        0,                                /*signed?*/
        0,                                /*min value read can return*/
        1,                                /*max value read can return*/
        0,                                /*min vale device can report*/
        1,                                /*max value device can report*/
        1                                 /*resolution*/
};

static const HID_Report_ItemTypedef imp_0_key_array = {
        (uint8_t *)keybd_report_data + 2, /*data*/
        8,                                /*size*/
        0,                                /*shift*/
        6,                                /*count (only for array items)*/
        0,                                /*signed?*/
        0,                                /*min value read can return*/
        101,                              /*max value read can return*/
        0,                                /*min vale device can report*/
        101,                              /*max value device can report*/
        1                                 /*resolution*/
};

#ifdef QWERTY_KEYBOARD
static const int8_t HID_KEYBRD_Key[]
        = {'\0', '`',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=',  '\0', '\r', '\t', 'q',  'w',  'e',  'r',  't',
           'y',  'u',  'i',  'o',  'p',  '[',  ']',  '\\', '\0', 'a',  's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '\'', '\0', '\n',
           '\0', '\0', 'z',  'x',  'c',  'v',  'b',  'n',  'm',  ',',  '.',  '/',  '\0', '\0', '\0', '\0', '\0', ' ',  '\0', '\0', '\0', '\0',
           '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\r', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
           '\0', '\0', '\0', '7',  '4',  '1',  '\0', '/',  '8',  '5',  '2',  '0',  '*',  '9',  '6',  '3',  '.',  '-',  '+',  '\0', '\n', '\0',
           '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

static const int8_t HID_KEYBRD_ShiftKey[]
        = {'\0', '~',  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',  '_',  '+',  '\0', '\0', '\0', 'Q',  'W',  'E',  'R',  'T',
           'Y',  'U',  'I',  'O',  'P',  '{',  '}',  '|',  '\0', 'A',  'S',  'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',  '"',  '\0', '\n',
           '\0', '\0', 'Z',  'X',  'C',  'V',  'B',  'N',  'M',  '<',  '>',  '?',  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
           '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
           '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
           '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

#else

static const int8_t HID_KEYBRD_Key[]
        = {'\0', '`',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=',  '\0', '\r', '\t', 'a',  'z',  'e',  'r',  't',
           'y',  'u',  'i',  'o',  'p',  '[',  ']',  '\\', '\0', 'q',  's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  'm',  '\0', '\0', '\n',
           '\0', '\0', 'w',  'x',  'c',  'v',  'b',  'n',  ',',  ';',  ':',  '!',  '\0', '\0', '\0', '\0', '\0', ' ',  '\0', '\0', '\0', '\0',
           '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\r', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
           '\0', '\0', '\0', '7',  '4',  '1',  '\0', '/',  '8',  '5',  '2',  '0',  '*',  '9',  '6',  '3',  '.',  '-',  '+',  '\0', '\n', '\0',
           '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

static const int8_t HID_KEYBRD_ShiftKey[]
        = {'\0', '~',  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',  '_',  '+',  '\0', '\0', '\0', 'A',  'Z',  'E',  'R',  'T',
           'Y',  'U',  'I',  'O',  'P',  '{',  '}',  '*',  '\0', 'Q',  'S',  'D',  'F',  'G',  'H',  'J',  'K',  'L',  'M',  '%',  '\0', '\n',
           '\0', '\0', 'W',  'X',  'C',  'V',  'B',  'N',  '?',  '.',  '/',  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
           '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
           '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
           '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
#endif

static const uint8_t HID_KEYBRD_Codes[] = {
        0,   0,   0,   0,   31,  50,  48,  33,  19,  34,  35,  36,  24,  37,  38,  39,  /* 0x00 - 0x0F */
        52,  51,  25,  26,  17,  20,  32,  21,  23,  49,  18,  47,  22,  46,  2,   3,   /* 0x10 - 0x1F */
        4,   5,   6,   7,   8,   9,   10,  11,  43,  110, 15,  16,  61,  12,  13,  27,  /* 0x20 - 0x2F */
        28,  29,  42,  40,  41,  1,   53,  54,  55,  30,  112, 113, 114, 115, 116, 117, /* 0x30 - 0x3F */
        118, 119, 120, 121, 122, 123, 124, 125, 126, 75,  80,  85,  76,  81,  86,  89,  /* 0x40 - 0x4F */
        79,  84,  83,  90,  95,  100, 105, 106, 108, 93,  98,  103, 92,  97,  102, 91,  /* 0x50 - 0x5F */
        96,  101, 99,  104, 45,  129, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   /* 0x60 - 0x6F */
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   /* 0x70 - 0x7F */
        0,   0,   0,   0,   0,   107, 0,   56,  0,   0,   0,   0,   0,   0,   0,   0,   /* 0x80 - 0x8F */
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   /* 0x90 - 0x9F */
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   /* 0xA0 - 0xAF */
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   /* 0xB0 - 0xBF */
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   /* 0xC0 - 0xCF */
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   /* 0xD0 - 0xDF */
        58,  44,  60,  127, 64,  57,  62,  128                                          /* 0xE0 - 0xE7 */
};

/**
 * @brief  USBH_HID_KeybdInit
 *         The function init the HID keyboard.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef usbhHidGamepadInit (USBH_HandleTypeDef *phost)
{
        uint32_t x;
        HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *)phost->pActiveClass->pData;

        keybd_info.lctrl = keybd_info.lshift = 0;
        keybd_info.lalt = keybd_info.lgui = 0;
        keybd_info.rctrl = keybd_info.rshift = 0;
        keybd_info.ralt = keybd_info.rgui = 0;

        for (x = 0; x < (sizeof (keybd_report_data) / sizeof (uint32_t)); x++) {
                keybd_report_data[x] = 0;
        }

        if (HID_Handle->length > (sizeof (keybd_report_data) / sizeof (uint32_t))) {
                HID_Handle->length = (sizeof (keybd_report_data) / sizeof (uint32_t));
        }
        HID_Handle->pData = (uint8_t *)keybd_report_data;
        fifo_init (&HID_Handle->fifo, phost->device.Data, HID_QUEUE_SIZE * sizeof (keybd_report_data));

        return USBH_OK;
}

/**
 * @brief  USBH_HID_GetKeybdInfo
 *         The function return keyboard information.
 * @param  phost: Host handle
 * @retval keyboard information
 */
HidGamepadInfo *usbhHidGetGamepadInfo (USBH_HandleTypeDef *phost)
{
        if (USBH_HID_KeybdDecode (phost) == USBH_OK) {
                return &keybd_info;
        }
        else {
                return NULL;
        }
}

/**
 * @brief  USBH_HID_KeybdDecode
 *         The function decode keyboard data.
 * @param  phost: Host handle
 * @retval USBH Status
 */
static USBH_StatusTypeDef USBH_HID_KeybdDecode (USBH_HandleTypeDef *phost)
{
        uint8_t x;

        HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *)phost->pActiveClass->pData;
        if (HID_Handle->length == 0) {
                return USBH_FAIL;
        }
        /*Fill report */
        if (fifo_read (&HID_Handle->fifo, &keybd_report_data, HID_Handle->length) == HID_Handle->length) {

                keybd_info.lctrl = (uint8_t)HID_ReadItem ((HID_Report_ItemTypedef *)&imp_0_lctrl, 0);
                keybd_info.lshift = (uint8_t)HID_ReadItem ((HID_Report_ItemTypedef *)&imp_0_lshift, 0);
                keybd_info.lalt = (uint8_t)HID_ReadItem ((HID_Report_ItemTypedef *)&imp_0_lalt, 0);
                keybd_info.lgui = (uint8_t)HID_ReadItem ((HID_Report_ItemTypedef *)&imp_0_lgui, 0);
                keybd_info.rctrl = (uint8_t)HID_ReadItem ((HID_Report_ItemTypedef *)&imp_0_rctrl, 0);
                keybd_info.rshift = (uint8_t)HID_ReadItem ((HID_Report_ItemTypedef *)&imp_0_rshift, 0);
                keybd_info.ralt = (uint8_t)HID_ReadItem ((HID_Report_ItemTypedef *)&imp_0_ralt, 0);
                keybd_info.rgui = (uint8_t)HID_ReadItem ((HID_Report_ItemTypedef *)&imp_0_rgui, 0);

                for (x = 0; x < sizeof (keybd_info.keys); x++) {
                        keybd_info.keys[x] = (uint8_t)HID_ReadItem ((HID_Report_ItemTypedef *)&imp_0_key_array, x);
                }

                return USBH_OK;
        }
        return USBH_FAIL;
}
