#pragma once

#include <APE_filedialog.hxx>
#include <GLFW/glfw3.h>
#include <ImGuiFileDialog.h>
#include <imgui.h>

inline void SetUpMenuBar(GLFWwindow *window) {

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New")) {
        // New file
      }
      if (ImGui::MenuItem("Open")) {
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFile", "Open File",
                                                ".txt,.cpp,.h");
      }

      if (ImGuiFileDialog::Instance()->Display("ChooseFile")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
          std::string path = ImGuiFileDialog::Instance()->GetFilePathName();

          // Open file...
        }

        ImGuiFileDialog::Instance()->Close();
      }

      if (ImGui::MenuItem("Save", "Ctrl+S")) {
        // Save file
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Exit")) {
        glfwSetWindowShouldClose(window, true);
      }

      ImGui::EndMenu();
    }
  }
  SetUpFileDialog();

  if (ImGui::BeginMenu("Edit")) {
    ImGui::MenuItem("Undo", "Ctrl+Z");
    ImGui::MenuItem("Redo", "Ctrl+Y");
    ImGui::Separator();
    ImGui::MenuItem("Cut", "Ctrl+X");
    ImGui::MenuItem("Copy", "Ctrl+C");
    ImGui::MenuItem("Paste", "Ctrl+V");

    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("View")) {
    ImGui::MenuItem("Properties");
    ImGui::MenuItem("Console");
    ImGui::MenuItem("Inspector");

    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();
}
