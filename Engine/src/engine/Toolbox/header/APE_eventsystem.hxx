#pragma once

#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>
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
  CTRL,
  ALT,
  SHIFT,
  HOME,

};
enum class MouseButtonPress {
  LEFT,
  RIGHT,
  MIDDLE,
};

struct MousePosition {
  float xPosition;
  float yPosition;
};

// fill it
// like return it
// so, do I like make an EventSystem, make a ptr in the inputsys
class EventSystem {
public:
  std::vector<KeyPress> m_KeysPressed;
  std::vector<MouseButtonPress> m_MouseButtonPress;

  void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
    if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT)
      if (action == GLFW_PRESS)
        m_KeysPressed.push_back(KeyPress::SHIFT);

    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
      if (action == GLFW_PRESS)
        m_KeysPressed.push_back(KeyPress::CTRL);

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::Q);

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::W);

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::E);

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::R);

    if (key == GLFW_KEY_T && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::T);

    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::Y);

    if (key == GLFW_KEY_U && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::U);

    if (key == GLFW_KEY_I && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::I);

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::O);

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::P);

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::A);

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::S);

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::D);

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::F);

    if (key == GLFW_KEY_G && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::G);
		
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::H);

    if (key == GLFW_KEY_J && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::J);

    if (key == GLFW_KEY_K && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::K);

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::L);

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::Z);

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::X);

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::C);

    if (key == GLFW_KEY_V && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::V);

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::B);

    if (key == GLFW_KEY_N && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::N);

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::M);

    if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
      m_KeysPressed.push_back(KeyPress::HOME);
  }

  void MouseButtonCallback(GLFWwindow *window, int button, int action,
                           int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
      m_MouseButtonPress.push_back(MouseButtonPress::LEFT);
    
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) 
      m_MouseButtonPress.push_back(MouseButtonPress::RIGHT);
    
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) 
      m_MouseButtonPress.push_back(MouseButtonPress::MIDDLE);
   
  }

private:
};
