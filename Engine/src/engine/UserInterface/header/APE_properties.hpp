#pragma once

#include <entt/entt.hpp>
#include <imgui.h>

class Properties {
public:
  void MakeProperties(entt::registry &);
};

// ImGui::Text("Basic Widgets");
// ImGui::Separator();
//
// ImGui::Button("Button");
// ImGui::SameLine();
// ImGui::SmallButton("Small");
//
// static bool check = false;
// ImGui::Checkbox("Enable feature", &check);
//
// static int radio = 0;
// ImGui::RadioButton("Option A", &radio, 0);
// ImGui::RadioButton("Option B", &radio, 1);
//
// static float value = 50.0f;
// ImGui::SliderFloat("Slider", &value, 0.0f, 100.0f);
// ImGui::DragFloat("Drag", &value, 0.5f, 0.0f, 100.0f);
//
// static char text[256] = {};
// ImGui::InputText("Name", text, sizeof(text));
//
// static int combo = 0;
// const char *options[] = {"First", "Second", "Third"};
// ImGui::Combo("Options", &combo, options, IM_ARRAYSIZE(options));
//
// static float color[4] = {1, 0, 0, 1};
// ImGui::ColorEdit4("Color", color);
//
// ImGui::ProgressBar(0.7f, ImVec2(250, 0));
//
// if (ImGui::CollapsingHeader("More")) {
//   ImGui::Text("Extra content");
//   ImGui::BulletText("Item one");
//   ImGui::BulletText("Item two");
//   ImGui::BulletText("Item three");
// }
