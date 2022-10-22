//
// Created by Garrett on 11/27/21.
//

#ifndef GLW_KEY_CODES_HPP
#define GLW_KEY_CODES_HPP

#include <iostream>

namespace glw {

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

} key;

inline std::ostream &operator<<(std::ostream &os, key code)
{
    os << static_cast<int32_t>(code);
    return os;
}

// From glfw3.h
#define KEY_SPACE           ::key::Space
#define KEY_APOSTROPHE      ::key::Apostrophe    /* ' */
#define KEY_COMMA           ::key::Comma         /* , */
#define KEY_MINUS           ::key::Minus         /* - */
#define KEY_PERIOD          ::key::Period        /* . */
#define KEY_SLASH           ::key::Slash         /* / */
#define KEY_0               ::key::D0
#define KEY_1               ::key::D1
#define KEY_2               ::key::D2
#define KEY_3               ::key::D3
#define KEY_4               ::key::D4
#define KEY_5               ::key::D5
#define KEY_6               ::key::D6
#define KEY_7               ::key::D7
#define KEY_8               ::key::D8
#define KEY_9               ::key::D9
#define KEY_SEMICOLON       ::key::Semicolon     /* ; */
#define KEY_EQUAL           ::key::Equal         /* = */
#define KEY_A               ::key::A
#define KEY_B               ::key::B
#define KEY_C               ::key::C
#define KEY_D               ::key::D
#define KEY_E               ::key::E
#define KEY_F               ::key::F
#define KEY_G               ::key::G
#define KEY_H               ::key::H
#define KEY_I               ::key::I
#define KEY_J               ::key::J
#define KEY_K               ::key::K
#define KEY_L               ::key::L
#define KEY_M               ::key::M
#define KEY_N               ::key::N
#define KEY_O               ::key::O
#define KEY_P               ::key::P
#define KEY_Q               ::key::Q
#define KEY_R               ::key::R
#define KEY_S               ::key::S
#define KEY_T               ::key::T
#define KEY_U               ::key::U
#define KEY_V               ::key::V
#define KEY_W               ::key::W
#define KEY_X               ::key::X
#define KEY_Y               ::key::Y
#define KEY_Z               ::key::Z
#define KEY_LEFT_BRACKET    ::key::LeftBracket   /* [ */
#define KEY_BACKSLASH       ::key::Backslash     /* \ */
#define KEY_RIGHT_BRACKET   ::key::RightBracket  /* ] */
#define KEY_GRAVE_ACCENT    ::key::GraveAccent   /* ` */
#define KEY_WORLD_1         ::key::World1        /* non-US #1 */
#define KEY_WORLD_2         ::key::World2        /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE          ::key::Escape
#define KEY_ENTER           ::key::Enter
#define KEY_TAB             ::key::Tab
#define KEY_BACKSPACE       ::key::Backspace
#define KEY_INSERT          ::key::Insert
#define KEY_DELETE          ::key::Delete
#define KEY_RIGHT           ::key::Right
#define KEY_LEFT            ::key::Left
#define KEY_DOWN            ::key::Down
#define KEY_UP              ::key::Up
#define KEY_PAGE_UP         ::key::PageUp
#define KEY_PAGE_DOWN       ::key::PageDown
#define KEY_HOME            ::key::Home
#define KEY_END             ::key::End
#define KEY_CAPS_LOCK       ::key::CapsLock
#define KEY_SCROLL_LOCK     ::key::ScrollLock
#define KEY_NUM_LOCK        ::key::NumLock
#define KEY_PRINT_SCREEN    ::key::PrintScreen
#define KEY_PAUSE           ::key::Pause
#define KEY_F1              ::key::F1
#define KEY_F2              ::key::F2
#define KEY_F3              ::key::F3
#define KEY_F4              ::key::F4
#define KEY_F5              ::key::F5
#define KEY_F6              ::key::F6
#define KEY_F7              ::key::F7
#define KEY_F8              ::key::F8
#define KEY_F9              ::key::F9
#define KEY_F10             ::key::F10
#define KEY_F11             ::key::F11
#define KEY_F12             ::key::F12
#define KEY_F13             ::key::F13
#define KEY_F14             ::key::F14
#define KEY_F15             ::key::F15
#define KEY_F16             ::key::F16
#define KEY_F17             ::key::F17
#define KEY_F18             ::key::F18
#define KEY_F19             ::key::F19
#define KEY_F20             ::key::F20
#define KEY_F21             ::key::F21
#define KEY_F22             ::key::F22
#define KEY_F23             ::key::F23
#define KEY_F24             ::key::F24
#define KEY_F25             ::key::F25

/* keypad */
#define KEY_KP_0            ::key::KP0
#define KEY_KP_1            ::key::KP1
#define KEY_KP_2            ::key::KP2
#define KEY_KP_3            ::key::KP3
#define KEY_KP_4            ::key::KP4
#define KEY_KP_5            ::key::KP5
#define KEY_KP_6            ::key::KP6
#define KEY_KP_7            ::key::KP7
#define KEY_KP_8            ::key::KP8
#define KEY_KP_9            ::key::KP9
#define KEY_KP_DECIMAL      ::key::KPDecimal
#define KEY_KP_DIVIDE       ::key::KPDivide
#define KEY_KP_MULTIPLY     ::key::KPMultiply
#define KEY_KP_SUBTRACT     ::key::KPSubtract
#define KEY_KP_ADD          ::key::KPAdd
#define KEY_KP_ENTER        ::key::KPEnter
#define KEY_KP_EQUAL        ::key::KPEqual

#define KEY_LEFT_SHIFT      ::key::LeftShift
#define KEY_LEFT_CONTROL    ::key::LeftControl
#define KEY_LEFT_ALT        ::key::LeftAlt
#define KEY_LEFT_SUPER      ::key::LeftSuper
#define KEY_RIGHT_SHIFT     ::key::RightShift
#define KEY_RIGHT_CONTROL   ::key::RightControl
#define KEY_RIGHT_ALT       ::key::RightAlt
#define KEY_RIGHT_SUPER     ::key::RightSuper
#define KEY_MENU            ::key::Menu

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

#define MOUSE_BTN_0 ::mouse_btn::BUTTON0
#define MOUSE_BTN_1 ::mouse_btn::BUTTON1
#define MOUSE_BTN_2 ::mouse_btn::BUTTON2
#define MOUSE_BTN_3 ::mouse_btn::BUTTON3
#define MOUSE_BTN_4 ::mouse_btn::BUTTON4
#define MOUSE_BTN_5 ::mouse_btn::BUTTON5
#define MOUSE_BTN_LEFT ::mouse_btn::LEFT
#define MOUSE_BTN_RIGHT ::mouse_btn::RIGHT
#define MOUSE_BTN_MIDDLE ::mouse_btn::MIDDLE

}

#endif //GLW_KEY_CODES_HPP
