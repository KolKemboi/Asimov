#include <APE_Components.hpp>
#include <APE_properties.hpp>
#include <format>
#include <imgui.h>
#include <string>

void Properties::MakeProperties(entt::registry &reg) {

  auto view = reg.view<Name, ObjectCount, Transform>();

  ImGui::Begin("Properties");
  for (auto [ent, name, count, transform] : view.each()) {
    std::string title = name.s_Name + std::to_string(count.s_Count);
    ImGui::Text("%s", title.c_str());
    std::string tx = title + " x";
    std::string ty = title + " y";
    std::string tz = title + " z";

    // ImGui::Begin(title.c_str());
    ImGui::SliderFloat(tx.c_str(), &transform.s_Position.x, -10, 10);
    ImGui::SliderFloat(ty.c_str(), &transform.s_Position.y, -10, 10);
    ImGui::SliderFloat(tz.c_str(), &transform.s_Position.z, -10, 10);
    // ImGui::End();
    ImGui::Separator();
  }
  ImGui::End();
}
