// halcyon_types.h

#ifndef HALCYON_TYPES_H_INCLUDED
#define HALCYON_TYPES_H_INCLUDED

#include <stdint.h>

enum keycode_type_id
{
   KEYCODE_BACK = 0x08,
   KEYCODE_TAB = 0x09,
   KEYCODE_CLEAR = 0x0C,
   KEYCODE_RETURN = 0x0D,
   KEYCODE_SHIFT = 0x10,
   KEYCODE_CONTROL = 0x11,
   KEYCODE_MENU = 0x12,
   KEYCODE_PAUSE = 0x13,
   KEYCODE_CAPITAL = 0x14,
   KEYCODE_KANA = 0x15,
   KEYCODE_JUNJA = 0x17,
   KEYCODE_FINAL = 0x18,
   KEYCODE_HANJA = 0x19,
   KEYCODE_ESCAPE = 0x1B,
   KEYCODE_CONVERT = 0x1C,
   KEYCODE_NONCONVERT = 0x1D,
   KEYCODE_ACCEPT = 0x1E,
   KEYCODE_MODECHANGE = 0x1F,
   KEYCODE_SPACE = 0x20,
   KEYCODE_PRIOR = 0x21,
   KEYCODE_NEXT = 0x22,
   KEYCODE_END = 0x23,
   KEYCODE_HOME = 0x24,
   KEYCODE_LEFT = 0x25,
   KEYCODE_UP = 0x26,
   KEYCODE_RIGHT = 0x27,
   KEYCODE_DOWN = 0x28,
   KEYCODE_SELECT = 0x29,
   KEYCODE_PRINT = 0x2A,
   KEYCODE_EXECUTE = 0x2B,
   KEYCODE_SNAPSHOT = 0x2C,
   KEYCODE_INSERT = 0x2D,
   KEYCODE_DELETE = 0x2E,
   KEYCODE_HELP = 0x2F,
   KEYCODE_KEY_0 = 0x30,
   KEYCODE_KEY_1 = 0x31,
   KEYCODE_KEY_2 = 0x32,
   KEYCODE_KEY_3 = 0x33,
   KEYCODE_KEY_4 = 0x34,
   KEYCODE_KEY_5 = 0x35,
   KEYCODE_KEY_6 = 0x36,
   KEYCODE_KEY_7 = 0x37,
   KEYCODE_KEY_8 = 0x38,
   KEYCODE_KEY_9 = 0x39,
   KEYCODE_KEY_A = 0x41,
   KEYCODE_KEY_B = 0x42,
   KEYCODE_KEY_C = 0x43,
   KEYCODE_KEY_D = 0x44,
   KEYCODE_KEY_E = 0x45,
   KEYCODE_KEY_F = 0x46,
   KEYCODE_KEY_G = 0x47,
   KEYCODE_KEY_H = 0x48,
   KEYCODE_KEY_I = 0x49,
   KEYCODE_KEY_J = 0x4A,
   KEYCODE_KEY_K = 0x4B,
   KEYCODE_KEY_L = 0x4C,
   KEYCODE_KEY_M = 0x4D,
   KEYCODE_KEY_N = 0x4E,
   KEYCODE_KEY_O = 0x4F,
   KEYCODE_KEY_P = 0x50,
   KEYCODE_KEY_Q = 0x51,
   KEYCODE_KEY_R = 0x52,
   KEYCODE_KEY_S = 0x53,
   KEYCODE_KEY_T = 0x54,
   KEYCODE_KEY_U = 0x55,
   KEYCODE_KEY_V = 0x56,
   KEYCODE_KEY_W = 0x57,
   KEYCODE_KEY_X = 0x58,
   KEYCODE_KEY_Y = 0x59,
   KEYCODE_KEY_Z = 0x5A,
   KEYCODE_LWIN = 0x5B,
   KEYCODE_RWIN = 0x5C,
   KEYCODE_APPS = 0x5D,
   KEYCODE_SLEEP = 0x5F,
   KEYCODE_NUMPAD0 = 0x60,
   KEYCODE_NUMPAD1 = 0x61,
   KEYCODE_NUMPAD2 = 0x62,
   KEYCODE_NUMPAD3 = 0x63,
   KEYCODE_NUMPAD4 = 0x64,
   KEYCODE_NUMPAD5 = 0x65,
   KEYCODE_NUMPAD6 = 0x66,
   KEYCODE_NUMPAD7 = 0x67,
   KEYCODE_NUMPAD8 = 0x68,
   KEYCODE_NUMPAD9 = 0x69,
   KEYCODE_MULTIPLY = 0x6A,
   KEYCODE_ADD = 0x6B,
   KEYCODE_SEPARATOR = 0x6C,
   KEYCODE_SUBTRACT = 0x6D,
   KEYCODE_DECIMAL = 0x6E,
   KEYCODE_DIVIDE = 0x6F,
   KEYCODE_F1 = 0x70,
   KEYCODE_F2 = 0x71,
   KEYCODE_F3 = 0x72,
   KEYCODE_F4 = 0x73,
   KEYCODE_F5 = 0x74,
   KEYCODE_F6 = 0x75,
   KEYCODE_F7 = 0x76,
   KEYCODE_F8 = 0x77,
   KEYCODE_F9 = 0x78,
   KEYCODE_F10 = 0x79,
   KEYCODE_F11 = 0x7A,
   KEYCODE_F12 = 0x7B,
   KEYCODE_F13 = 0x7C,
   KEYCODE_F14 = 0x7D,
   KEYCODE_F15 = 0x7E,
   KEYCODE_F16 = 0x7F,
   KEYCODE_F17 = 0x80,
   KEYCODE_F18 = 0x81,
   KEYCODE_F19 = 0x82,
   KEYCODE_F20 = 0x83,
   KEYCODE_F21 = 0x84,
   KEYCODE_F22 = 0x85,
   KEYCODE_F23 = 0x86,
   KEYCODE_F24 = 0x87,
   KEYCODE_NUMLOCK = 0x90,
   KEYCODE_SCROLL = 0x91,
   KEYCODE_LSHIFT = 0xA0,
   KEYCODE_RSHIFT = 0xA1,
   KEYCODE_LCONTROL = 0xA2,
   KEYCODE_RCONTROL = 0xA3,
   KEYCODE_LMENU = 0xA4,
   KEYCODE_RMENU = 0xA5,
   KEYCODE_BROWSER_BACK = 0xA6,
   KEYCODE_BROWSER_FORWARD = 0xA7,
   KEYCODE_BROWSER_REFRESH = 0xA8,
   KEYCODE_BROWSER_STOP = 0xA9,
   KEYCODE_BROWSER_SEARCH = 0xAA,
   KEYCODE_BROWSER_FAVORITES = 0xAB,
   KEYCODE_BROWSER_HOME = 0xAC,
   KEYCODE_VOLUME_MUTE = 0xAD,
   KEYCODE_VOLUME_DOWN = 0xAE,
   KEYCODE_VOLUME_UP = 0xAF,
   KEYCODE_MEDIA_NEXT_TRACK = 0xB0,
   KEYCODE_MEDIA_PREV_TRACK = 0xB1,
   KEYCODE_MEDIA_STOP = 0xB2,
   KEYCODE_MEDIA_PLAY_PAUSE = 0xB3,
   KEYCODE_LAUNCH_MAIL = 0xB4,
   KEYCODE_MEDIA_SELECT = 0xB5,
   KEYCODE_LAUNCH_APP1 = 0xB6,
   KEYCODE_LAUNCH_APP2 = 0xB7,
   KEYCODE_OEM_1 = 0xBA,
   KEYCODE_OEM_PLUS = 0xBB,
   KEYCODE_OEM_COMMA = 0xBC,
   KEYCODE_OEM_MINUS = 0xBD,
   KEYCODE_OEM_PERIOD = 0xBE,
   KEYCODE_OEM_2 = 0xBF,
   KEYCODE_OEM_3 = 0xC0,
   KEYCODE_ABNT_C1 = 0xC1,
   KEYCODE_ABNT_C2 = 0xC2,
   KEYCODE_OEM_4 = 0xDB,
   KEYCODE_OEM_5 = 0xDC,
   KEYCODE_OEM_6 = 0xDD,
   KEYCODE_OEM_7 = 0xDE,
   KEYCODE_OEM_8 = 0xDF,
   KEYCODE_OEM_102 = 0xE2,
   KEYCODE_PROCESSKEY = 0xE5,
   KEYCODE_PACKET = 0xE7,
   KEYCODE_ATTN = 0xF6,
   KEYCODE_CRSEL = 0xF7,
   KEYCODE_EXSEL = 0xF8,
   KEYCODE_EREOF = 0xF9,
   KEYCODE_PLAY = 0xFA,
   KEYCODE_ZOOM = 0xFB,
   KEYCODE_NONAME = 0xFC,
   KEYCODE_PA1 = 0xFD,
   KEYCODE_OEM_CLEAR = 0xFE,
   KEYCODE_COUNT = 256,
};

enum mouse_button_type_id
{
   MOUSE_BUTTON_LEFT,
   MOUSE_BUTTON_RIGHT,
   MOUSE_BUTTON_MIDDLE,
   MOUSE_BUTTON_EXTRA_1,
   MOUSE_BUTTON_EXTRA_2,
   MOUSE_BUTTON_COUNT,
};

struct button_state
{
   bool down_;
   bool pressed_;
   bool released_;
};

struct mouse_device
{
   int x_;
   int y_;
   button_state buttons_[MOUSE_BUTTON_COUNT];
};

struct key_state
{
   bool down_;
   bool pressed_;
   bool released_;
};

struct keyboard_device
{
   key_state keys_[KEYCODE_COUNT];
};

#pragma warning(push)
#pragma warning(disable:4201) // nonstandard extension used: nameless struct/union

union v2
{
   struct
   {
      float x_;
      float y_;
   };
   float e_[2];
};

union v3
{
   struct
   {
      float x_;
      float y_;
      float z_;
   };
   struct
   {
      v2 xy_;
   };
   float e_[3];
};

union v4
{
   struct
   {
      float x_;
      float y_;
      float z_;
      float w_;
   };
   struct
   {
      float r_;
      float g_;
      float b_;
      float a_;
   };
   struct
   {
      v2 xy_;
      v2 zw_;
   };
   struct
   {
      v3 xyz_;
   };
   float e_[4];
};

union m4
{
   struct
   {
      v4 x_;
      v4 y_;
      v4 z_;
      v4 w_;
   };
   float e_[16];
};

#pragma warning(pop)

#endif // HALCYON_TYPES_H_INCLUDED
