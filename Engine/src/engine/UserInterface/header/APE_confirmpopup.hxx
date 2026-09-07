#pragma once
#include <APE_eventsystem.hxx>
#include <cstdio>
#include <imgui.h>

class ConfirmPopUp {
public:
  bool ConfirmDelete(bool del) {

    bool res = false;

		if(del)
    { ImGui::OpenPopup("Delete Object"); }

    if (ImGui::BeginPopupModal("Delete Object", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {

      if (ImGui::Button("Yes")) {
        res = true;
        ImGui::CloseCurrentPopup();
      }

      ImGui::SameLine();

      if (ImGui::Button("No")) {
        res = false;
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }
    return res;
  }
};
