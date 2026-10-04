#pragma once

#include <APE_Dispatcher.hpp>
#include <APE_KeyEvents.hpp>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstdio>
#include <sys/types.h>

// I have made something beautiful here
// Reduced the cycles for key event registration
// Originally, it was a vector search(probable memory IO) then a compare
// now
// it is a compare only
//
// Also, vector resizing and relocation overhead has been reduced to an uint of
// 64 bytes
class EventSystem {
public:
  MouseOffset m_MouseOffset;

  // holds all possible keys and mouse position
  uint64_t Keys = 0;
  uint32_t MousePos = 0;

  void MousePosCallback(GLFWwindow *window, double xPos, double yPos) {
    // grab the positions and set the pos
    MousePos = (uint32_t)xPos | ((uint32_t)yPos << 16);
  }

  // bit set the offset per axis,
  void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    m_MouseOffset.xOffset = xOffset;

    if (xOffset == -1) {
      Keys |= (uint)Alicia::MOUSE_ON_X;
      Keys |= (uint)Alicia::MOUSE_X_SIGN;
    } else if (xOffset == 1) {
      Keys |= (uint)Alicia::MOUSE_ON_X;
      Keys &= ~(uint)Alicia::MOUSE_X_SIGN;
    } else {
      Keys &= ~(uint)Alicia::MOUSE_ON_X;
      Keys &= ~(uint)Alicia::MOUSE_X_SIGN;
    }
    m_MouseOffset.yOffset = yOffset;

    if (yOffset == -1) {
      Keys |= (uint)Alicia::MOUSE_ON_Y;
      Keys |= (uint)Alicia::MOUSE_Y_SIGN;
    } else if (yOffset == 1) {
      Keys |= (uint)Alicia::MOUSE_ON_Y;
      Keys &= ~(uint)Alicia::MOUSE_Y_SIGN;
    } else {
      Keys &= ~(uint)Alicia::MOUSE_ON_Y;
      Keys &= ~(uint)Alicia::MOUSE_Y_SIGN;
    }
  }

  // bit set all keys on the keyboard
  void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods) {

    if (mods == GLFW_MOD_SHIFT && action == GLFW_PRESS)
      Keys |= (uint)Alicia::SHIFT;

    if (mods == GLFW_MOD_ALT && action == GLFW_PRESS)
      Keys |= (uint)Alicia::ALT;

    if (mods == GLFW_MOD_CONTROL && action == GLFW_PRESS)
      Keys |= (uint)Alicia::CTRL;

    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
      Keys |= (uint)Alicia::DELETE;

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
      Keys |= (uint)Alicia::Q;

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
      Keys |= (uint)Alicia::W;

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
      Keys |= (uint)Alicia::E;

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
      Keys |= (uint)Alicia::R;

    if (key == GLFW_KEY_T && action == GLFW_PRESS)
      Keys |= (uint)Alicia::T;

    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
      Keys |= (uint)Alicia::Y;

    if (key == GLFW_KEY_U && action == GLFW_PRESS)
      Keys |= (uint)Alicia::U;

    if (key == GLFW_KEY_I && action == GLFW_PRESS)
      Keys |= (uint)Alicia::I;

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
      Keys |= (uint)Alicia::O;

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
      Keys |= (uint)Alicia::P;

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
      Keys |= (uint)Alicia::A;

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
      Keys |= (uint)Alicia::S;

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
      Keys |= (uint)Alicia::D;

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
      Keys |= (uint)Alicia::F;

    if (key == GLFW_KEY_G && action == GLFW_PRESS)
      Keys |= (uint)Alicia::G;

    if (key == GLFW_KEY_H && action == GLFW_PRESS)
      Keys |= (uint)Alicia::H;

    if (key == GLFW_KEY_J && action == GLFW_PRESS)
      Keys |= (uint)Alicia::J;

    if (key == GLFW_KEY_K && action == GLFW_PRESS)
      Keys |= (uint)Alicia::K;

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
      Keys |= (uint)Alicia::L;

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
      Keys |= (uint)Alicia::Z;

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
      Keys |= (uint)Alicia::X;

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
      Keys |= (uint)Alicia::C;

    if (key == GLFW_KEY_V && action == GLFW_PRESS)
      Keys |= (uint)Alicia::V;

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
      Keys |= (uint)Alicia::B;

    if (key == GLFW_KEY_N && action == GLFW_PRESS)
      Keys |= (uint)Alicia::N;

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
      Keys |= (uint)Alicia::M;

    if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
      Keys |= (uint)Alicia::HOME;
  }
  // bit set mouse key press events
  void MouseButtonCallback(GLFWwindow *window, int button, int action,
                           int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
      Keys |= (uint)Alicia::LEFT;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
      Keys |= (uint)Alicia::RIGHT;

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
      Keys |= (uint)Alicia::MIDDLE;
  }

private:
};
