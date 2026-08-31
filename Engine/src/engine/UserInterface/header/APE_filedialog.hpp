
//   if (ImGui::Button("Open")) {
//     ImGuiFileDialog::Instance()->OpenDialog("ChooseFile", "Open File",
//                                             ".txt,.cpp,.h");
//   }
//
//   if (ImGuiFileDialog::Instance()->Display("ChooseFile")) {
//     if (ImGuiFileDialog::Instance()->IsOk()) {
//       std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
//
//       // Open file...
//     }
//
//     ImGuiFileDialog::Instance()->Close();
//   }
//   if (ImGui::BeginMenu("Edit")) {
//     ImGui::MenuItem("Undo", "Ctrl+Z");
//     ImGui::MenuItem("Redo", "Ctrl+Y");
//     ImGui::Separator();
//     ImGui::MenuItem("Cut", "Ctrl+X");
//     ImGui::MenuItem("Copy", "Ctrl+C");
//     ImGui::MenuItem("Paste", "Ctrl+V");
//
//     ImGui::EndMenu();
//   }
//
//   if (ImGui::BeginMenu("View")) {
//     ImGui::MenuItem("Properties");
//     ImGui::MenuItem("Console");
//     ImGui::MenuItem("Inspector");
//
//     ImGui::EndMenu();
//   }
//
//   ImGui::EndMainMenuBar();
// }
