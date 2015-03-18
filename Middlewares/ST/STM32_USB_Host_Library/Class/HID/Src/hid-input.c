/*
 *  Copyright (c) 2000-2001 Vojtech Pavlik
 *  Copyright (c) 2006-2010 Jiri Kosina
 *
 *  HID to Linux Input mapping
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@ucw.cz>, or by paper mail:
 * Vojtech Pavlik, Simunkova 1594, Prague 8, 182 00 Czech Republic
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "non-atomic.h"
#include "uinput.h"
#include "kinput.h"
#include "hid.h"
#include "usbh_conf.h"


/**
 * __ffs - find first bit in word.
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
static inline unsigned long __ffs(unsigned long word)
{
  int num = 0;

#if BITS_PER_LONG == 64
  if ((word & 0xffffffff) == 0)
  {
    num += 32;
    word >>= 32;
  }
#endif
  if ((word & 0xffff) == 0)
  {
    num += 16;
    word >>= 16;
  }
  if ((word & 0xff) == 0)
  {
    num += 8;
    word >>= 8;
  }
  if ((word & 0xf) == 0)
  {
    num += 4;
    word >>= 4;
  }
  if ((word & 0x3) == 0)
  {
    num += 2;
    word >>= 2;
  }
  if ((word & 0x1) == 0)
    num += 1;
  return num;
}

#define BITOP_WORD(nr)          ((nr) / BITS_PER_LONG)
#define ffz(x)  __ffs(~(x))

unsigned long find_next_zero_bit(const unsigned long *addr, unsigned long size, unsigned long offset)
{
  const unsigned long *p = addr + BITOP_WORD(offset);
  unsigned long result = offset & ~(BITS_PER_LONG - 1);
  unsigned long tmp;

  if (offset >= size)
    return size;
  size -= result;
  offset %= BITS_PER_LONG;
  if (offset)
  {
    tmp = *(p++);
    tmp |= ~0UL >> (BITS_PER_LONG - offset);
    if (size < BITS_PER_LONG)
      goto found_first;
    if (~tmp)
      goto found_middle;
    size -= BITS_PER_LONG;
    result += BITS_PER_LONG;
  }
  while (size & ~(BITS_PER_LONG - 1))
  {
    if (~(tmp = *(p++)))
      goto found_middle;
    result += BITS_PER_LONG;
    size -= BITS_PER_LONG;
  }
  if (!size)
    return result;
  tmp = *p;

  found_first: tmp |= ~0UL << size;
  if (tmp == ~0UL) /* Are any bits zero? */
    return result + size; /* Nope. */
  found_middle: return result + ffz(tmp);
}


void input_event (struct input_dev * dev, unsigned int type, unsigned int code, int value);

#define unk	KEY_UNKNOWN

/*
 * This table differs from HID AT-101 position reference.
 * This table translate HID HUT definition to linux keycodes, which
 * are defined in input.h (uapi)
 */
static const unsigned char hid_keyboard[256] = {
	  0,  0,  0,  0, 30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38,
	 50, 49, 24, 25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44,  2,  3,
	  4,  5,  6,  7,  8,  9, 10, 11, 28,  1, 14, 15, 57, 12, 13, 26,
	 27, 43, 43, 39, 40, 41, 51, 52, 53, 58, 59, 60, 61, 62, 63, 64,
	 65, 66, 67, 68, 87, 88, 99, 70,119,110,102,104,111,107,109,106,
	105,108,103, 69, 98, 55, 74, 78, 96, 79, 80, 81, 75, 76, 77, 71,
	 72, 73, 82, 83, 86,127,116,117,183,184,185,186,187,188,189,190,
	191,192,193,194,134,138,130,132,128,129,131,137,133,135,136,113,
	115,114,unk,unk,unk,121,unk, 89, 93,124, 92, 94, 95,unk,unk,unk,
	122,123, 90, 91, 85,unk,unk,unk,unk,unk,unk,unk,111,unk,unk,unk,
	unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,
	unk,unk,unk,unk,unk,unk,179,180,unk,unk,unk,unk,unk,unk,unk,unk,
	unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,
	unk,unk,unk,unk,unk,unk,unk,unk,111,unk,unk,unk,unk,unk,unk,unk,
	 29, 42, 56,125, 97, 54,100,126,164,166,165,163,161,115,114,113,
	150,158,159,128,136,177,178,176,142,152,173,140,unk,unk,unk,unk
};

/*
 * This string array is used for debug/print.
 */
static const char* event_type_str[] = {
  "EV_SYN",               // 0x00
  "EV_KEY",               // 0x01
  "EV_REL",               // 0x02
  "EV_ABS",               // 0x03
  "EV_MSC",               // 0x04
  "EV_SW",                // 0x05
  "EV_UNDEFINED",         // 0x06
  "EV_UNDEFINED",         // 0x07
  "EV_UNDEFINED",         // 0x08
  "EV_UNDEFINED",         // 0x09
  "EV_UNDEFINED",         // 0x0A
  "EV_UNDEFINED",         // 0x0B
  "EV_UNDEFINED",         // 0x0C
  "EV_UNDEFINED",         // 0x0D
  "EV_UNDEFINED",         // 0x0E
  "EV_UNDEFINED",         // 0x0F
  "EV_UNDEFINED",         // 0x10
  "EV_LED",               // 0x11
  "EV_SND",               // 0x12
  "EV_REP",               // 0x14
  "EV_FF",                // 0x15
  "EV_PWR",               // 0x16
  "EV_FF_STATUS",         // 0x17
  "EV_MAX",               // 0x1f
};

/*
 * This string array is used for debug/print.
 */
static const char* keycode_str[256] = {

  "KEY_RESERVED",             // #define KEY_RESERVED         0
  "KEY_ESC",                  // #define KEY_ESC              1
  "KEY_1",                    // #define KEY_1                2
  "KEY_2",                    // #define KEY_2                3
  "KEY_3",                    // #define KEY_3                4
  "KEY_4",                    // #define KEY_4                5
  "KEY_5",                    // #define KEY_5                6
  "KEY_6",                    // #define KEY_6                7
  "KEY_7",                    // #define KEY_7                8
  "KEY_8",                    // #define KEY_8                9
  "KEY_9",                    // #define KEY_9                10
  "KEY_0",                    // #define KEY_0                11
  "KEY_MINUS",                // #define KEY_MINUS            12
  "KEY_EQUAL",                // #define KEY_EQUAL            13
  "KEY_BACKSPACE",            // #define KEY_BACKSPACE        14
  "KEY_TAB",                  // #define KEY_TAB              15
  "KEY_Q",                    // #define KEY_Q                16
  "KEY_W",                    // #define KEY_W                17
  "KEY_E",                    // #define KEY_E                18
  "KEY_R",                    // #define KEY_R                19
  "KEY_T",                    // #define KEY_T                20
  "KEY_Y",                    // #define KEY_Y                21
  "KEY_U",                    // #define KEY_U                22
  "KEY_I",                    // #define KEY_I                23
  "KEY_O",                    // #define KEY_O                24
  "KEY_P",                    // #define KEY_P                25
  "KEY_LEFTBRACE",            // #define KEY_LEFTBRACE        26
  "KEY_RIGHTBRACE",           // #define KEY_RIGHTBRACE       27
  "KEY_ENTER",                // #define KEY_ENTER            28
  "KEY_LEFTCTRL",             // #define KEY_LEFTCTRL         29
  "KEY_A",                    // #define KEY_A                30
  "KEY_S",                    // #define KEY_S                31
  "KEY_D",                    // #define KEY_D                32
  "KEY_F",                    // #define KEY_F                33
  "KEY_G",                    // #define KEY_G                34
  "KEY_H",                    // #define KEY_H                35
  "KEY_J",                    // #define KEY_J                36
  "KEY_K",                    // #define KEY_K                37
  "KEY_L",                    // #define KEY_L                38
  "KEY_SEMICOLON",            // #define KEY_SEMICOLON        39
  "KEY_APOSTROPHE",           // #define KEY_APOSTROPHE       40
  "KEY_GRAVE",                // #define KEY_GRAVE            41
  "KEY_LEFTSHIFT",            // #define KEY_LEFTSHIFT        42
  "KEY_BACKSLASH",            // #define KEY_BACKSLASH        43
  "KEY_Z",                    // #define KEY_Z                44
  "KEY_X",                    // #define KEY_X                45
  "KEY_C",                    // #define KEY_C                46
  "KEY_V",                    // #define KEY_V                47
  "KEY_B",                    // #define KEY_B                48
  "KEY_N",                    // #define KEY_N                49
  "KEY_M",                    // #define KEY_M                50
  "KEY_COMMA",                // #define KEY_COMMA            51
  "KEY_DOT",                  // #define KEY_DOT              52
  "KEY_SLASH",                // #define KEY_SLASH            53
  "KEY_RIGHTSHIFT",           // #define KEY_RIGHTSHIFT       54
  "KEY_KPASTERISK",           // #define KEY_KPASTERISK       55
  "KEY_LEFTALT",              // #define KEY_LEFTALT          56
  "KEY_SPACE",                // #define KEY_SPACE            57
  "KEY_CAPSLOCK",             // #define KEY_CAPSLOCK         58
  "KEY_F1",                   // #define KEY_F1               59
  "KEY_F2",                   // #define KEY_F2               60
  "KEY_F3",                   // #define KEY_F3               61
  "KEY_F4",                   // #define KEY_F4               62
  "KEY_F5",                   // #define KEY_F5               63
  "KEY_F6",                   // #define KEY_F6               64
  "KEY_F7",                   // #define KEY_F7               65
  "KEY_F8",                   // #define KEY_F8               66
  "KEY_F9",                   // #define KEY_F9               67
  "KEY_F10",                  // #define KEY_F10              68
  "KEY_NUMLOCK",              // #define KEY_NUMLOCK          69
  "KEY_SCROLLLOCK",           // #define KEY_SCROLLLOCK       70
  "KEY_KP7",                  // #define KEY_KP7              71
  "KEY_KP8",                  // #define KEY_KP8              72
  "KEY_KP9",                  // #define KEY_KP9              73
  "KEY_KPMINUS",              // #define KEY_KPMINUS          74
  "KEY_KP4",                  // #define KEY_KP4              75
  "KEY_KP5",                  // #define KEY_KP5              76
  "KEY_KP6",                  // #define KEY_KP6              77
  "KEY_KPPLUS",               // #define KEY_KPPLUS           78
  "KEY_KP1",                  // #define KEY_KP1              79
  "KEY_KP2",                  // #define KEY_KP2              80
  "KEY_KP3",                  // #define KEY_KP3              81
  "KEY_KP0",                  // #define KEY_KP0              82
  "KEY_KPDOT",                // #define KEY_KPDOT            83
  "UNDEFINED",                //                              84
  "KEY_ZENKAKUHANKAKU",       // #define KEY_ZENKAKUHANKAKU   85
  "KEY_102ND",                // #define KEY_102ND            86
  "KEY_F11",                  // #define KEY_F11              87
  "KEY_F12",                  // #define KEY_F12              88
  "KEY_RO",                   // #define KEY_RO               89
  "KEY_KATAKANA",             // #define KEY_KATAKANA         90
  "KEY_HIRAGANA",             // #define KEY_HIRAGANA         91
  "KEY_HENKAN",               // #define KEY_HENKAN           92
  "KEY_KATAKANAHIRAGANA",     // #define KEY_KATAKANAHIRAGANA 93
  "KEY_MUHENKAN",             // #define KEY_MUHENKAN         94
  "KEY_KPJPCOMMA",            // #define KEY_KPJPCOMMA        95
  "KEY_KPENTER",              // #define KEY_KPENTER          96
  "KEY_RIGHTCTRL",            // #define KEY_RIGHTCTRL        97
  "KEY_KPSLASH",              // #define KEY_KPSLASH          98
  "KEY_SYSRQ",                // #define KEY_SYSRQ            99
  "KEY_RIGHTALT",             // #define KEY_RIGHTALT         100
  "KEY_LINEFEED",             // #define KEY_LINEFEED         101
  "KEY_HOME",                 // #define KEY_HOME             102
  "KEY_UP",                   // #define KEY_UP               103
  "KEY_PAGEUP",               // #define KEY_PAGEUP           104
  "KEY_LEFT",                 // #define KEY_LEFT             105
  "KEY_RIGHT",                // #define KEY_RIGHT            106
  "KEY_END",                  // #define KEY_END              107
  "KEY_DOWN",                 // #define KEY_DOWN             108
  "KEY_PAGEDOWN",             // #define KEY_PAGEDOWN         109
  "KEY_INSERT",               // #define KEY_INSERT           110
  "KEY_DELETE",               // #define KEY_DELETE           111
  "KEY_MACRO",                // #define KEY_MACRO            112
  "KEY_MUTE",                 // #define KEY_MUTE             113
  "KEY_VOLUMEDOWN",           // #define KEY_VOLUMEDOWN       114
  "KEY_VOLUMEUP",             // #define KEY_VOLUMEUP         115
  "KEY_POWER",                // #define KEY_POWER            116 /* SC System Power Down */
  "KEY_KPEQUAL",              // #define KEY_KPEQUAL          117
  "KEY_KPPLUSMINUS",          // #define KEY_KPPLUSMINUS      118
  "KEY_PAUSE",                // #define KEY_PAUSE            119
  "KEY_SCALE",                // #define KEY_SCALE            120 /* AL Compiz Scale (Expose) */
  "KEY_KPCOMMA",              // #define KEY_KPCOMMA          121
  "KEY_HANGEUL",              // #define KEY_HANGEUL          122
                              // #define KEY_HANGUEL          KEY_HANGEUL
  "KEY_HANJA",                // #define KEY_HANJA            123
  "KEY_YEN",                  // #define KEY_YEN              124
  "KEY_LEFTMETA",             // #define KEY_LEFTMETA         125
  "KEY_RIGHTMETA",            // #define KEY_RIGHTMETA        126
  "KEY_COMPOSE",              // #define KEY_COMPOSE          127
  "KEY_STOP",                 // #define KEY_STOP             128 /* AC Stop */
  "KEY_AGAIN",                // #define KEY_AGAIN            129
  "KEY_PROPS",                // #define KEY_PROPS            130 /* AC Properties */
  "KEY_UNDO",                 // #define KEY_UNDO             131 /* AC Undo */
  "KEY_FRONT",                // #define KEY_FRONT            132
  "KEY_COPY",                 // #define KEY_COPY             133 /* AC Copy */
  "KEY_OPEN",                 // #define KEY_OPEN             134 /* AC Open */
  "KEY_PASTE",                // #define KEY_PASTE            135 /* AC Paste */
  "KEY_FIND",                 // #define KEY_FIND             136 /* AC Search */
  "KEY_CUT",                  // #define KEY_CUT              137 /* AC Cut */
  "KEY_HELP",                 // #define KEY_HELP             138 /* AL Integrated Help Center */
  "KEY_MENU",                 // #define KEY_MENU             139 /* Menu (show menu) */
  "KEY_CALC",                 // #define KEY_CALC             140 /* AL Calculator */
  "KEY_SETUP",                // #define KEY_SETUP            141
  "KEY_SLEEP",                // #define KEY_SLEEP            142 /* SC System Sleep */
  "KEY_WAKEUP",               // #define KEY_WAKEUP           143 /* System Wake Up */
  "KEY_FILE",                 // #define KEY_FILE             144 /* AL Local Machine Browser */
  "KEY_SENDFILE",             // #define KEY_SENDFILE         145
  "KEY_DELETEFILE",           // #define KEY_DELETEFILE       146
  "KEY_XFER",                 // #define KEY_XFER             147
  "KEY_PROG1",                // #define KEY_PROG1            148
  "KEY_PROG2",                // #define KEY_PROG2            149
  "KEY_WWW",                  // #define KEY_WWW              150 /* AL Internet Browser */
  "KEY_MSDOS",                // #define KEY_MSDOS            151
                              // #define KEY_COFFEE           152 /* AL Terminal Lock/Screensaver */
  "KEY_SCREENLOCK",           // #define KEY_SCREENLOCK       KEY_COFFEE
  "KEY_DIRECTION",            // #define KEY_DIRECTION        153
  "KEY_CYCLEWINDOWS",         // #define KEY_CYCLEWINDOWS     154
  "KEY_MAIL",                 // #define KEY_MAIL             155
  "KEY_BOOKMARKS",            // #define KEY_BOOKMARKS        156 /* AC Bookmarks */
  "KEY_COMPUTER",             // #define KEY_COMPUTER         157
  "KEY_BACK",                 // #define KEY_BACK             158 /* AC Back */
  "KEY_FORWARD",              // #define KEY_FORWARD          159 /* AC Forward */
  "KEY_CLOSECD",              // #define KEY_CLOSECD          160
  "KEY_EJECTCD",              // #define KEY_EJECTCD          161
  "KEY_EJECTCLOSECD",         // #define KEY_EJECTCLOSECD     162
  "KEY_NEXTSONG",             // #define KEY_NEXTSONG         163
  "KEY_PLAYPAUSE",            // #define KEY_PLAYPAUSE        164
  "KEY_PREVIOUSSONG",         // #define KEY_PREVIOUSSONG     165
  "KEY_STOPCD",               // #define KEY_STOPCD           166
  "KEY_RECORD",               // #define KEY_RECORD           167
  "KEY_REWIND",               // #define KEY_REWIND           168
  "KEY_PHONE",                // #define KEY_PHONE            169 /* Media Select Telephone */
  "KEY_ISO",                  // #define KEY_ISO              170
  "KEY_CONFIG",               // #define KEY_CONFIG           171 /* AL Consumer Control Configuration */
  "KEY_HOMEPAGE",             // #define KEY_HOMEPAGE         172 /* AC Home */
  "KEY_REFRESH",              // #define KEY_REFRESH          173 /* AC Refresh */
  "KEY_EXIT",                 // #define KEY_EXIT             174 /* AC Exit */
  "KEY_MOVE",                 // #define KEY_MOVE             175
  "KEY_EDIT",                 // #define KEY_EDIT             176
  "KEY_SCROLLUP",             // #define KEY_SCROLLUP         177
  "KEY_SCROLLDOWN",           // #define KEY_SCROLLDOWN       178
  "KEY_KPLEFTPAREN",          // #define KEY_KPLEFTPAREN      179
  "KEY_KPRIGHTPAREN",         // #define KEY_KPRIGHTPAREN     180
  "KEY_NEW",                  // #define KEY_NEW              181 /* AC New */
  "KEY_REDO",                 // #define KEY_REDO             182 /* AC Redo/Repeat */
  "KEY_F13",                  // #define KEY_F13              183
  "KEY_F14",                  // #define KEY_F14              184
  "KEY_F15",                  // #define KEY_F15              185
  "KEY_F16",                  // #define KEY_F16              186
  "KEY_F17",                  // #define KEY_F17              187
  "KEY_F18",                  // #define KEY_F18              188
  "KEY_F19",                  // #define KEY_F19              189
  "KEY_F20",                  // #define KEY_F20              190
  "KEY_F21",                  // #define KEY_F21              191
  "KEY_F22",                  // #define KEY_F22              192
  "KEY_F23",                  // #define KEY_F23              193
  "KEY_F24",                  // #define KEY_F24              194
  "UNDEFINED",                //                              195
  "UNDEFINED",                //                              196
  "UNDEFINED",                //                              197
  "UNDEFINED",                //                              198
  "UNDEFINED",                //                              199
  "KEY_PLAYCD",               // #define KEY_PLAYCD           200
  "KEY_PAUSECD",              // #define KEY_PAUSECD          201
  "KEY_PROG3",                // #define KEY_PROG3            202
  "KEY_PROG4",                // #define KEY_PROG4            203
  "KEY_DASHBOARD",            // #define KEY_DASHBOARD        204 /* AL Dashboard */
  "KEY_SUSPEND",              // #define KEY_SUSPEND          205
  "KEY_CLOSE",                // #define KEY_CLOSE            206 /* AC Close */
  "KEY_PLAY",                 // #define KEY_PLAY             207
  "KEY_FASTFORWARD",          // #define KEY_FASTFORWARD      208
  "KEY_BASSBOOST",            // #define KEY_BASSBOOST        209
  "KEY_PRINT",                // #define KEY_PRINT            210 /* AC Print */
  "KEY_HP",                   // #define KEY_HP               211
  "KEY_CAMERA",               // #define KEY_CAMERA           212
  "KEY_SOUND",                // #define KEY_SOUND            213
  "KEY_QUESTION",             // #define KEY_QUESTION         214
  "KEY_EMAIL",                // #define KEY_EMAIL            215
  "KEY_CHAT",                 // #define KEY_CHAT             216
  "KEY_SEARCH",               // #define KEY_SEARCH           217
  "KEY_CONNECT",              // #define KEY_CONNECT          218
  "KEY_FINANCE",              // #define KEY_FINANCE          219 /* AL Checkbook/Finance */
  "KEY_SPORT",                // #define KEY_SPORT            220
  "KEY_SHOP",                 // #define KEY_SHOP             221
  "KEY_ALTERASE",             // #define KEY_ALTERASE         222
  "KEY_CANCEL",               // #define KEY_CANCEL           223 /* AC Cancel */
  "KEY_BRIGHTNESSDOWN",       // #define KEY_BRIGHTNESSDOWN   224
  "KEY_BRIGHTNESSUP",         // #define KEY_BRIGHTNESSUP     225
  "KEY_MEDIA",                // #define KEY_MEDIA            226
  "KEY_SWITCHVIDEOMODE",      // #define KEY_SWITCHVIDEOMODE  227 /* Cycle between available video outputs (Monitor/LCD/TV-out/etc) */
  "KEY_KBDILLUMTOGGLE",       // #define KEY_KBDILLUMTOGGLE   228
  "KEY_KBDILLUMDOWN",         // #define KEY_KBDILLUMDOWN     229
  "KEY_KBDILLUMUP",           // #define KEY_KBDILLUMUP       230
  "KEY_SEND",                 // #define KEY_SEND             231 /* AC Send */
  "KEY_REPLY",                // #define KEY_REPLY            232 /* AC Reply */
  "KEY_FORWARDMAIL",          // #define KEY_FORWARDMAIL      233 /* AC Forward Msg */
  "KEY_SAVE",                 // #define KEY_SAVE             234 /* AC Save */
  "KEY_DOCUMENTS",            // #define KEY_DOCUMENTS        235
  "KEY_BATTERY",              // #define KEY_BATTERY          236
  "KEY_BLUETOOTH",            // #define KEY_BLUETOOTH        237
  "KEY_WLAN",                 // #define KEY_WLAN             238
  "KEY_UWB",                  // #define KEY_UWB              239
  "KEY_UNKNOWN",              // #define KEY_UNKNOWN          240
  "KEY_VIDEO_NEXT",           // #define KEY_VIDEO_NEXT       241 /* drive next video source */
  "KEY_VIDEO_PREV",           // #define KEY_VIDEO_PREV       242 /* drive previous video source */
  "KEY_BRIGHTNESS_CYCLE",     // #define KEY_BRIGHTNESS_CYCLE 243 /* brightness up, after max is min */
  "KEY_BRIGHTNESS_ZERO",      // #define KEY_BRIGHTNESS_ZERO  244 /* brightness off, use ambient */
  "KEY_DISPLAY_OFF",          // #define KEY_DISPLAY_OFF      245 /* display device to off state */
  "KEY_WIMAX",                // #define KEY_WIMAX            246
  "KEY_RFKILL",               // #define KEY_RFKILL           247 /* Key that controls all radios */
  "KEY_MICMUTE",              // #define KEY_MICMUTE          248 /* Mute / unmute the microphone */
  "UNDEFINED",                //                              249
  "UNDEFINED",                //                              250
  "UNDEFINED",                //                              251
  "UNDEFINED",                //                              252
  "UNDEFINED",                //                              253
  "UNDEFINED",                //                              254
  "UNDEFINED",                //                              255
};

/*
 * This is for debugging only
 */
void input_event(struct input_dev * dev, unsigned int type, unsigned int code, int value) {

  static int lctrl = 0;
  static int lshift = 0;
  static int lalt = 0;
  static int lmeta = 0;

  static int rctrl = 0;
  static int rshift = 0;
  static int ralt = 0;
  static int rmeta = 0;

  static int i;

  static int keycode[256] = { 0 };

  if (type == EV_KEY) {

    if (code == KEY_LEFTCTRL) {
      lctrl = value;
    }
    else if (code == KEY_LEFTSHIFT) {
      lshift = value;
    }
    else if (code == KEY_LEFTALT) {
      lalt = value;
    }
    else if (code == KEY_LEFTMETA) {
      lmeta = value;
    }
    else if (code == KEY_RIGHTCTRL) {
      rctrl = value;
    }
    else if (code == KEY_RIGHTSHIFT) {
      rshift = value;
    }
    else if (code == KEY_RIGHTALT) {
      ralt = value;
    }
    else if (code == KEY_RIGHTMETA) {
      rmeta = value;
    }
    else {
//      USBH_UsrLog("        input_event, type: %d/%s, code: %d/%s, value: %d, lcsam: %d %d %d %d, rcsam: %d %d %d %d",
//          type, event_type_str[type], code, keycode_str[code], value, lctrl, lshift, lalt, lmeta, rctrl, rshift, ralt, rmeta);
    }
    keycode[code] = value;
  }
  else if (type == EV_SYN) {
    USBH_UsrLog("        input_event, type: %d/%s", type, event_type_str[type]);
    USBH_UsrLog("        pressed key:");

    int keypressed = 0;
    for (i = 0; i < 256; i++) {
      if (keycode[i]) {
        keypressed = 1;
        USBH_UsrLog("    %s", keycode_str[i]);
      }
    }

    if (!keypressed) {
      USBH_UsrLog("    no key pressed")
    }
  }
}

/*
 * The following two functions are moved from hid.h to avoid including input.h in hid.h
 */

/**
 * hid_map_usage - map usage input bits
 *
 * @hidinput: hidinput which we are interested in
 * @usage: usage to fill in
 * @bit: pointer to input->{}bit (out parameter)
 * @max: maximal valid usage->code to consider later (out parameter)
 * @type: input event type (EV_KEY, EV_REL, ...)
 * @c: code which corresponds to this usage and type
 */
static inline void hid_map_usage(struct hid_input *hidinput,
    struct hid_usage *usage, unsigned long **bit, int *max,
    uint8_t type, uint16_t c) // __u8 type, __u16 c)
{
  struct input_dev *input = &hidinput->input;

  usage->type = type;
  usage->code = c;

  switch (type)
  {
    /* to avoid circular include */
    case 0x03: /* EV_ABS: */
    *bit = input->absbit;
    *max = 0x3f; /* ABS_MAX; */
    break;
    case 0x02: /* EV_REL: */
    *bit = input->relbit;
    *max = 0x0f; /* REL_MAX; */
    break;
    case 0x01: /* EV_KEY: */
    *bit = input->keybit;
    *max = 0x2ff; /* KEY_MAX; */
    break;
    case 0x11: /* EV_LED: */
    *bit = input->ledbit;
    *max = 0x0f; /* LED_MAX; */
    break;
  }
}

/**
 * hid_map_usage_clear - map usage input bits and clear the input bit
 *
 * The same as hid_map_usage, except the @c bit is also cleared in supported
 * bits (@bit).
 */
static inline void hid_map_usage_clear(struct hid_input *hidinput,
    struct hid_usage *usage, unsigned long **bit, int *max,
    uint8_t type, uint16_t c) // _u8 type, __u16 c)
{
  hid_map_usage(hidinput, usage, bit, max, type, c);
  clear_bit(c, *bit);
}

static const struct {
  /* __s32 */  int32_t x;
  /* __s32 */  int32_t y;
}  hid_hat_to_axis[] = {{ 0, 0}, { 0,-1}, { 1,-1}, { 1, 0}, { 1, 1}, { 0, 1}, {-1, 1}, {-1, 0}, {-1,-1}};

#define map_abs(c)	hid_map_usage(hidinput, usage, &bit, &max, EV_ABS, (c))
#define map_rel(c)	hid_map_usage(hidinput, usage, &bit, &max, EV_REL, (c))
#define map_key(c)	hid_map_usage(hidinput, usage, &bit, &max, EV_KEY, (c))
#define map_led(c)	hid_map_usage(hidinput, usage, &bit, &max, EV_LED, (c))

#define map_abs_clear(c)	hid_map_usage_clear(hidinput, usage, &bit, &max, EV_ABS, (c))
#define map_key_clear(c)	hid_map_usage_clear(hidinput, usage, &bit, &max, EV_KEY, (c))

#if 0

static bool match_scancode(struct hid_usage *usage,
			   unsigned int cur_idx, unsigned int scancode)
{
	return (usage->hid & (HID_USAGE_PAGE | HID_USAGE)) == scancode;
}

static bool match_keycode(struct hid_usage *usage,
			  unsigned int cur_idx, unsigned int keycode)
{
	/*
	 * We should exclude unmapped usages when doing lookup by keycode.
	 */
	return (usage->type == EV_KEY && usage->code == keycode);
}

static bool match_index(struct hid_usage *usage,
			unsigned int cur_idx, unsigned int idx)
{
	return cur_idx == idx;
}

typedef bool (*hid_usage_cmp_t)(struct hid_usage *usage,
				unsigned int cur_idx, unsigned int val);

static struct hid_usage *hidinput_find_key(struct hid_device *hid,
					   hid_usage_cmp_t match,
					   unsigned int value,
					   unsigned int *usage_idx)
{
	unsigned int i, j, k, cur_idx = 0;
	struct hid_report *report;
	struct hid_usage *usage;

	for (k = HID_INPUT_REPORT; k <= HID_OUTPUT_REPORT; k++) {
		list_for_each_entry(report, &hid->report_enum[k].report_list, list) {
			for (i = 0; i < report->maxfield; i++) {
				for (j = 0; j < report->field[i]->maxusage; j++) {
					usage = report->field[i]->usage + j;
					if (usage->type == EV_KEY || usage->type == 0) {
						if (match(usage, cur_idx, value)) {
							if (usage_idx)
								*usage_idx = cur_idx;
							return usage;
						}
						cur_idx++;
					}
				}
			}
		}
	}
	return NULL;
}

static struct hid_usage *hidinput_locate_usage(struct hid_device *hid,
					const struct input_keymap_entry *ke,
					unsigned int *index)
{
	struct hid_usage *usage;
	unsigned int scancode;

	if (ke->flags & INPUT_KEYMAP_BY_INDEX)
		usage = hidinput_find_key(hid, match_index, ke->index, index);
	else if (input_scancode_to_scalar(ke, &scancode) == 0)
		usage = hidinput_find_key(hid, match_scancode, scancode, index);
	else
		usage = NULL;

	return usage;
}

static int hidinput_getkeycode(struct input_dev *dev,
			       struct input_keymap_entry *ke)
{
	struct hid_device *hid = input_get_drvdata(dev);
	struct hid_usage *usage;
	unsigned int scancode, index;

	usage = hidinput_locate_usage(hid, ke, &index);
	if (usage) {
		ke->keycode = usage->type == EV_KEY ?
				usage->code : KEY_RESERVED;
		ke->index = index;
		scancode = usage->hid & (HID_USAGE_PAGE | HID_USAGE);
		ke->len = sizeof(scancode);
		memcpy(ke->scancode, &scancode, sizeof(scancode));
		return 0;
	}

	return -EINVAL;
}

static int hidinput_setkeycode(struct input_dev *dev,
			       const struct input_keymap_entry *ke,
			       unsigned int *old_keycode)
{
	struct hid_device *hid = input_get_drvdata(dev);
	struct hid_usage *usage;

	usage = hidinput_locate_usage(hid, ke, NULL);
	if (usage) {
		*old_keycode = usage->type == EV_KEY ?
				usage->code : KEY_RESERVED;
		usage->code = ke->keycode;

		clear_bit(*old_keycode, dev->keybit);
		set_bit(usage->code, dev->keybit);
		dbg_hid("Assigned keycode %d to HID usage code %x\n",
			usage->code, usage->hid);

		/*
		 * Set the keybit for the old keycode if the old keycode is used
		 * by another key
		 */
		if (hidinput_find_key(hid, match_keycode, *old_keycode, NULL))
			set_bit(*old_keycode, dev->keybit);

		return 0;
	}

	return -EINVAL;
}
#endif

/*
 * Divide positive or negative dividend by positive divisor and round
 * to closest integer. Result is undefined for negative divisors and
 * for negative dividends if the divisor variable type is unsigned.
 */
#define DIV_ROUND_CLOSEST(x, divisor)(                  \
{                                                       \
        typeof(x) __x = x;                              \
        typeof(divisor) __d = divisor;                  \
        (((typeof(x))-1) > 0 ||                         \
         ((typeof(divisor))-1) > 0 || (__x) > 0) ?      \
                (((__x) + ((__d) / 2)) / (__d)) :       \
                (((__x) - ((__d) / 2)) / (__d));        \
}                                                       \
)


/**
 * hidinput_calc_abs_res - calculate an absolute axis resolution
 * @field: the HID report field to calculate resolution for
 * @code: axis code
 *
 * The formula is:
 *                         (logical_maximum - logical_minimum)
 * resolution = ----------------------------------------------------------
 *              (physical_maximum - physical_minimum) * 10 ^ unit_exponent
 *
 * as seen in the HID specification v1.11 6.2.2.7 Global Items.
 *
 * Only exponent 1 length units are processed. Centimeters and inches are
 * converted to millimeters. Degrees are converted to radians.
 */
int32_t hidinput_calc_abs_res(const struct hid_field *field, uint16_t code)
{
  int32_t unit_exponent = field->unit_exponent;
  int32_t logical_extents = field->logical_maximum -
					field->logical_minimum;
  int32_t physical_extents = field->physical_maximum -
					field->physical_minimum;
  int32_t prev;

	/* Check if the extents are sane */
	if (logical_extents <= 0 || physical_extents <= 0)
		return 0;

	/*
	 * Verify and convert units.
	 * See HID specification v1.11 6.2.2.7 Global Items for unit decoding
	 */
	switch (code) {
	case ABS_X:
	case ABS_Y:
	case ABS_Z:
	case ABS_MT_POSITION_X:
	case ABS_MT_POSITION_Y:
	case ABS_MT_TOOL_X:
	case ABS_MT_TOOL_Y:
	case ABS_MT_TOUCH_MAJOR:
	case ABS_MT_TOUCH_MINOR:
		if (field->unit == 0x11) {		/* If centimeters */
			/* Convert to millimeters */
			unit_exponent += 1;
		} else if (field->unit == 0x13) {	/* If inches */
			/* Convert to millimeters */
			prev = physical_extents;
			physical_extents *= 254;
			if (physical_extents < prev)
				return 0;
			unit_exponent -= 1;
		} else {
			return 0;
		}
		break;

	case ABS_RX:
	case ABS_RY:
	case ABS_RZ:
	case ABS_TILT_X:
	case ABS_TILT_Y:
		if (field->unit == 0x14) {		/* If degrees */
			/* Convert to radians */
			prev = logical_extents;
			logical_extents *= 573;
			if (logical_extents < prev)
				return 0;
			unit_exponent += 1;
		} else if (field->unit != 0x12) {	/* If not radians */
			return 0;
		}
		break;

	default:
		return 0;
	}

	/* Apply negative unit exponent */
	for (; unit_exponent < 0; unit_exponent++) {
		prev = logical_extents;
		logical_extents *= 10;
		if (logical_extents < prev)
			return 0;
	}
	/* Apply positive unit exponent */
	for (; unit_exponent > 0; unit_exponent--) {
		prev = physical_extents;
		physical_extents *= 10;
		if (physical_extents < prev)
			return 0;
	}

	/* Calculate resolution */
	return DIV_ROUND_CLOSEST(logical_extents, physical_extents);
}
// EXPORT_SYMBOL_GPL(hidinput_calc_abs_res);

#if 0

#ifdef CONFIG_HID_BATTERY_STRENGTH
static enum power_supply_property hidinput_battery_props[] = {
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_MODEL_NAME,
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_SCOPE,
};

#define HID_BATTERY_QUIRK_PERCENT	(1 << 0) /* always reports percent */
#define HID_BATTERY_QUIRK_FEATURE	(1 << 1) /* ask for feature report */

static const struct hid_device_id hid_battery_quirks[] = {
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE,
			USB_DEVICE_ID_APPLE_ALU_WIRELESS_2009_ISO),
	HID_BATTERY_QUIRK_PERCENT | HID_BATTERY_QUIRK_FEATURE },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE,
			       USB_DEVICE_ID_APPLE_ALU_WIRELESS_2011_ANSI),
	  HID_BATTERY_QUIRK_PERCENT | HID_BATTERY_QUIRK_FEATURE },
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_APPLE,
		USB_DEVICE_ID_APPLE_ALU_WIRELESS_ANSI),
	  HID_BATTERY_QUIRK_PERCENT | HID_BATTERY_QUIRK_FEATURE },
	{}
};

static unsigned find_battery_quirk(struct hid_device *hdev)
{
	unsigned quirks = 0;
	const struct hid_device_id *match;

	match = hid_match_id(hdev, hid_battery_quirks);
	if (match != NULL)
		quirks = match->driver_data;

	return quirks;
}

static int hidinput_get_battery_property(struct power_supply *psy,
					 enum power_supply_property prop,
					 union power_supply_propval *val)
{
	struct hid_device *dev = container_of(psy, struct hid_device, battery);
	int ret = 0;
	__u8 *buf;

	switch (prop) {
	case POWER_SUPPLY_PROP_PRESENT:
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = 1;
		break;

	case POWER_SUPPLY_PROP_CAPACITY:

		buf = kmalloc(2 * sizeof(__u8), GFP_KERNEL);
		if (!buf) {
			ret = -ENOMEM;
			break;
		}
		ret = dev->hid_get_raw_report(dev, dev->battery_report_id,
					      buf, 2,
					      dev->battery_report_type);

		if (ret != 2) {
			ret = -ENODATA;
			kfree(buf);
			break;
		}
		ret = 0;

		if (dev->battery_min < dev->battery_max &&
		    buf[1] >= dev->battery_min &&
		    buf[1] <= dev->battery_max)
			val->intval = (100 * (buf[1] - dev->battery_min)) /
				(dev->battery_max - dev->battery_min);
		kfree(buf);
		break;

	case POWER_SUPPLY_PROP_MODEL_NAME:
		val->strval = dev->name;
		break;

	case POWER_SUPPLY_PROP_STATUS:
		val->intval = POWER_SUPPLY_STATUS_DISCHARGING;
		break;

	case POWER_SUPPLY_PROP_SCOPE:
		val->intval = POWER_SUPPLY_SCOPE_DEVICE;
		break;

	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static bool hidinput_setup_battery(struct hid_device *dev, unsigned report_type, struct hid_field *field)
{
	struct power_supply *battery = &dev->battery;
	int ret;
	unsigned quirks;
	s32 min, max;

	if (field->usage->hid != HID_DC_BATTERYSTRENGTH)
		return false;	/* no match */

	if (battery->name != NULL)
		goto out;	/* already initialized? */

	battery->name = kasprintf(GFP_KERNEL, "hid-%s-battery", dev->uniq);
	if (battery->name == NULL)
		goto out;

	battery->type = POWER_SUPPLY_TYPE_BATTERY;
	battery->properties = hidinput_battery_props;
	battery->num_properties = ARRAY_SIZE(hidinput_battery_props);
	battery->use_for_apm = 0;
	battery->get_property = hidinput_get_battery_property;

	quirks = find_battery_quirk(dev);

	hid_dbg(dev, "device %x:%x:%x %d quirks %d\n",
		dev->bus, dev->vendor, dev->product, dev->version, quirks);

	min = field->logical_minimum;
	max = field->logical_maximum;

	if (quirks & HID_BATTERY_QUIRK_PERCENT) {
		min = 0;
		max = 100;
	}

	if (quirks & HID_BATTERY_QUIRK_FEATURE)
		report_type = HID_FEATURE_REPORT;

	dev->battery_min = min;
	dev->battery_max = max;
	dev->battery_report_type = report_type;
	dev->battery_report_id = field->report->id;

	ret = power_supply_register(&dev->dev, battery);
	if (ret != 0) {
		hid_warn(dev, "can't register power supply: %d\n", ret);
		kfree(battery->name);
		battery->name = NULL;
	}

	power_supply_powers(battery, &dev->dev);

out:
	return true;
}

static void hidinput_cleanup_battery(struct hid_device *dev)
{
	if (!dev->battery.name)
		return;

	power_supply_unregister(&dev->battery);
	kfree(dev->battery.name);
	dev->battery.name = NULL;
}
#else  /* !CONFIG_HID_BATTERY_STRENGTH */
static bool hidinput_setup_battery(struct hid_device *dev, unsigned report_type,
				   struct hid_field *field)
{
	return false;
}

static void hidinput_cleanup_battery(struct hid_device *dev)
{
}
#endif	/* CONFIG_HID_BATTERY_STRENGTH */

#endif



static void hidinput_configure_usage(struct hid_input *hidinput,
    struct hid_field *field, struct hid_usage *usage)
{
  struct input_dev *input = &hidinput->input;
  // struct hid_device *device = input_get_drvdata(input);
  struct hid_device *device = input->hiddev;

  int max = 0, code;
  unsigned long *bit = NULL;

  field->hidinput = hidinput;

  if (field->flags & HID_MAIN_ITEM_CONSTANT)
  {
    goto ignore;
  }

  /* Ignore if report count is out of bounds. */
  if (field->report_count < 1)
  {
    goto ignore;
  }

  /* only LED usages are supported in output fields */
  if (field->report_type == HID_OUTPUT_REPORT
      && (usage->hid & HID_USAGE_PAGE) != HID_UP_LED)
  {
    goto ignore;
  }

  /*
   * the following code seems to do input_mapping by driver
   * the driver mapper returns either mapped or ignoring mapping
   */
//	if (device->driver->input_mapping) {
//		int ret = device->driver->input_mapping(device, hidinput, field,
//				usage, &bit, &max);
//		if (ret > 0)
//			goto mapped;
//		if (ret < 0)
//			goto ignore;
//	}

  switch (usage->hid & HID_USAGE_PAGE)
  {
  case HID_UP_UNDEFINED:
    USBH_UsrLog("HID_UP_UNDEFINED")
    ;
    goto ignore;

  case HID_UP_KEYBOARD:
    set_bit(EV_REP, input->evbit);

    if ((usage->hid & HID_USAGE) < 256)
    {
      if (!hid_keyboard[usage->hid & HID_USAGE])
        goto ignore;
      map_key_clear(hid_keyboard[usage->hid & HID_USAGE]);
    }
    else
      map_key(KEY_UNKNOWN);

//    USBH_UsrLog("        HID_UP_KEYBOARD usage type: %d, code: %d", usage->type,
//        usage->code);
    break;

  case HID_UP_BUTTON:
    USBH_UsrLog("HID_UP_BUTTON")
    ;
    code = ((usage->hid - 1) & HID_USAGE);

    switch (field->application)
    {
    case HID_GD_MOUSE:
    case HID_GD_POINTER:
      code += BTN_MOUSE;
      break;
    case HID_GD_JOYSTICK:
      if (code <= 0xf)
        code += BTN_JOYSTICK;
      else
        code += BTN_TRIGGER_HAPPY - 0x10;
      break;
    case HID_GD_GAMEPAD:
      if (code <= 0xf)
        code += BTN_GAMEPAD;
      else
        code += BTN_TRIGGER_HAPPY - 0x10;
      break;
    default:
      switch (field->physical)
      {
      case HID_GD_MOUSE:
      case HID_GD_POINTER:
        code += BTN_MOUSE;
        break;
      case HID_GD_JOYSTICK:
        code += BTN_JOYSTICK;
        break;
      case HID_GD_GAMEPAD:
        code += BTN_GAMEPAD;
        break;
      default:
        code += BTN_MISC;
      }
    }

    map_key(code);
    break;

  case HID_UP_SIMULATION:
    USBH_UsrLog("HID_UP_SIMULATION")
    ;
    switch (usage->hid & 0xffff)
    {
    case 0xba:
      map_abs(ABS_RUDDER);
      break;
    case 0xbb:
      map_abs(ABS_THROTTLE);
      break;
    case 0xc4:
      map_abs(ABS_GAS);
      break;
    case 0xc5:
      map_abs(ABS_BRAKE);
      break;
    case 0xc8:
      map_abs(ABS_WHEEL);
      break;
    default:
      goto ignore;
    }
    break;

  case HID_UP_GENDESK:
    USBH_UsrLog("HID_UP_GENDESK")
    ;
    if ((usage->hid & 0xf0) == 0x80)
    { /* SystemControl */
      switch (usage->hid & 0xf)
      {
      case 0x1:
        map_key_clear(KEY_POWER);
        break;
      case 0x2:
        map_key_clear(KEY_SLEEP);
        break;
      case 0x3:
        map_key_clear(KEY_WAKEUP);
        break;
      case 0x4:
        map_key_clear(KEY_CONTEXT_MENU);
        break;
      case 0x5:
        map_key_clear(KEY_MENU);
        break;
      case 0x6:
        map_key_clear(KEY_PROG1);
        break;
      case 0x7:
        map_key_clear(KEY_HELP);
        break;
      case 0x8:
        map_key_clear(KEY_EXIT);
        break;
      case 0x9:
        map_key_clear(KEY_SELECT);
        break;
      case 0xa:
        map_key_clear(KEY_RIGHT);
        break;
      case 0xb:
        map_key_clear(KEY_LEFT);
        break;
      case 0xc:
        map_key_clear(KEY_UP);
        break;
      case 0xd:
        map_key_clear(KEY_DOWN);
        break;
      case 0xe:
        map_key_clear(KEY_POWER2);
        break;
      case 0xf:
        map_key_clear(KEY_RESTART);
        break;
      default:
        goto unknown;
      }
      break;
    }

    if ((usage->hid & 0xf0) == 0x90)
    { /* D-pad */
      switch (usage->hid)
      {
      case HID_GD_UP:
        usage->hat_dir = 1;
        break;
      case HID_GD_DOWN:
        usage->hat_dir = 5;
        break;
      case HID_GD_RIGHT:
        usage->hat_dir = 3;
        break;
      case HID_GD_LEFT:
        usage->hat_dir = 7;
        break;
      default:
        goto unknown;
      }
      if (field->dpad)
      {
        map_abs(field->dpad);
        goto ignore;
      }
      map_abs(ABS_HAT0X);
      break;
    }

    switch (usage->hid)
    {
    /* These usage IDs map directly to the usage codes. */
    case HID_GD_X:
    case HID_GD_Y:
    case HID_GD_Z:
    case HID_GD_RX:
    case HID_GD_RY:
    case HID_GD_RZ:
    case HID_GD_SLIDER:
    case HID_GD_DIAL:
    case HID_GD_WHEEL:
      if (field->flags & HID_MAIN_ITEM_RELATIVE)
        map_rel(usage->hid & 0xf);
      else
        map_abs(usage->hid & 0xf);
      break;

    case HID_GD_HATSWITCH:
      usage->hat_min = field->logical_minimum;
      usage->hat_max = field->logical_maximum;
      map_abs(ABS_HAT0X);
      break;

    case HID_GD_START:
      map_key_clear(BTN_START);
      break;
    case HID_GD_SELECT:
      map_key_clear(BTN_SELECT);
      break;

    default:
      goto unknown;
    }

    break;

  case HID_UP_LED:
    // USBH_UsrLog("HID_UP_LED");

    switch (usage->hid & 0xffff)
    { /* HID-Value:                   */
    case 0x01:
      map_led(LED_NUML);
      break; /*   "Num Lock"                 */
    case 0x02:
      map_led(LED_CAPSL);
      break; /*   "Caps Lock"                */
    case 0x03:
      map_led(LED_SCROLLL);
      break; /*   "Scroll Lock"              */
    case 0x04:
      map_led(LED_COMPOSE);
      break; /*   "Compose"                  */
    case 0x05:
      map_led(LED_KANA);
      break; /*   "Kana"                     */
    case 0x27:
      map_led(LED_SLEEP);
      break; /*   "Stand-By"                 */
    case 0x4c:
      map_led(LED_SUSPEND);
      break; /*   "System Suspend"           */
    case 0x09:
      map_led(LED_MUTE);
      break; /*   "Mute"                     */
    case 0x4b:
      map_led(LED_MISC);
      break; /*   "Generic Indicator"        */
    case 0x19:
      map_led(LED_MAIL);
      break; /*   "Message Waiting"          */
    case 0x4d:
      map_led(LED_CHARGING);
      break; /*   "External Power Connected" */

    default:
      goto ignore;
    }
    break;

  case HID_UP_DIGITIZER:
    USBH_UsrLog("HID_UP_DIGITIZER")
    ;
    switch (usage->hid & 0xff)
    {
    case 0x00: /* Undefined */
      goto ignore;

    case 0x30: /* TipPressure */
      if (!test_bit(BTN_TOUCH, input->keybit))
      {
        // device->quirks |= HID_QUIRK_NOTOUCH;
        // hidinput->
        set_bit(EV_KEY, input->evbit);
        set_bit(BTN_TOUCH, input->keybit);
      }
      map_abs_clear(ABS_PRESSURE);
      break;

    case 0x32: /* InRange */
      switch (field->physical & 0xff)
      {
      case 0x21:
        map_key(BTN_TOOL_MOUSE);
        break;
      case 0x22:
        map_key(BTN_TOOL_FINGER);
        break;
      default:
        map_key(BTN_TOOL_PEN);
        break;
      }
      break;

    case 0x3c: /* Invert */
      map_key_clear(BTN_TOOL_RUBBER);
      break;

    case 0x3d: /* X Tilt */
      map_abs_clear(ABS_TILT_X);
      break;

    case 0x3e: /* Y Tilt */
      map_abs_clear(ABS_TILT_Y);
      break;

    case 0x33: /* Touch */
    case 0x42: /* TipSwitch */
    case 0x43: /* TipSwitch2 */
      device->quirks &= ~HID_QUIRK_NOTOUCH;
      map_key_clear(BTN_TOUCH);
      break;

    case 0x44: /* BarrelSwitch */
      map_key_clear(BTN_STYLUS);
      break;

    case 0x46: /* TabletPick */
      map_key_clear(BTN_STYLUS2);
      break;

    default:
      goto unknown;
    }
    break;

  case HID_UP_CONSUMER: /* USB HUT v1.12, pages 75-84 */
    USBH_UsrLog("HID_UP_CONSUMER")
    ;
    switch (usage->hid & HID_USAGE)
    {
    case 0x000:
      goto ignore;
    case 0x030:
      map_key_clear(KEY_POWER);
      break;
    case 0x031:
      map_key_clear(KEY_RESTART);
      break;
    case 0x032:
      map_key_clear(KEY_SLEEP);
      break;
    case 0x034:
      map_key_clear(KEY_SLEEP);
      break;
    case 0x035:
      map_key_clear(KEY_KBDILLUMTOGGLE);
      break;
    case 0x036:
      map_key_clear(BTN_MISC);
      break;

    case 0x040:
      map_key_clear(KEY_MENU);
      break; /* Menu */
    case 0x041:
      map_key_clear(KEY_SELECT);
      break; /* Menu Pick */
    case 0x042:
      map_key_clear(KEY_UP);
      break; /* Menu Up */
    case 0x043:
      map_key_clear(KEY_DOWN);
      break; /* Menu Down */
    case 0x044:
      map_key_clear(KEY_LEFT);
      break; /* Menu Left */
    case 0x045:
      map_key_clear(KEY_RIGHT);
      break; /* Menu Right */
    case 0x046:
      map_key_clear(KEY_ESC);
      break; /* Menu Escape */
    case 0x047:
      map_key_clear(KEY_KPPLUS);
      break; /* Menu Value Increase */
    case 0x048:
      map_key_clear(KEY_KPMINUS);
      break; /* Menu Value Decrease */

    case 0x060:
      map_key_clear(KEY_INFO);
      break; /* Data On Screen */
    case 0x061:
      map_key_clear(KEY_SUBTITLE);
      break; /* Closed Caption */
    case 0x063:
      map_key_clear(KEY_VCR);
      break; /* VCR/TV */
    case 0x065:
      map_key_clear(KEY_CAMERA);
      break; /* Snapshot */
    case 0x069:
      map_key_clear(KEY_RED);
      break;
    case 0x06a:
      map_key_clear(KEY_GREEN);
      break;
    case 0x06b:
      map_key_clear(KEY_BLUE);
      break;
    case 0x06c:
      map_key_clear(KEY_YELLOW);
      break;
    case 0x06d:
      map_key_clear(KEY_ZOOM);
      break;

    case 0x082:
      map_key_clear(KEY_VIDEO_NEXT);
      break;
    case 0x083:
      map_key_clear(KEY_LAST);
      break;
    case 0x084:
      map_key_clear(KEY_ENTER);
      break;
    case 0x088:
      map_key_clear(KEY_PC);
      break;
    case 0x089:
      map_key_clear(KEY_TV);
      break;
    case 0x08a:
      map_key_clear(KEY_WWW);
      break;
    case 0x08b:
      map_key_clear(KEY_DVD);
      break;
    case 0x08c:
      map_key_clear(KEY_PHONE);
      break;
    case 0x08d:
      map_key_clear(KEY_PROGRAM);
      break;
    case 0x08e:
      map_key_clear(KEY_VIDEOPHONE);
      break;
    case 0x08f:
      map_key_clear(KEY_GAMES);
      break;
    case 0x090:
      map_key_clear(KEY_MEMO);
      break;
    case 0x091:
      map_key_clear(KEY_CD);
      break;
    case 0x092:
      map_key_clear(KEY_VCR);
      break;
    case 0x093:
      map_key_clear(KEY_TUNER);
      break;
    case 0x094:
      map_key_clear(KEY_EXIT);
      break;
    case 0x095:
      map_key_clear(KEY_HELP);
      break;
    case 0x096:
      map_key_clear(KEY_TAPE);
      break;
    case 0x097:
      map_key_clear(KEY_TV2);
      break;
    case 0x098:
      map_key_clear(KEY_SAT);
      break;
    case 0x09a:
      map_key_clear(KEY_PVR);
      break;

    case 0x09c:
      map_key_clear(KEY_CHANNELUP);
      break;
    case 0x09d:
      map_key_clear(KEY_CHANNELDOWN);
      break;
    case 0x0a0:
      map_key_clear(KEY_VCR2);
      break;

    case 0x0b0:
      map_key_clear(KEY_PLAY);
      break;
    case 0x0b1:
      map_key_clear(KEY_PAUSE);
      break;
    case 0x0b2:
      map_key_clear(KEY_RECORD);
      break;
    case 0x0b3:
      map_key_clear(KEY_FASTFORWARD);
      break;
    case 0x0b4:
      map_key_clear(KEY_REWIND);
      break;
    case 0x0b5:
      map_key_clear(KEY_NEXTSONG);
      break;
    case 0x0b6:
      map_key_clear(KEY_PREVIOUSSONG);
      break;
    case 0x0b7:
      map_key_clear(KEY_STOPCD);
      break;
    case 0x0b8:
      map_key_clear(KEY_EJECTCD);
      break;
    case 0x0bc:
      map_key_clear(KEY_MEDIA_REPEAT);
      break;
    case 0x0b9:
      map_key_clear(KEY_SHUFFLE);
      break;
    case 0x0bf:
      map_key_clear(KEY_SLOW);
      break;

    case 0x0cd:
      map_key_clear(KEY_PLAYPAUSE);
      break;
    case 0x0e0:
      map_abs_clear(ABS_VOLUME);
      break;
    case 0x0e2:
      map_key_clear(KEY_MUTE);
      break;
    case 0x0e5:
      map_key_clear(KEY_BASSBOOST);
      break;
    case 0x0e9:
      map_key_clear(KEY_VOLUMEUP);
      break;
    case 0x0ea:
      map_key_clear(KEY_VOLUMEDOWN);
      break;
    case 0x0f5:
      map_key_clear(KEY_SLOW);
      break;

    case 0x182:
      map_key_clear(KEY_BOOKMARKS);
      break;
    case 0x183:
      map_key_clear(KEY_CONFIG);
      break;
    case 0x184:
      map_key_clear(KEY_WORDPROCESSOR);
      break;
    case 0x185:
      map_key_clear(KEY_EDITOR);
      break;
    case 0x186:
      map_key_clear(KEY_SPREADSHEET);
      break;
    case 0x187:
      map_key_clear(KEY_GRAPHICSEDITOR);
      break;
    case 0x188:
      map_key_clear(KEY_PRESENTATION);
      break;
    case 0x189:
      map_key_clear(KEY_DATABASE);
      break;
    case 0x18a:
      map_key_clear(KEY_MAIL);
      break;
    case 0x18b:
      map_key_clear(KEY_NEWS);
      break;
    case 0x18c:
      map_key_clear(KEY_VOICEMAIL);
      break;
    case 0x18d:
      map_key_clear(KEY_ADDRESSBOOK);
      break;
    case 0x18e:
      map_key_clear(KEY_CALENDAR);
      break;
    case 0x191:
      map_key_clear(KEY_FINANCE);
      break;
    case 0x192:
      map_key_clear(KEY_CALC);
      break;
    case 0x193:
      map_key_clear(KEY_PLAYER);
      break;
    case 0x194:
      map_key_clear(KEY_FILE);
      break;
    case 0x196:
      map_key_clear(KEY_WWW);
      break;
    case 0x199:
      map_key_clear(KEY_CHAT);
      break;
    case 0x19c:
      map_key_clear(KEY_LOGOFF);
      break;
    case 0x19e:
      map_key_clear(KEY_COFFEE);
      break;
    case 0x1a6:
      map_key_clear(KEY_HELP);
      break;
    case 0x1a7:
      map_key_clear(KEY_DOCUMENTS);
      break;
    case 0x1ab:
      map_key_clear(KEY_SPELLCHECK);
      break;
    case 0x1ae:
      map_key_clear(KEY_KEYBOARD);
      break;
    case 0x1b6:
      map_key_clear(KEY_IMAGES);
      break;
    case 0x1b7:
      map_key_clear(KEY_AUDIO);
      break;
    case 0x1b8:
      map_key_clear(KEY_VIDEO);
      break;
    case 0x1bc:
      map_key_clear(KEY_MESSENGER);
      break;
    case 0x1bd:
      map_key_clear(KEY_INFO);
      break;
    case 0x201:
      map_key_clear(KEY_NEW);
      break;
    case 0x202:
      map_key_clear(KEY_OPEN);
      break;
    case 0x203:
      map_key_clear(KEY_CLOSE);
      break;
    case 0x204:
      map_key_clear(KEY_EXIT);
      break;
    case 0x207:
      map_key_clear(KEY_SAVE);
      break;
    case 0x208:
      map_key_clear(KEY_PRINT);
      break;
    case 0x209:
      map_key_clear(KEY_PROPS);
      break;
    case 0x21a:
      map_key_clear(KEY_UNDO);
      break;
    case 0x21b:
      map_key_clear(KEY_COPY);
      break;
    case 0x21c:
      map_key_clear(KEY_CUT);
      break;
    case 0x21d:
      map_key_clear(KEY_PASTE);
      break;
    case 0x21f:
      map_key_clear(KEY_FIND);
      break;
    case 0x221:
      map_key_clear(KEY_SEARCH);
      break;
    case 0x222:
      map_key_clear(KEY_GOTO);
      break;
    case 0x223:
      map_key_clear(KEY_HOMEPAGE);
      break;
    case 0x224:
      map_key_clear(KEY_BACK);
      break;
    case 0x225:
      map_key_clear(KEY_FORWARD);
      break;
    case 0x226:
      map_key_clear(KEY_STOP);
      break;
    case 0x227:
      map_key_clear(KEY_REFRESH);
      break;
    case 0x22a:
      map_key_clear(KEY_BOOKMARKS);
      break;
    case 0x22d:
      map_key_clear(KEY_ZOOMIN);
      break;
    case 0x22e:
      map_key_clear(KEY_ZOOMOUT);
      break;
    case 0x22f:
      map_key_clear(KEY_ZOOMRESET);
      break;
    case 0x233:
      map_key_clear(KEY_SCROLLUP);
      break;
    case 0x234:
      map_key_clear(KEY_SCROLLDOWN);
      break;
    case 0x238:
      map_rel(REL_HWHEEL);
      break;
    case 0x23d:
      map_key_clear(KEY_EDIT);
      break;
    case 0x25f:
      map_key_clear(KEY_CANCEL);
      break;
    case 0x269:
      map_key_clear(KEY_INSERT);
      break;
    case 0x26a:
      map_key_clear(KEY_DELETE);
      break;
    case 0x279:
      map_key_clear(KEY_REDO);
      break;

    case 0x289:
      map_key_clear(KEY_REPLY);
      break;
    case 0x28b:
      map_key_clear(KEY_FORWARDMAIL);
      break;
    case 0x28c:
      map_key_clear(KEY_SEND);
      break;

    default:
      goto ignore;
    }
    break;

  case HID_UP_GENDEVCTRLS:        // TODO
    USBH_UsrLog("HID_UP_GENDEVCTRLS")
    ;
//		if (hidinput_setup_battery(device, HID_INPUT_REPORT, field))
//			goto ignore;
//		else
//			goto unknown;
    break;

  case HID_UP_HPVENDOR: /* Reported on a Dutch layout HP5308 */
    USBH_UsrLog("HID_UP_HPVENDOR")
    ;
    set_bit(EV_REP, input->evbit);
    switch (usage->hid & HID_USAGE)
    {
    case 0x021:
      map_key_clear(KEY_PRINT);
      break;
    case 0x070:
      map_key_clear(KEY_HP);
      break;
    case 0x071:
      map_key_clear(KEY_CAMERA);
      break;
    case 0x072:
      map_key_clear(KEY_SOUND);
      break;
    case 0x073:
      map_key_clear(KEY_QUESTION);
      break;
    case 0x080:
      map_key_clear(KEY_EMAIL);
      break;
    case 0x081:
      map_key_clear(KEY_CHAT);
      break;
    case 0x082:
      map_key_clear(KEY_SEARCH);
      break;
    case 0x083:
      map_key_clear(KEY_CONNECT);
      break;
    case 0x084:
      map_key_clear(KEY_FINANCE);
      break;
    case 0x085:
      map_key_clear(KEY_SPORT);
      break;
    case 0x086:
      map_key_clear(KEY_SHOP);
      break;
    default:
      goto ignore;
    }
    break;

  case HID_UP_HPVENDOR2:
    USBH_UsrLog("HID_UP_HPVENDOR2")
    ;
    set_bit(EV_REP, input->evbit);
    switch (usage->hid & HID_USAGE)
    {
    case 0x003:
      map_key_clear(KEY_BRIGHTNESSDOWN);
      break;
    case 0x004:
      map_key_clear(KEY_BRIGHTNESSUP);
      break;
    default:
      goto ignore;
    }
    break;

  case HID_UP_MSVENDOR:
    USBH_UsrLog("HID_UP_MSVENDOR")
    ;
    goto ignore;

  case HID_UP_CUSTOM: /* Reported on Logitech and Apple USB keyboards */
    USBH_UsrLog("HID_UP_CUSTOM")
    ;
    set_bit(EV_REP, input->evbit);
    goto ignore;

  case HID_UP_LOGIVENDOR:
    USBH_UsrLog("HID_UP_LOGIVENDOR")
    ;
    goto ignore;

  case HID_UP_PID:
    USBH_UsrLog("HID_UP_PID")
    ;
    switch (usage->hid & HID_USAGE)
    {
    case 0xa4:
      map_key_clear(BTN_DEAD);
      break;
    default:
      goto ignore;
    }
    break;

  default:
    USBH_UsrLog("HID_UP_DEFAULT")
    ;
    unknown: if (field->report_size == 1)
    {
      if (field->report->type == HID_OUTPUT_REPORT)
      {
        map_led(LED_MISC);
        break;
      }
      map_key(BTN_MISC);
      break;
    }
    if (field->flags & HID_MAIN_ITEM_RELATIVE)
    {
      map_rel(REL_MISC);
      break;
    }
    map_abs(ABS_MISC);
    break;
  }

  mapped:
//	if (device->driver->input_mapped && device->driver->input_mapped(device,
//				hidinput, field, usage, &bit, &max) < 0)
//		goto ignore;

  set_bit(usage->type, input->evbit);

  while (usage->code <= max && test_and_set_bit(usage->code, bit))
    usage->code = find_next_zero_bit(bit, max + 1, usage->code);

  if (usage->code > max)
    goto ignore;

  if (usage->type == EV_ABS)
  {

    int a = field->logical_minimum;
    int b = field->logical_maximum;

    if ((device->quirks & HID_QUIRK_BADPAD)
        && (usage->code == ABS_X || usage->code == ABS_Y))
    {
      a = field->logical_minimum = 0;
      b = field->logical_maximum = 255;
    }

    if (field->application == HID_GD_GAMEPAD
        || field->application == HID_GD_JOYSTICK)
      input_set_abs_params(input, usage->code, a, b, (b - a) >> 8,
          (b - a) >> 4);
    else
      input_set_abs_params(input, usage->code, a, b, 0, 0);

    input_abs_set_res(input, usage->code,
        hidinput_calc_abs_res(field, usage->code));

    /* use a larger default input buffer for MT devices */
    if (usage->code == ABS_MT_POSITION_X && input->hint_events_per_packet == 0)
      input_set_events_per_packet(input, 60);
  }

  if (usage->type == EV_ABS
      && (usage->hat_min < usage->hat_max || usage->hat_dir))
  {
    int i;
    for (i = usage->code; i < usage->code + 2 && i <= max; i++)
    {
      input_set_abs_params(input, i, -1, 1, 0, 0);
      set_bit(i, input->absbit);
    }
    if (usage->hat_dir && !field->dpad)
      field->dpad = usage->code;
  }

  /* for those devices which produce Consumer volume usage as relative,
   * we emulate pressing volumeup/volumedown appropriate number of times
   * in hidinput_hid_event()
   */
  if ((usage->type == EV_ABS) && (field->flags & HID_MAIN_ITEM_RELATIVE)
      && (usage->code == ABS_VOLUME))
  {
    set_bit(KEY_VOLUMEUP, input->keybit);
    set_bit(KEY_VOLUMEDOWN, input->keybit);
  }

  if (usage->type == EV_KEY)
  {
    set_bit(EV_MSC, input->evbit);
    set_bit(MSC_SCAN, input->mscbit);
  }

  ignore: return;
}

/*
 * interpret each usage inside a field.
 *
 * void hidinput_hid_event(struct hid_device *hid, struct hid_field *field, struct hid_usage *usage, __s32 value)
 */
void hidinput_hid_event(struct hid_device *hid, struct hid_field *field,
    struct hid_usage *usage, int32_t value)
{
  struct input_dev *input;
  unsigned *quirks = &hid->quirks;

  // USBH_UsrLog("      %s: usage code: %d, type: %d", __func__, usage->code, usage->type);

  if (!field->hidinput)
  {
    return;
  }

  input = &(field->hidinput->input);

  if (!usage->type)
  {
    return;
  }

  if (usage->hat_min < usage->hat_max || usage->hat_dir)
  {
    int hat_dir = usage->hat_dir;
    if (!hat_dir)
      hat_dir = (value - usage->hat_min) * 8
          / (usage->hat_max - usage->hat_min + 1) + 1;
    if (hat_dir < 0 || hat_dir > 8)
      hat_dir = 0;
    input_event(input, usage->type, usage->code, hid_hat_to_axis[hat_dir].x);
    input_event(input, usage->type, usage->code + 1,
        hid_hat_to_axis[hat_dir].y);
    return;
  }

  if (usage->hid == (HID_UP_DIGITIZER | 0x003c))
  { /* Invert */
    *quirks = value ? (*quirks | HID_QUIRK_INVERT) : (*quirks & ~HID_QUIRK_INVERT);
    return;
  }

  if (usage->hid == (HID_UP_DIGITIZER | 0x0032))
  { /* InRange */
    if (value)
    {
      input_event(input, usage->type,
          (*quirks & HID_QUIRK_INVERT) ? BTN_TOOL_RUBBER : usage->code, 1);
      return;
    }
    input_event(input, usage->type, usage->code, 0);
    input_event(input, usage->type, BTN_TOOL_RUBBER, 0);
    return;
  }

  if (usage->hid == (HID_UP_DIGITIZER | 0x0030)
      && (*quirks & HID_QUIRK_NOTOUCH))
  { /* Pressure */
    int a = field->logical_minimum;
    int b = field->logical_maximum;
    input_event(input, EV_KEY, BTN_TOUCH, value > a + ((b - a) >> 3));
  }

  if (usage->hid == (HID_UP_PID | 0x83UL))
  { /* Simultaneous Effects Max */
    // dbg_hid("Maximum Effects - %d\n",value);
    return;
  }

  if (usage->hid == (HID_UP_PID | 0x7fUL))
  {
    // dbg_hid("PID Pool Report\n");
    return;
  }

  if ((usage->type == EV_KEY) && (usage->code == 0)) /* Key 0 is "unassigned", not KEY_UNKNOWN */
  {
    return;
  }

  if ((usage->type == EV_ABS) && (field->flags & HID_MAIN_ITEM_RELATIVE)
      && (usage->code == ABS_VOLUME))
  {
    int count = abs(value);
    int direction = value > 0 ? KEY_VOLUMEUP : KEY_VOLUMEDOWN;
    int i;

    for (i = 0; i < count; i++)
    {
      input_event(input, EV_KEY, direction, 1);
      // input_sync(input);
      input_event(input, EV_KEY, direction, 0);
      // input_sync(input);
    }
    return;
  }

  /*
   * Ignore out-of-range values as per HID specification,
   * section 5.10 and 6.2.25.
   *
   * The logical_minimum < logical_maximum check is done so that we
   * don't unintentionally discard values sent by devices which
   * don't specify logical min and max.
   */
  if ((field->flags & HID_MAIN_ITEM_VARIABLE)
      && (field->logical_minimum < field->logical_maximum)
      && (value < field->logical_minimum || value > field->logical_maximum))
  {
    // dbg_hid("Ignoring out-of-range value %x\n", value);
    return;
  }

  /* report the usage code as scancode if the key status has changed */
//	if (usage->type == EV_KEY && !!test_bit(usage->code, input->key) != value)
//		input_event(input, EV_MSC, MSC_SCAN, usage->hid);
  input_event(input, usage->type, usage->code, value);

  if ((field->flags & HID_MAIN_ITEM_RELATIVE) && (usage->type == EV_KEY))
    input_event(input, usage->type, usage->code, 0);
}

/*
 * do input sync for each device
 */
void hidinput_report_event(struct hid_device *hid, struct hid_report *report)
{
  int i;
  struct hid_input *hidinput;

  USBH_UsrLog("%s", __func__);

  if (hid->quirks & HID_QUIRK_NO_INPUT_SYNC)
    return;

//  list_for_each_entry(hidinput, &hid->inputs, list)
//    // input_sync(hidinput->input);
//    input_event(hidinput->input, EV_SYN, SYN_REPORT, 0);

  for (i = 0; i < hid->hidinput_list_size; i++) {
    input_event(&hid->hidinput_list[i].input, EV_SYN, SYN_REPORT, 0);
  }
}


#if 0

int hidinput_find_field(struct hid_device *hid, unsigned int type, unsigned int code, struct hid_field **field)
{
	struct hid_report *report;
	int i, j;

	list_for_each_entry(report, &hid->report_enum[HID_OUTPUT_REPORT].report_list, list) {
		for (i = 0; i < report->maxfield; i++) {
			*field = report->field[i];
			for (j = 0; j < (*field)->maxusage; j++)
				if ((*field)->usage[j].type == type && (*field)->usage[j].code == code)
					return j;
		}
	}
	return -1;
}
EXPORT_SYMBOL_GPL(hidinput_find_field);

struct hid_field *hidinput_get_led_field(struct hid_device *hid)
{
	struct hid_report *report;
	struct hid_field *field;
	int i, j;

	list_for_each_entry(report,
			    &hid->report_enum[HID_OUTPUT_REPORT].report_list,
			    list) {
		for (i = 0; i < report->maxfield; i++) {
			field = report->field[i];
			for (j = 0; j < field->maxusage; j++)
				if (field->usage[j].type == EV_LED)
					return field;
		}
	}
	return NULL;
}
EXPORT_SYMBOL_GPL(hidinput_get_led_field);

unsigned int hidinput_count_leds(struct hid_device *hid)
{
	struct hid_report *report;
	struct hid_field *field;
	int i, j;
	unsigned int count = 0;

	list_for_each_entry(report,
			    &hid->report_enum[HID_OUTPUT_REPORT].report_list,
			    list) {
		for (i = 0; i < report->maxfield; i++) {
			field = report->field[i];
			for (j = 0; j < field->maxusage; j++)
				if (field->usage[j].type == EV_LED &&
				    field->value[j])
					count += 1;
		}
	}
	return count;
}
EXPORT_SYMBOL_GPL(hidinput_count_leds);

static void hidinput_led_worker(struct work_struct *work)
{
	struct hid_device *hid = container_of(work, struct hid_device,
					      led_work);
	struct hid_field *field;
	struct hid_report *report;
	int len;
	__u8 *buf;

	field = hidinput_get_led_field(hid);
	if (!field)
		return;

	/*
	 * field->report is accessed unlocked regarding HID core. So there might
	 * be another incoming SET-LED request from user-space, which changes
	 * the LED state while we assemble our outgoing buffer. However, this
	 * doesn't matter as hid_output_report() correctly converts it into a
	 * boolean value no matter what information is currently set on the LED
	 * field (even garbage). So the remote device will always get a valid
	 * request.
	 * And in case we send a wrong value, a next led worker is spawned
	 * for every SET-LED request so the following worker will send the
	 * correct value, guaranteed!
	 */

	report = field->report;

	/* use custom SET_REPORT request if possible (asynchronous) */
	if (hid->ll_driver->request)
		return hid->ll_driver->request(hid, report, HID_REQ_SET_REPORT);

	/* fall back to generic raw-output-report */
	len = ((report->size - 1) >> 3) + 1 + (report->id > 0);
	buf = kmalloc(len, GFP_KERNEL);
	if (!buf)
		return;

	hid_output_report(report, buf);
	/* synchronous output report */
	hid->hid_output_raw_report(hid, buf, len, HID_OUTPUT_REPORT);
	kfree(buf);
}

static int hidinput_input_event(struct input_dev *dev, unsigned int type,
				unsigned int code, int value)
{
	struct hid_device *hid = input_get_drvdata(dev);
	struct hid_field *field;
	int offset;

	if (type == EV_FF)
		return input_ff_event(dev, type, code, value);

	if (type != EV_LED)
		return -1;

	if ((offset = hidinput_find_field(hid, type, code, &field)) == -1) {
		hid_warn(dev, "event field not found\n");
		return -1;
	}

	hid_set_field(field, offset, value);

	schedule_work(&hid->led_work);
	return 0;
}

static int hidinput_open(struct input_dev *dev)
{
	struct hid_device *hid = input_get_drvdata(dev);

	return hid_hw_open(hid);
}

static void hidinput_close(struct input_dev *dev)
{
	struct hid_device *hid = input_get_drvdata(dev);

	hid_hw_close(hid);
}

#endif

// TODO wth is this?
static void report_features(struct hid_device *hid)
{
//	struct hid_driver *drv = hid->driver;
//	struct hid_report_enum *rep_enum;
//	struct hid_report *rep;
//	int i, j;
//
//	rep_enum = &hid->report_enum[HID_FEATURE_REPORT];
//	list_for_each_entry(rep, &rep_enum->report_list, list)
//		for (i = 0; i < rep->maxfield; i++) {
//			/* Ignore if report count is out of bounds. */
//			if (rep->field[i]->report_count < 1)
//				continue;
//
//			for (j = 0; j < rep->field[i]->maxusage; j++) {
//				/* Verify if Battery Strength feature is available */
//				hidinput_setup_battery(hid, HID_FEATURE_REPORT, rep->field[i]);
//
//				if (drv->feature_mapping)
//					drv->feature_mapping(hid, rep->field[i],
//							     rep->field[i]->usage + j);
//			}
//		}
}

/**
 * TODO don't know the consequence
 * the strategy is create hid_input but not input_dev
 */
static struct hid_input *hidinput_allocate(struct hid_device *hid)
{
//	struct hid_input *hidinput = kzalloc(sizeof(*hidinput), GFP_KERNEL);
//	struct input_dev *input_dev = input_allocate_device();
//	if (!hidinput || !input_dev) {
//		kfree(hidinput);
//		input_free_device(input_dev);
//		hid_err(hid, "Out of memory during hid input probe\n");
//		return NULL;
//	}

    USBH_UsrLog("!!! %s", __func__);

//    struct hid_input *hidinput = malloc(sizeof(struct hid_input));
//    if (hidinput == NULL)
//      return NULL;
    struct hid_input *hidinput = &hid->hidinput_list[hid->hidinput_list_size];

//    struct input_dev *input_dev = malloc(sizeof(struct input_dev));
//    if (input_dev == NULL) {
//      // free(hidinput);
//      return NULL;
//    }

//    memset(input_dev, 0, sizeof(struct input_dev));
//    input_dev->hiddev = hid;

    memset(hidinput, 0, sizeof(struct hid_input));

//	input_set_drvdata(input_dev, hid);
//  input_dev->hiddev = hid;
    hidinput->input.hiddev = hid;

//	if (hid->ll_driver->hidinput_input_event)
//		input_dev->event = hid->ll_driver->hidinput_input_event;
//	else if (hid->ll_driver->request || hid->hid_output_raw_report)
//		input_dev->event = hidinput_input_event;
//	input_dev->open = hidinput_open;
//	input_dev->close = hidinput_close;
//	input_dev->setkeycode = hidinput_setkeycode;
//	input_dev->getkeycode = hidinput_getkeycode;
//
//	input_dev->name = hid->name;
//	input_dev->phys = hid->phys;
//	input_dev->uniq = hid->uniq;
//	input_dev->id.bustype = hid->bus;
//	input_dev->id.vendor  = hid->vendor;
//	input_dev->id.product = hid->product;
//	input_dev->id.version = hid->version;
//	input_dev->dev.parent = hid->dev.parent;

//  hidinput->input = input_dev;



//	list_add_tail(&hidinput->list, &hid->inputs);

//	memcpy(&hid->hidinput_list[hid->hidinput_list_size], hidinput, sizeof(struct hid_input));
  // TODO set limit
	hid->hidinput_list_size++;

	return hidinput;
}

static bool hidinput_has_been_populated(struct hid_input *hidinput)
{
	int i;
	unsigned long r = 0;

	for (i = 0; i < BITS_TO_LONGS(EV_CNT); i++)
		r |= hidinput->input.evbit[i];

	for (i = 0; i < BITS_TO_LONGS(KEY_CNT); i++)
		r |= hidinput->input.keybit[i];

	for (i = 0; i < BITS_TO_LONGS(REL_CNT); i++)
		r |= hidinput->input.relbit[i];

	for (i = 0; i < BITS_TO_LONGS(ABS_CNT); i++)
		r |= hidinput->input.absbit[i];

	for (i = 0; i < BITS_TO_LONGS(MSC_CNT); i++)
		r |= hidinput->input.mscbit[i];

	for (i = 0; i < BITS_TO_LONGS(LED_CNT); i++)
		r |= hidinput->input.ledbit[i];

	for (i = 0; i < BITS_TO_LONGS(SND_CNT); i++)
		r |= hidinput->input.sndbit[i];

	for (i = 0; i < BITS_TO_LONGS(FF_CNT); i++)
		r |= hidinput->input.ffbit[i];

	for (i = 0; i < BITS_TO_LONGS(SW_CNT); i++)
		r |= hidinput->input.swbit[i];

	return !!r;
}

/*
 * since hidinput cannot be freed in our modification, this clean-up
 * only clean references inside report field.
 */
static void hidinput_cleanup_hidinput(struct hid_device *hid, struct hid_input *hidinput)
{
  struct hid_report *report;
  int i, j, k;

// list_del(&hidinput->list);
// input_free_device(hidinput->input);
// free(hidinput->input);

  for (k = HID_INPUT_REPORT; k <= HID_OUTPUT_REPORT; k++)
  {
    if (k == HID_OUTPUT_REPORT && (hid->quirks & HID_QUIRK_SKIP_OUTPUT_REPORTS))
      continue;

//    list_for_each_entry(report, &hid->report_enum[k].report_list, list)
//    {
//
//      for (i = 0; i < report->maxfield; i++)
//        if (report->field[i]->hidinput == hidinput)
//          report->field[i]->hidinput = NULL;
//    }

    for (j = 0; j < hid->report_enum[k].report_array_size; j++) {

      report = &hid->report_enum[k].report_array[j];

      /** remove all reference inside report_field to this hidinput **/
      for (i = 0; i < report->maxfield; i++) {
        if (report->field[i]->hidinput == hidinput)
          report->field[i]->hidinput = NULL;
      }
    }
  }

  // kfree(hidinput);
  // free(hidinput);
  hid->hidinput_list_size--;
}

/*
 * Register the input device; print a message.
 * Configure the input layer interface
 * Read all reports and initialize the absolute field values.
 */
int hidinput_connect(struct hid_device *hid, unsigned int force)
{
  struct hid_report *report;
  struct hid_input *hidinput = NULL;
  int i, j, k, x;

//  INIT_LIST_HEAD(&hid->inputs);
//  INIT_WORK(&hid->led_work, hidinput_led_worker);

  hid->hidinput_list_size = 0;

  if (!force)
  {
    for (i = 0; i < hid->maxcollection; i++)
    {
      struct hid_collection *col = &hid->collection[i];
      if (col->type == HID_COLLECTION_APPLICATION
          || col->type == HID_COLLECTION_PHYSICAL)
        if (IS_INPUT_APPLICATION(col->usage))
          break;
    }

    if (i == hid->maxcollection)
      return -1;
  }

  report_features(hid);   // null implementation

  for (k = HID_INPUT_REPORT; k <= HID_OUTPUT_REPORT; k++)
  {
    if (k == HID_OUTPUT_REPORT && (hid->quirks & HID_QUIRK_SKIP_OUTPUT_REPORTS))
      continue;

    for (x = 0; x < hid->report_enum[k].report_array_size; x++)
    {
      report = &hid->report_enum[k].report_array[x];

      if (!report->maxfield)
        continue;

      if (!hidinput)
      {
        hidinput = hidinput_allocate(hid);
        if (!hidinput)
          goto out_unwind;
      }

      for (i = 0; i < report->maxfield; i++)
      {
        for (j = 0; j < report->field[i]->maxusage; j++)
        {
          hidinput_configure_usage(hidinput, report->field[i],
              report->field[i]->usage + j);
        }
      }

      if ((hid->quirks & HID_QUIRK_NO_EMPTY_INPUT) &&
          !hidinput_has_been_populated(hidinput))
          continue;

//          if (hid->quirks & HID_QUIRK_MULTI_INPUT) {
//              /* This will leave hidinput NULL, so that it
//               * allocates another one if we have more inputs on
//               * the same interface. Some devices (e.g. Happ's
//               * UGCI) cram a lot of unrelated inputs into the
//               * same interface. */
//              hidinput->report = report;
//              if (drv->input_configured)
//                  drv->input_configured(hid, hidinput);
//              if (input_register_device(hidinput->input))
//                  goto out_cleanup;
//              hidinput = NULL;
//          }
    }
  }

  if (hidinput && (hid->quirks & HID_QUIRK_NO_EMPTY_INPUT)
      && !hidinput_has_been_populated(hidinput))
  {
    /* no need to register an input device not populated */
    hidinput_cleanup_hidinput(hid, hidinput);
    // hidinput = NULL;
  }

//  if (list_empty(&hid->inputs))
//  {
//    hid_err(hid, "No inputs registered, leaving\n");
//    goto out_unwind;
//  }

  // EQUIVALENT
  if (hid->hidinput_list_size == 0)
  {
    hid_err(hid, "No inputs registered, leaving\n");
    goto out_unwind;
  }



//	if (hidinput) {
//		if (drv->input_configured)
//			drv->input_configured(hid, hidinput);
//		if (input_register_device(hidinput->input))
//			goto out_cleanup;
//	}

  return 0;

//  out_cleanup:
    // list_del(&hidinput->list);
    // input_free_device(hidinput->input);
    // kfree(hidinput);
    // free(hidinput);

  out_unwind:
    /* unwind the ones we already registered */
    hidinput_disconnect(hid);
    // TODO unwind disabled for failure
    return -1;
}
// EXPORT_SYMBOL_GPL(hidinput_connect);


void hidinput_disconnect(struct hid_device *hid)
{
//  struct hid_input *hidinput, *next;
//  int i;
//  hidinput_cleanup_battery(hid);

//  list_for_each_entry_safe(hidinput, next, &hid->inputs, list)
//  {
//    list_del(&hidinput->list);
//    // input_unregister_device(hidinput->input);
//    // kfree(hidinput);
//    free(hidinput);
//  }

//  for (i = 0; i < hid->hidinput_list_size; i++) {
//    if (hid->hidinput_list[i].input) {
//      free(hid->hidinput_list[i].input);
//    }
//  }

  USBH_UsrLog("%s", __func__);
  hid->hidinput_list_size = 0;

  /* led_work is spawned by input_dev callbacks, but doesn't access the
   * parent input_dev at all. Once all input devices are removed, we
   * know that led_work will never get restarted, so we can cancel it
   * synchronously and are safe. */
  // cancel_work_sync(&hid->led_work);
}
// EXPORT_SYMBOL_GPL(hidinput_disconnect);


