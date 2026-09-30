#include "TextEditor.hpp"

class ScriptingTab {
public:
  ScriptingTab() {
    m_Editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
    m_Editor.SetPalette(TextEditor::GetDarkPalette());
    m_Editor.SetText("// write your script here\n");
  }

  void Draw() { m_Editor.Render("Code Editor##Scripting"); }

private:
  TextEditor m_Editor;
};
