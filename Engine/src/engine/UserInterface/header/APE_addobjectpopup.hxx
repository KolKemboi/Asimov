#pragma once
#include <APE_AddEntitySystem.hpp>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>
#include <imgui.h>

class AddObjectPopUp {
public:
  void SetUpPrimitiveData(std::tuple<unsigned int, unsigned int> &cubeData,
                          std::tuple<unsigned int, unsigned int> &cylinderData,
                          std::tuple<unsigned int, unsigned int> &sphereData) {

    this->_CubePrimitiveData = cubeData;
    this->_CylinderPrimitiveData = cylinderData;
    this->_SpherePrimitiveData = sphereData;
  }

  inline void SetUpPopUp(GLFWwindow *window, entt::registry &reg) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ImGui::OpenPopup("Add Object");

    if (ImGui::BeginPopupModal("Add Object", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {

      if (ImGui::Button("Cube")) {

        m_AddEntitySystem.AddCubeSystem(reg, std::get<0>(_CubePrimitiveData),
                                        std::get<1>(_CubePrimitiveData));
        ImGui::CloseCurrentPopup();
      }

      // if (ImGui::Button("Plane")) {
      //   m_AddEntitySystem.AddSphereSystem(reg,
      //                                     std::get<0>(_SpherePrimitiveData),
      //                                     std::get<1>(_SpherePrimitiveData));
      //   ImGui::CloseCurrentPopup();
      // }

      if (ImGui::Button("Sphere")) {
        m_AddEntitySystem.AddSphereSystem(reg,
                                          std::get<0>(_SpherePrimitiveData),
                                          std::get<1>(_SpherePrimitiveData));
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::Button("Cylinder")) {
        m_AddEntitySystem.AddCylinderSystem(
            reg, std::get<0>(_CylinderPrimitiveData),
            std::get<1>(_CylinderPrimitiveData));
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }
  }

private:
  AddEntitySystem m_AddEntitySystem;
  std::tuple<unsigned int, unsigned int> _CubePrimitiveData;
  std::tuple<unsigned int, unsigned int> _SpherePrimitiveData;
  std::tuple<unsigned int, unsigned int> _CylinderPrimitiveData;
};
