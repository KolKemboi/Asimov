#pragma once
#include <APE_Components.hpp>
#include <cstdio>
#include <entt/entt.hpp>
#include <imgui.h>
#include <string>

class SelectionSystem {
public:
  void Selection(entt::registry &reg) {
    ImGui::Begin("Outliner");

    // grab all objects names and count
    auto view = reg.view<Name, ObjectCount>();

    // loop over the objects
    for (auto [ent, name, count] : view.each()) {
      // take the selected object, if any
      bool isSelected = reg.all_of<Selected>(ent);
      // make the name, from name and count
      std::string title = name.s_Name + "_" + std::to_string(count.s_Count);

      // make a selectable list
      if (ImGui::Selectable(title.c_str(), isSelected)) {
        // grab the selected entity
        auto selectedView = reg.view<Selected>();
        // for all selected, remove them
        for (auto [selectedEntity] : selectedView.each()) {
          reg.remove<Selected>(selectedEntity);
        }
        // put the selected into the selected
        reg.emplace<Selected>(ent);
      }
    }

    ImGui::End();
  }
};
