#pragma once
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

struct MouseOffset {
  float xOffset;
  float yOffset;
};
