#pragma once

#include <cstdint>

// Alicia Keys
enum class Alicia : std::uint64_t {
  // QWERTY
  A = 1UL << 0,
  B = 1UL << 1,
  C = 1UL << 2,
  D = 1UL << 3,
  E = 1UL << 4,
  F = 1UL << 5,
  G = 1UL << 6,
  H = 1UL << 7,
  I = 1UL << 8,
  J = 1UL << 9,
  K = 1UL << 10,
  L = 1UL << 11,
  M = 1UL << 12,
  N = 1UL << 13,
  O = 1UL << 14,
  P = 1UL << 15,
  Q = 1UL << 16,
  R = 1UL << 17,
  S = 1UL << 18,
  T = 1UL << 19,
  U = 1UL << 20,
  V = 1UL << 21,
  W = 1UL << 22,
  X = 1UL << 23,
  Y = 1UL << 24,
  Z = 1UL << 25,
  // mouse keys
  LEFT = 1UL << 26,
  RIGHT = 1UL << 27,
  MIDDLE = 1UL << 28,
  // modifier keys
  SHIFT = 1UL << 29,
  CTRL = 1UL << 30,
  ALT = 1UL << 31,
  // HOME, DELETE, ESCAPE
  HOME = 1UL << 32,
  DELETE = 1UL << 33,
  ESCAPE = 1UL << 34,
  // MOUSE DIR
  // Signed Bit Notation
  // if bit 35 | 37 -> 0, ignore bit 36 & 38
  // else if bit 35 | 37 -> 1, check bit 36 & 38 for the sign
  MOUSE_ON_X = 1UL << 35,
  MOUSE_X_SIGN = 1UL << 36,
  MOUSE_ON_Y = 1UL << 37,
  MOUSE_Y_SIGN = 1UL << 38,
};

// Jerry, Mouse from tom and Jerry
enum class Jerry : std::uint32_t {
  MOUSE_X_POS = 0xFFFFu << 0,
  MOUSE_Y_POS = 0xFFFFu << 16,
};

enum class KeyPress {
  Q,
  W,
  E,
  R,
  T,
  Y,
  U,
  I,
  O,
  P,
  A,
  S,
  D,
  F,
  G,
  H,
  J,
  K,
  L,
  Z,
  X,
  C,
  V,
  B,
  N,
  M,
  HOME,
  DELETE,
};
enum class ModKeys {
  CTRL,
  ALT,
  SHIFT,
};
enum class MouseButtonPress {
  LEFT,
  RIGHT,
  MIDDLE,
};

struct KeyData {
  KeyPress key;
  ModKeys mod;
};

struct MouseOffset {
  float xOffset;
  float yOffset;
};
