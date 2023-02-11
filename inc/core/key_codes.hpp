//
// Created by Garrett on 11/27/21.
//

#ifndef MB2DC_KEY_CODES_HPP
#define MB2DC_KEY_CODES_HPP

#include <iostream>

namespace mb2dc {

typedef enum class key_code : uint16_t {
  // From glfw3.h
  Space = 32,
  Apostrophe = 39, /* ' */
  Comma = 44, /* , */
  Minus = 45, /* - */
  Period = 46, /* . */
  Slash = 47, /* / */

  D0 = 48, /* 0 */
  D1 = 49, /* 1 */
  D2 = 50, /* 2 */
  D3 = 51, /* 3 */
  D4 = 52, /* 4 */
  D5 = 53, /* 5 */
  D6 = 54, /* 6 */
  D7 = 55, /* 7 */
  D8 = 56, /* 8 */
  D9 = 57, /* 9 */

  Semicolon = 59, /* ; */
  Equal = 61, /* = */

  A = 65,
  B = 66,
  C = 67,
  D = 68,
  E = 69,
  F = 70,
  G = 71,
  H = 72,
  I = 73,
  J = 74,
  K = 75,
  L = 76,
  M = 77,
  N = 78,
  O = 79,
  P = 80,
  Q = 81,
  R = 82,
  S = 83,
  T = 84,
  U = 85,
  V = 86,
  W = 87,
  X = 88,
  Y = 89,
  Z = 90,

  LeftBracket = 91,  /* [ */
  Backslash = 92,  /* \ */
  RightBracket = 93,  /* ] */
  GraveAccent = 96,  /* ` */

  World1 = 161, /* non-US #1 */
  World2 = 162, /* non-US #2 */

  /* Function keys */
  Escape = 256,
  Enter = 257,
  Tab = 258,
  Backspace = 259,
  Insert = 260,
  Delete = 261,
  Right = 262,
  Left = 263,
  Down = 264,
  Up = 265,
  PageUp = 266,
  PageDown = 267,
  Home = 268,
  End = 269,
  CapsLock = 280,
  ScrollLock = 281,
  NumLock = 282,
  PrintScreen = 283,
  Pause = 284,
  F1 = 290,
  F2 = 291,
  F3 = 292,
  F4 = 293,
  F5 = 294,
  F6 = 295,
  F7 = 296,
  F8 = 297,
  F9 = 298,
  F10 = 299,
  F11 = 300,
  F12 = 301,
  F13 = 302,
  F14 = 303,
  F15 = 304,
  F16 = 305,
  F17 = 306,
  F18 = 307,
  F19 = 308,
  F20 = 309,
  F21 = 310,
  F22 = 311,
  F23 = 312,
  F24 = 313,
  F25 = 314,

  /* Keypad */
  KP0 = 320,
  KP1 = 321,
  KP2 = 322,
  KP3 = 323,
  KP4 = 324,
  KP5 = 325,
  KP6 = 326,
  KP7 = 327,
  KP8 = 328,
  KP9 = 329,
  KPDecimal = 330,
  KPDivide = 331,
  KPMultiply = 332,
  KPSubtract = 333,
  KPAdd = 334,
  KPEnter = 335,
  KPEqual = 336,

  LeftShift = 340,
  LeftControl = 341,
  LeftAlt = 342,
  LeftSuper = 343,
  RightShift = 344,
  RightControl = 345,
  RightAlt = 346,
  RightSuper = 347,
  Menu = 348

} key_code;

inline std::ostream &operator<<(std::ostream &os, key_code code)
{
    os << static_cast<int32_t>(code);
    return os;
}

// From glfw3.h
#define KEY_SPACE           key_code::Space
#define KEY_APOSTROPHE      key_code::Apostrophe    /* ' */
#define KEY_COMMA           key_code::Comma         /* , */
#define KEY_MINUS           key_code::Minus         /* - */
#define KEY_PERIOD          key_code::Period        /* . */
#define KEY_SLASH           key_code::Slash         /* / */
#define KEY_0               key_code::D0
#define KEY_1               key_code::D1
#define KEY_2               key_code::D2
#define KEY_3               key_code::D3
#define KEY_4               key_code::D4
#define KEY_5               key_code::D5
#define KEY_6               key_code::D6
#define KEY_7               key_code::D7
#define KEY_8               key_code::D8
#define KEY_9               key_code::D9
#define KEY_SEMICOLON       key_code::Semicolon     /* ; */
#define KEY_EQUAL           key_code::Equal         /* = */
#define KEY_A               key_code::A
#define KEY_B               key_code::B
#define KEY_C               key_code::C
#define KEY_D               key_code::D
#define KEY_E               key_code::E
#define KEY_F               key_code::F
#define KEY_G               key_code::G
#define KEY_H               key_code::H
#define KEY_I               key_code::I
#define KEY_J               key_code::J
#define KEY_K               key_code::K
#define KEY_L               key_code::L
#define KEY_M               key_code::M
#define KEY_N               key_code::N
#define KEY_O               key_code::O
#define KEY_P               key_code::P
#define KEY_Q               key_code::Q
#define KEY_R               key_code::R
#define KEY_S               key_code::S
#define KEY_T               key_code::T
#define KEY_U               key_code::U
#define KEY_V               key_code::V
#define KEY_W               key_code::W
#define KEY_X               key_code::X
#define KEY_Y               key_code::Y
#define KEY_Z               key_code::Z
#define KEY_LEFT_BRACKET    key_code::LeftBracket   /* [ */
#define KEY_BACKSLASH       key_code::Backslash     /* \ */
#define KEY_RIGHT_BRACKET   key_code::RightBracket  /* ] */
#define KEY_GRAVE_ACCENT    key_code::GraveAccent   /* ` */
#define KEY_WORLD_1         key_code::World1        /* non-US #1 */
#define KEY_WORLD_2         key_code::World2        /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE          key_code::Escape
#define KEY_ENTER           key_code::Enter
#define KEY_TAB             key_code::Tab
#define KEY_BACKSPACE       key_code::Backspace
#define KEY_INSERT          key_code::Insert
#define KEY_DELETE          key_code::Delete
#define KEY_RIGHT           key_code::Right
#define KEY_LEFT            key_code::Left
#define KEY_DOWN            key_code::Down
#define KEY_UP              key_code::Up
#define KEY_PAGE_UP         key_code::PageUp
#define KEY_PAGE_DOWN       key_code::PageDown
#define KEY_HOME            key_code::Home
#define KEY_END             key_code::End
#define KEY_CAPS_LOCK       key_code::CapsLock
#define KEY_SCROLL_LOCK     key_code::ScrollLock
#define KEY_NUM_LOCK        key_code::NumLock
#define KEY_PRINT_SCREEN    key_code::PrintScreen
#define KEY_PAUSE           key_code::Pause
#define KEY_F1              key_code::F1
#define KEY_F2              key_code::F2
#define KEY_F3              key_code::F3
#define KEY_F4              key_code::F4
#define KEY_F5              key_code::F5
#define KEY_F6              key_code::F6
#define KEY_F7              key_code::F7
#define KEY_F8              key_code::F8
#define KEY_F9              key_code::F9
#define KEY_F10             key_code::F10
#define KEY_F11             key_code::F11
#define KEY_F12             key_code::F12
#define KEY_F13             key_code::F13
#define KEY_F14             key_code::F14
#define KEY_F15             key_code::F15
#define KEY_F16             key_code::F16
#define KEY_F17             key_code::F17
#define KEY_F18             key_code::F18
#define KEY_F19             key_code::F19
#define KEY_F20             key_code::F20
#define KEY_F21             key_code::F21
#define KEY_F22             key_code::F22
#define KEY_F23             key_code::F23
#define KEY_F24             key_code::F24
#define KEY_F25             key_code::F25

/* keypad */
#define KEY_KP_0            key_code::KP0
#define KEY_KP_1            key_code::KP1
#define KEY_KP_2            key_code::KP2
#define KEY_KP_3            key_code::KP3
#define KEY_KP_4            key_code::KP4
#define KEY_KP_5            key_code::KP5
#define KEY_KP_6            key_code::KP6
#define KEY_KP_7            key_code::KP7
#define KEY_KP_8            key_code::KP8
#define KEY_KP_9            key_code::KP9
#define KEY_KP_DECIMAL      key_code::KPDecimal
#define KEY_KP_DIVIDE       key_code::KPDivide
#define KEY_KP_MULTIPLY     key_code::KPMultiply
#define KEY_KP_SUBTRACT     key_code::KPSubtract
#define KEY_KP_ADD          key_code::KPAdd
#define KEY_KP_ENTER        key_code::KPEnter
#define KEY_KP_EQUAL        key_code::KPEqual

#define KEY_LEFT_SHIFT      key_code::LeftShift
#define KEY_LEFT_CONTROL    key_code::LeftControl
#define KEY_LEFT_ALT        key_code::LeftAlt
#define KEY_LEFT_SUPER      key_code::LeftSuper
#define KEY_RIGHT_SHIFT     key_code::RightShift
#define KEY_RIGHT_CONTROL   key_code::RightControl
#define KEY_RIGHT_ALT       key_code::RightAlt
#define KEY_RIGHT_SUPER     key_code::RightSuper
#define KEY_MENU            key_code::Menu

typedef enum class mouse_button : uint16_t {
  BUTTON0 = 0,
  BUTTON1 = 1,
  BUTTON2 = 2,
  BUTTON3 = 3,
  BUTTON4 = 4,
  BUTTON5 = 5,
  LEFT = BUTTON0,
  RIGHT = BUTTON1,
  MIDDLE = BUTTON2,
} mouse_btn;

#define MOUSE_BTN_0 mouse_btn::BUTTON0
#define MOUSE_BTN_1 mouse_btn::BUTTON1
#define MOUSE_BTN_2 mouse_btn::BUTTON2
#define MOUSE_BTN_3 mouse_btn::BUTTON3
#define MOUSE_BTN_4 mouse_btn::BUTTON4
#define MOUSE_BTN_5 mouse_btn::BUTTON5
#define MOUSE_BTN_LEFT mouse_btn::LEFT
#define MOUSE_BTN_RIGHT mouse_btn::RIGHT
#define MOUSE_BTN_MIDDLE mouse_btn::MIDDLE

}

#endif
