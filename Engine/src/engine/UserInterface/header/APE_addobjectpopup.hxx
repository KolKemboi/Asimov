#pragma once
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>
#include <imgui.h>

inline void SetUpPopUp(GLFWwindow *window, entt::registry &reg) {
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      ImGui::OpenPopup("Add Object");

  if (ImGui::BeginPopupModal("Add Object", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {

    if (ImGui::Button("Cube")) {
      ImGui::CloseCurrentPopup();
    }

    if (ImGui::Button("Plane")) {
      ImGui::CloseCurrentPopup();
    }

    if (ImGui::Button("Sphere")) {
      ImGui::CloseCurrentPopup();
    }

    if (ImGui::Button("Cylinder")) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}
