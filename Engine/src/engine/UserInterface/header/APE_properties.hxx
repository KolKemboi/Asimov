#pragma once

#include <APE_Components.hpp>
#include <APE_shader.hpp>
#include <entt/entt.hpp>
#include <imgui.h>
#include <memory>
#include <string>
#include <vector>

class Properties {
public:
  void MakeProperties(entt::registry &reg, std::shared_ptr<Shader> &shader,
                      glm::vec3 &lightColor, rp3d::PhysicsWorld *&world,
                      rp3d::PhysicsCommon &physicsCommon) {

    auto view = reg.view<Name, ObjectCount, Transform>();
    // glm::vec3 lightColor = glm::vec3(1.0f);

    ImGui::Begin("Properties");
    ImGui::DragFloat3("Light Color", glm::value_ptr(lightColor), 1.f, 0.0f,
                      1.0f);
    shader->SetVec3(lightColor, "lightColor");
    ImGui::Separator();

    auto selectedView = reg.view<Selected>();
    for (auto [entity] : selectedView.each()) {
      if (reg.all_of<Name>(entity)) {
        auto &name = reg.get<Name>(entity);
      }

      if (reg.all_of<Transform>(entity)) {
        auto &transform = reg.get<Transform>(entity);
        ImGui::DragFloat3("Position", glm::value_ptr(transform.s_Position),
                          0.1f);
        ImGui::DragFloat3("Rotation", glm::value_ptr(transform.s_Rotation),
                          1.0f);
        ImGui::DragFloat3("Scale", glm::value_ptr(transform.s_Scale), 1.0f);
      }

      if (reg.all_of<Material>(entity)) {
        auto &material = reg.get<Material>(entity);

        ImGui::ColorEdit3("Color", glm::value_ptr(material.s_Color));
      }
    }

    ImGui::End();
  }
  void MakePhysicsProperties(entt::registry &reg,
                             std::shared_ptr<Shader> &shader) {

    std::vector<std::string> opts = {"ACTIVE", "STATIC", "KINEMATIC"};
    static int selected = 0;
    ImGui::Begin("Physics Properties");
    if (ImGui::BeginCombo("TYPE", opts[selected].c_str())) {
      for (int i{0}; i < opts.size(); i++) {
        bool isSelected = (selected == i);
        if (ImGui::Selectable(opts[i].c_str(), isSelected)) {
          selected = i;
        }
        if (isSelected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    ImGui::End();
  }
};
