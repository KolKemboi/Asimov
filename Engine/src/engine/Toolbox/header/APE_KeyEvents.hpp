#pragma once

#include <cstdint>

// Alicia Keys
enum class Alicia : std::uint64_t {
  // QWERTY
  A = 1LL << 0,
  B = 1LL << 1,
  C = 1LL << 2,
  D = 1LL << 3,
  E = 1LL << 4,
  F = 1LL << 5,
  G = 1LL << 6,
  H = 1LL << 7,
  I = 1LL << 8,
  J = 1LL << 9,
  K = 1LL << 10,
  L = 1LL << 11,
  M = 1LL << 12,
  N = 1LL << 13,
  O = 1LL << 14,
  P = 1LL << 15,
  Q = 1LL << 16,
  R = 1LL << 17,
  S = 1LL << 18,
  T = 1LL << 19,
  U = 1LL << 20,
  V = 1LL << 21,
  W = 1LL << 22,
  X = 1LL << 23,
  Y = 1LL << 24,
  Z = 1LL << 25,
  // mouse keys
  LEFT = 1LL << 26,
  RIGHT = 1LL << 27,
  MIDDLE = 1LL << 28,
  // modifier keys
  SHIFT = 1LL << 29,
  CTRL = 1LL << 30,
  ALT = 1LL << 31,
  // HOME, DELETE, ESCAPE
  HOME = 1LL << 32,
  DELETE = 1LL << 33,
  ESCAPE = 1LL << 34,
  // MOUSE DIR
  // Signed Bit Notation
  // if bit 35 | 37 -> 0, ignore bit 36 & 38
  // else if bit 35 | 37 -> 1, check bit 36 & 38 for the sign
  MOUSE_ON_X = 1LL << 35,
  MOUSE_X_SIGN = 1LL << 36,
  MOUSE_ON_Y = 1LL << 37,
  MOUSE_Y_SIGN = 1LL << 38,
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
