#pragma once

#include "APE_Dispatcher.hpp"
#include <APE_Components.hpp>
#include <APE_shader.hpp>
#include <cstdio>
#include <entt/entt.hpp>
#include <imgui.h>
#include <memory>
#include <reactphysics3d/components/RigidBodyComponents.h>
#include <reactphysics3d/engine/PhysicsCommon.h>

class Properties {
public:
  void MakeProperties(entt::registry &reg, std::shared_ptr<Shader> &shader,
                      glm::vec3 &lightColor, Dispatcher &dispatcher) {

    auto view = reg.view<Name, ObjectCount, Transform>();

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
        if (ImGui::IsItemDeactivatedAfterEdit()) {
          dispatcher.Emitter(EventType::OBJECT_MODIFIED,
                             "Selected object has been changed");
        }
        ImGui::DragFloat3("Rotation", glm::value_ptr(transform.s_Rotation),
                          1.0f);
        if (ImGui::IsItemDeactivatedAfterEdit()) {
          dispatcher.Emitter(EventType::OBJECT_MODIFIED,
                             "Selected object has been changed");
        }
        ImGui::DragFloat3("Scale", glm::value_ptr(transform.s_Scale), 1.0f);

        if (ImGui::IsItemDeactivatedAfterEdit()) {
          dispatcher.Emitter(EventType::OBJECT_MODIFIED,
                             "Selected object has been changed");
        }
      }

      if (reg.all_of<Material>(entity)) {
        auto &material = reg.get<Material>(entity);

        ImGui::ColorEdit3("Color", glm::value_ptr(material.s_Color));
      }
    }

    ImGui::End();
  }
  void MakePhysicsProperties(entt::registry &reg, rp3d::PhysicsCommon &phyCom,
                             rp3d::PhysicsWorld *&world,
                             Dispatcher &dispatcher) {

    ImGui::Begin("Physics Properties");

    auto selectedView = reg.view<Selected>();
    for (auto entity : selectedView) {
      static int selected = 1;

      if (reg.all_of<PhysicsData>(entity)) {
        auto fzxData = reg.get<PhysicsData>(entity);
        rp3d::BodyType bodyType = fzxData.s_BodyType;

        if (bodyType == rp3d::BodyType::STATIC)
          selected = 1;
        if (bodyType == rp3d::BodyType::DYNAMIC)
          selected = 0;
        if (bodyType == rp3d::BodyType::KINEMATIC)
          selected = 2;

        std::vector<std::string> opts = {"DYNAMIC", "STATIC", "KINEMATIC"};

        if (ImGui::BeginCombo("TYPE", opts[selected].c_str())) {

          for (int i{0}; i < opts.size(); i++) {
            bool isSelected = (selected == i);
            if (ImGui::Selectable(opts[i].c_str(), isSelected)) {
              selected = i;
              printf("collider type changed to %s\n", opts[selected].c_str());
              // call the update from here
              dispatcher.Emitter(EventType::COLLIDER_TYPE_MODIFIED,
                                 opts[selected]);
            }
            if (isSelected) {
              ImGui::SetItemDefaultFocus();
            }
          }

          ImGui::EndCombo();
        }

        // auto &fzxData = reg.get<PhysicsData>(entity);
        // auto &fzxBody = reg.get<PhysicsBody>(entity);
        // if (selected == 0)
        //   fzxBody.s_Body->setType(rp3d::BodyType::DYNAMIC);
        // if (selected == 1)
        //   fzxBody.s_Body->setType(rp3d::BodyType::STATIC);
        // if (selected == 2)
        //   fzxBody.s_Body->setType(rp3d::BodyType::KINEMATIC);
      }
    }

    ImGui::End();
  }
};
