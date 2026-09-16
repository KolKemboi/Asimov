#pragma once

#include <GLFW/glfw3.h>
#include <ImGuiFileDialog.h>
#include <imgui.h>

inline void SetUpMenuBar(GLFWwindow *window) {
  if (ImGui::BeginMainMenuBar()) {

    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New")) {
        // New file
      }

      if (ImGui::MenuItem("Open", "Ctrl+O")) {
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFile", "Open File",
                                                ".txt,.cpp,.h,.hpp,.json");
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

    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
        // Undo
      }

      if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
        // Redo
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Cut", "Ctrl+X")) {
        // Cut
      }

      if (ImGui::MenuItem("Copy", "Ctrl+C")) {
        // Copy
      }

      if (ImGui::MenuItem("Paste", "Ctrl+V")) {
        // Paste
      }

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

  if (ImGuiFileDialog::Instance()->Display("ChooseFile")) {
    ImGui::SetWindowSize(ImVec2(800.0f, 500.0f));
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string path = ImGuiFileDialog::Instance()->GetFilePathName();

      // Open file here
      // Example:
      // LoadFile(path);
    }

    ImGuiFileDialog::Instance()->Close();
  }
}
