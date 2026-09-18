#pragma once
#include <APE_eventsystem.hxx>
#include <cstdio>
#include <imgui.h>

class ConfirmPopUp {
public:
  bool ConfirmDelete() {
    bool res = false;

    if (ImGui::BeginPopupModal("Delete Object", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::TextUnformatted("Are you sure you want to delete?");

      ImGui::Spacing();
      // ImGui::Separator();
      ImGui::Spacing();

      // Buttons
      const float buttonWidth = 80.0f;

      if (ImGui::Button("Yes", ImVec2(buttonWidth, 0))) {
        res = true;
        ImGui::CloseCurrentPopup();
      }

      ImGui::SameLine();

      // Push No button toward the right
      float availableWidth = ImGui::GetContentRegionAvail().x;
      float noButtonWidth = buttonWidth;

      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availableWidth -
                           noButtonWidth);

      if (ImGui::Button("No", ImVec2(buttonWidth, 0))) {
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }

    return res;
  }
};
