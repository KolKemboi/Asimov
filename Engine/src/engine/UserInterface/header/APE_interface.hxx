#pragma once
#include "APE_viewport.hxx"
#ifndef __GLAD_GUARD__
#include <glad/glad.h>
#endif

#include "APE_UI_STYLE.hpp"
#include <APE_menubar.hxx>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class Interface {
public:
  // I can remove the default constructor, and use a member func to set the
  // variable, so that I dont need to use smart ptrs
  Interface(GLFWwindow *&window) {
    this->m_ImGUIWindow = window;

    this->SetUpIMGUIContext();
  }

  void SetUpIMGUIContext() { this->_setUpIMGUIContext(); }
  void DestroyIMGUIContext() { this->_destroyIMGUIContext(); }
  void SetUpDocking() { this->_setUpDocking(); }
  void SetUpNewFrame() { this->_setUpNewFrame(); }
  void NewRenderIMGUI() { this->_newRenderIMGUI(); }

private:
  void _setUpIMGUIContext() {
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
  void _destroyIMGUIContext() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    printf("INTERFACE::DESTROYED_CONTEXT\n");
  }

  void _setUpDocking() {
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
  void _setUpNewFrame() {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    SetUpMenuBar(m_ImGUIWindow);
    // MaterialEditor();
  }
  void _newRenderIMGUI() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

private:
  GLFWwindow *m_ImGUIWindow;
};
