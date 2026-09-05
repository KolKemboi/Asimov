#pragma once
#include "APE_viewport.hpp"
#ifndef __GLAD_GUARD__
#include <glad/glad.h>
#endif

#include "APE_UI_STYLE.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class Interface {
public:
  // I can remove the default constructor, and use a member func to set the
  // variable, so that I dont need to use smart ptrs
  Interface(GLFWwindow *&);

  void SetUpIMGUIContext();
  void DestroyIMGUIContext();
  void SetUpDocking();
  void SetUpNewFrame();
  void NewRenderIMGUI();

private:
  void _setUpIMGUIContext();
  void _destroyIMGUIContext();
  void _setUpDocking();
  void _setUpNewFrame();
  void _newRenderIMGUI();

private:
  GLFWwindow *m_ImGUIWindow;
};
