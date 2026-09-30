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

    // draw nodes here — with stable IDs

    ed::End();
    ed::SetCurrentEditor(nullptr);
  }

private:
  ax::NodeEditor::EditorContext *m_Context = nullptr;
};
