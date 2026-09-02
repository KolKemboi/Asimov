#include "APE_UI_STYLE.hpp"
#ifndef __
#include <APE_interface.hpp>
#endif

#include <APE_addobjectpopup.hxx>
#include <APE_menubar.hxx>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void Interface::SetUpIMGUIContext() { this->_setUpIMGUIContext(); }
void Interface::DestroyIMGUIContext() { this->_destroyIMGUIContext(); }
void Interface::SetUpDocking() { this->_setUpDocking(); }
void Interface::SetUpNewFrame() { this->_setUpNewFrame(); }
void Interface::NewRenderIMGUI() { this->_newRenderIMGUI(); }

void Interface::SetUpProperties(entt::registry &reg) {
  m_Properties.MakeProperties(reg);
  SetUpPopUp(m_ImGUIWindow, reg);
}

Interface::Interface(GLFWwindow *&window) {
  this->m_ImGUIWindow = window;

  this->SetUpIMGUIContext();
}

void Interface::_setUpIMGUIContext() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &m_IO = ImGui::GetIO();
  (void)m_IO;
  m_IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  m_IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  SetupImGuiStyle();
  ImGui_ImplGlfw_InitForOpenGL(m_ImGUIWindow, true);
  ImGui_ImplOpenGL3_Init("#version 460");
}
void Interface::_destroyIMGUIContext() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  printf("INTERFACE::DESTROYED_CONTEXT\n");
}

void Interface::_setUpDocking() {
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

  const ImGuiViewport *viewport = ImGui::GetMainViewport();

  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);

  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  ImGui::Begin("DockSpace", nullptr, window_flags);

  ImGui::PopStyleVar(2);

  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

  ImGui::End();
}
void Interface::_setUpNewFrame() {
  ImGui_ImplGlfw_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();
  SetUpMenuBar(m_ImGUIWindow);
}
void Interface::_newRenderIMGUI() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
