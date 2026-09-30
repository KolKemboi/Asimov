#pragma once
#include <imgui_node_editor.h>

class NodeEditor {
public:
  NodeEditor() {
    ax::NodeEditor::Config config;
    config.SettingsFile = "NodeEditor.json";
    m_Context = ax::NodeEditor::CreateEditor(&config);
  }
  ~NodeEditor() { ax::NodeEditor::DestroyEditor(m_Context); }

  void Draw() {
    namespace ed = ax::NodeEditor;
    ed::SetCurrentEditor(m_Context);

    ed::Begin("Node Editor");

    // ---- Placeholder Node 1: Start ----
    {
      ed::BeginNode(1); // stable unique ID

      ImGui::TextUnformatted("Start");

      ed::BeginPin(11, ed::PinKind::Output); // pin has its own stable ID
      ImGui::Text("Out");
      ed::EndPin();

      ed::EndNode();
    }

    // ---- Placeholder Node 2: Add ----
    {
      ed::BeginNode(2);

      ed::BeginPin(21, ed::PinKind::Input);
      ImGui::Text("A");
      ed::EndPin();

      ImGui::SameLine();

      ed::BeginPin(22, ed::PinKind::Input);
      ImGui::Text("B");
      ed::EndPin();

      ImGui::TextUnformatted("Add");

      ed::BeginPin(23, ed::PinKind::Output);
      ImGui::Text("Sum");
      ed::EndPin();

      ed::EndNode();
    }

    // ---- Placeholder Node 3: Print ----
    {
      ed::BeginNode(3);

      ed::BeginPin(31, ed::PinKind::Input);
      ImGui::Text("In");
      ed::EndPin();

      ImGui::TextUnformatted("Print");

      ed::EndNode();
    }

    // ---- Placeholder Node 4: Constant value ----
    {
      ed::BeginNode(4);

      ImGui::TextUnformatted("Constant");
      static float value = 0.0f;
      ImGui::SetNextItemWidth(80.0f);
      ImGui::DragFloat("##value", &value);

      ed::BeginPin(41, ed::PinKind::Output);
      ImGui::Text("Val");
      ed::EndPin();

      ed::EndNode();
    }

    // ---- Draw some fake links between them ----
    ed::Link(101, 11, 21); // Start.Out -> Add.A
    ed::Link(102, 41, 22); // Constant.Val -> Add.B
    ed::Link(103, 23, 31); // Add.Sum -> Print.In

    // ---- Optional: draw a hint in the background ----
    // (only if nothing is hovered — otherwise it fights the nodes)
    if (!ed::GetHoveredNode() && !ed::GetHoveredLink()) {
      ImGui::SetCursorPos(ImVec2(20, 20));
      ImGui::TextDisabled("Placeholder nodes — drag pins to connect");
    }

    ed::End();
    ed::SetCurrentEditor(nullptr);
  }

private:
  ax::NodeEditor::EditorContext *m_Context = nullptr;
};
