#pragma once
#include "APE_Dispatcher.hpp"
#include "APE_KeyEvents.hpp"
#include <APE_AddCollider.hxx>
#include <APE_AddEntitySystem.hpp>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <entt/entt.hpp>
#include <imgui.h>
#include <memory>

class AddObjectPopUp {
public:
  // smart ptr so that I manage when the object is created
  // will be destroyed when this goes off scope -> RAII
  void SetDispatcher(Dispatcher &dispatcher) {
    m_AddEntitySystem = std::make_unique<AddEntitySystem>(dispatcher);
    m_LocalDispatcher = &dispatcher; //  set up a ptr to the main dispatcher
  };

  // DONT TOUCH THIS
  void SetUpPrimitiveData(std::tuple<unsigned int, unsigned int> &cubeData,
                          std::tuple<unsigned int, unsigned int> &cylinderData,
                          std::tuple<unsigned int, unsigned int> &sphereData,
                          std::tuple<unsigned int, unsigned int> &capsuleData,
                          std::tuple<unsigned int, unsigned int> &convexMeshData

  ) {

    this->_CubePrimitiveData = cubeData;
    this->_CylinderPrimitiveData = cylinderData;
    this->_SpherePrimitiveData = sphereData;
    this->_CapsulePrimitiveData = capsuleData;
    this->_ConvexMeshPrimitiveData = convexMeshData;
  }

  inline void SetUpPopUp(GLFWwindow *window, entt::registry &reg,
                         rp3d::PhysicsWorld *&world,
                         rp3d::PhysicsCommon &phyCom) {
    // dispatcher.Subscibe("KEY_PRESS", "SHIFT_A")
    // m_LocalDispatcher->Subscriber(KEY_PRESS, );
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ImGui::OpenPopup("Add Object");

    // SHIFT A pop up window for primitives addition
    if (ImGui::BeginPopupModal("Add Object", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {

      if (ImGui::Button("Cube")) {

        m_AddEntitySystem->AddCubeSystem(reg, std::get<0>(_CubePrimitiveData),
                                         std::get<1>(_CubePrimitiveData));
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::Button("Sphere")) {
        m_AddEntitySystem->AddSphereSystem(reg,
                                           std::get<0>(_SpherePrimitiveData),
                                           std::get<1>(_SpherePrimitiveData));

        ImGui::CloseCurrentPopup();
      }

      if (ImGui::Button("Cylinder")) {
        m_AddEntitySystem->AddCylinderSystem(
            reg, std::get<0>(_CylinderPrimitiveData),
            std::get<1>(_CylinderPrimitiveData));
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::Button("Capsule")) {
        m_AddEntitySystem->AddCapsuleSystem(reg,
                                            std::get<0>(_CapsulePrimitiveData),
                                            std::get<1>(_CapsulePrimitiveData));
        ImGui::CloseCurrentPopup();
      }
      if (ImGui::Button("Convex Mesh")) {
        m_AddEntitySystem->AddConvexMeshSystem(
            reg, std::get<0>(_ConvexMeshPrimitiveData),
            std::get<1>(_ConvexMeshPrimitiveData));
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }
  }

private:
  std::unique_ptr<AddEntitySystem> m_AddEntitySystem;
  Dispatcher *m_LocalDispatcher;

  AddColliderSystem m_AddColliderSystem;
  std::tuple<unsigned int, unsigned int> _CubePrimitiveData;
  std::tuple<unsigned int, unsigned int> _SpherePrimitiveData;
  std::tuple<unsigned int, unsigned int> _CylinderPrimitiveData;
  std::tuple<unsigned int, unsigned int> _CapsulePrimitiveData;
  std::tuple<unsigned int, unsigned int> _ConvexMeshPrimitiveData;
};
