#pragma once
#ifndef __GLAD_GUARD__
#include <glad/glad.h>
#endif

#include "APE_UI_STYLE.hpp"
#include <APE_menubar.hxx>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

class Interface {
public:
  // I can remove the default constructor, and use a member func to set the
  // variable, so that I dont need to use smart ptrs
  Interface(GLFWwindow *&window) {
    this->m_ImGUIWindow = window;

#ifdef DEBUG__
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::warn);
#endif

    this->SetUpIMGUIContext();
  }
  using TabDrawFn = std::function<void(ImGuiID dock_id)>;

  void SetLayoutTab(TabDrawFn fn) { m_LayoutTab = std::move(fn); }
  void SetScriptingTab(TabDrawFn fn) { m_ScriptingTab = std::move(fn); }
  void SetNodeEditorTab(TabDrawFn fn) { m_NodeEditorTab = std::move(fn); }

  void SetUpIMGUIContext() { this->_setUpIMGUIContext(); }
  void DestroyIMGUIContext() { this->_destroyIMGUIContext(); }
  void SetUpDocking() { this->_setUpDocking(); }
  void SetUpNewFrame() { this->_setUpNewFrame(); }
  void NewRenderIMGUI() { this->_newRenderIMGUI(); }

private:
  void _setUpIMGUIContext() {
    // set up imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &m_IO = ImGui::GetIO();
    (void)m_IO;
    m_IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    m_IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    SetupImGuiStyle(); // the photoshop style, it is called here
    ImGui_ImplGlfw_InitForOpenGL(
        m_ImGUIWindow,
        true); // passes GLFW events to imgui, never knew that
    ImGui_ImplOpenGL3_Init("#version 460");
    // FIRA CODE
    // std::string firacode_path = "fonts/Fira_Code/static/FiraCode-Bold.ttf";
    // ImFont *Firacode =
    //     m_IO.Fonts->AddFontFromFileTTF(firacode_path.c_str(), 18.0f);
    // ImFontConfig cfg;
    // cfg.SizePixels = 16.0f;
    // m_IO.Fonts->AddFontDefault(&cfg);
  }
  void _destroyIMGUIContext() {
    // clean the imgui context
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    spdlog::info("INTERFACE::DESTROYED_CONTEXT");
  }

  void _setUpDocking() {
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    if (ImGui::BeginTabBar("TopLevelTabs")) {

      if (ImGui::BeginTabItem("Layout")) {
        ImGuiID dock_id = ImGui::GetID("DockSpace_Layout");
        ImGui::DockSpace(dock_id, ImVec2(0, 0), ImGuiDockNodeFlags_None);
        _buildLayoutDockspace(dock_id);
        if (m_LayoutTab)
          m_LayoutTab(dock_id);
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("Scripting")) {
        ImGuiID dock_id = ImGui::GetID("DockSpace_Scripting");
        ImGui::DockSpace(dock_id, ImVec2(0, 0), ImGuiDockNodeFlags_None);
        _buildScriptingDockspace(dock_id);
        if (m_ScriptingTab)
          m_ScriptingTab(dock_id);
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("Node Editor")) {
        ImGuiID dock_id = ImGui::GetID("DockSpace_NodeEditor");
        ImGui::DockSpace(dock_id, ImVec2(0, 0), ImGuiDockNodeFlags_None);
        _buildNodeEditorDockspace(dock_id);
        if (m_NodeEditorTab)
          m_NodeEditorTab(dock_id);
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }

    ImGui::End();
  }

  void _buildLayoutDockspace(ImGuiID dockspace_id) {

    if (ImGui::DockBuilderGetNode(dockspace_id) != nullptr)
      return;

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id,
                                  ImGui::GetMainViewport()->WorkSize);

    ImGuiID center = dockspace_id;
    ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.25f,
                                                nullptr, &center);
    ImGuiID bottom = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.25f,
                                                 nullptr, &center);

    ImGui::DockBuilderDockWindow("Viewport##Layout", center);
    ImGui::DockBuilderDockWindow("Properties##Layout", right);
    ImGui::DockBuilderDockWindow("Outliner##Layout", right);
    ImGui::DockBuilderDockWindow("Console##Layout", bottom);

    ImGui::DockBuilderFinish(dockspace_id);
  }

  void _buildScriptingDockspace(ImGuiID dockspace_id) {

    if (ImGui::DockBuilderGetNode(dockspace_id) != nullptr)
      return;

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id,
                                  ImGui::GetMainViewport()->WorkSize);

    ImGuiID center = dockspace_id;
    ImGuiID bottom = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.30f,
                                                 nullptr, &center);

    ImGui::DockBuilderDockWindow("Code Editor##Scripting", center);
    ImGui::DockBuilderDockWindow("Console##Scripting", bottom);

    ImGui::DockBuilderFinish(dockspace_id);
  }

  void _buildNodeEditorDockspace(ImGuiID dockspace_id) {

    if (ImGui::DockBuilderGetNode(dockspace_id) != nullptr)
      return;

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id,
                                  ImGui::GetMainViewport()->WorkSize);

    ImGuiID center = dockspace_id;
    ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.30f,
                                                nullptr, &center);

    ImGui::DockBuilderDockWindow("Node Graph##NodeEditor", center);
    ImGui::DockBuilderDockWindow("Node Inspector##NodeEditor", right);

    ImGui::DockBuilderFinish(dockspace_id);
  }

  void _setUpNewFrame() {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    // SetUpMenuBar(m_ImGUIWindow); // menu bar
    // MaterialEditor();
  }
  void _newRenderIMGUI() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

private:
  GLFWwindow *m_ImGUIWindow;
  TabDrawFn m_LayoutTab;
  TabDrawFn m_ScriptingTab;
  TabDrawFn m_NodeEditorTab;
};
