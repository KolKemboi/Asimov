#pragma once
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
	GLFWwindow* m_ImGUIWindow;
};
