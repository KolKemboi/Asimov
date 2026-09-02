#pragma once
#include <ImGuiFileDialog.h>
#include <imgui.h>

inline void SetUpFileDialog() {
  if (ImGui::Button("")) {
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFile", "Open File",
                                            ".txt,.cpp,.h");
  }

  if (ImGuiFileDialog::Instance()->Display("ChooseFile")) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string path = ImGuiFileDialog::Instance()->GetFilePathName();

      // Open file...
    }

    ImGuiFileDialog::Instance()->Close();
  }
}
