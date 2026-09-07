#pragma once
#include "APE_eventsystem.hxx"
#include <APE_camera.hpp>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <entt/entt.hpp>
#include <memory>

//--- DEV NOTES
/*
 * Event system
 * in the key call back, call a function that sets keys pressed
 * theis function can log the keys pressed, like an ENUM of sorts
 * and mouse key call back also does that
 * mouse scroll call back also does that, but with a STRUCT, to store x, and y
 * Have an event system class, which, returns the enum every loop,
 * and call the key events in this place to set the enums
 *
 */

// making a singleton
class InputSystem {
private:
  InputSystem() = default;
  ~InputSystem() = default;

public:
  static InputSystem &instance() {
    static InputSystem instance;
    return instance;
  }

  InputSystem(const InputSystem &) = delete;
  InputSystem &operator=(const InputSystem &) = delete;

public:
  bool firstMove = true;
  float lastX, lastY;
  bool isMiddleMouseDown = false;
  bool isShiftDown = false;
  GLFWwindow *window;
  Camera *camera; // Use a specific camera -> take the location in memory
  EventSystem *eventSystem;

  void SetVars(Camera &cam, EventSystem &eventsys) {
    camera = &cam;
    eventSystem = &eventsys;
  }

  static void MouseCallbackFunc(GLFWwindow *window, double xPos, double yPos) {
    instance().MouseCallBack(window, xPos, yPos);
  }

  void MouseCallBack(GLFWwindow *window, double xPos, double yPos) {

    if (firstMove) {
      lastX = xPos;
      lastY = yPos;
      firstMove = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    if (isMiddleMouseDown) {
      if (isShiftDown) {
        camera->ProcessPan(xOffset, yOffset);
      }
      if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        camera->ProcessOrbit(xOffset, yOffset);
      }
    }
  }

  static void ScrollCallbackFunc(GLFWwindow *window, double xOffset,
                                 double yOffset) {
    instance().ScrollCallback(window, xOffset, yOffset);
    instance().eventSystem->ScrollCallback(window, xOffset, yOffset);
  }
  void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {

    camera->ProcessDolly(yOffset);
  }

  static void MouseButtonCallbackFunc(GLFWwindow *window, int button,
                                      int action, int mods) {
    instance().MouseButtonCallback(window, button, action, mods);
    instance().eventSystem->MouseButtonCallback(window, button, action, mods);
  }

  void MouseButtonCallback(GLFWwindow *window, int button, int action,
                           int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      if (action == GLFW_PRESS) {
        isMiddleMouseDown = true;
        firstMove = false;
      } else if (action == GLFW_RELEASE) {
        isMiddleMouseDown = false;
      }
    }
  }

  static void KeyCallbackFunc(GLFWwindow *window, int key, int scancode,
                              int action, int mods) {
    instance().KeyCallback(window, key, scancode, action, mods);
    instance().eventSystem->KeyCallback(window, key, scancode, action, mods);
  }

  void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
    if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
      if (action == GLFW_PRESS) {
        isShiftDown = true;

      } else if (action == GLFW_RELEASE) {
        isShiftDown = false;
      }
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
      if (mods & GLFW_MOD_SHIFT) {
        camera->StartResetSmooth();
      }
    }

    if (key == GLFW_KEY_HOME && action == GLFW_PRESS) {
      camera->StartResetSmooth();
    }

    // for quick exiting, on release, this will be commented out
    // if (key == GLFW_KEY_CAPS_LOCK && action == GLFW_PRESS) {
    //   glfwSetWindowShouldClose(window, true);
    // }
  }
};
