#pragma once
#include <APE_Components.hpp>
#include <cstdio>
#include <entt/entt.hpp>
#include <imgui.h>
#include <string>
#include <vector>

class SelectionSystem {
public:
  void Selection(entt::registry &reg) {
    ImGui::Begin("Outliner");

    auto view = reg.view<Name, ObjectCount>();

    for (auto [ent, name, count] : view.each()) {
      bool isSelected = reg.all_of<Selected>(ent);
      std::string title = name.s_Name + "_" + std::to_string(count.s_Count);

      if (ImGui::Selectable(title.c_str(), isSelected)) {
        auto selectedView = reg.view<Selected>();
        for (auto [selectedEntity] : selectedView.each()) {
          reg.remove<Selected>(selectedEntity);
        }
        reg.emplace<Selected>(ent);
      }
    }

    ImGui::End();
  }
};
