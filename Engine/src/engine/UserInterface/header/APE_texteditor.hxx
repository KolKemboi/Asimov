#include "TextEditor.hpp"
#include <imgui.h>

class ScriptingTab {
public:
  ScriptingTab() {

    m_Editor.SetLanguageDefinition(PythonLanguage());
    m_Editor.SetPalette(TextEditor::GetDarkPalette());
    m_Editor.SetText("// write your script here\n");
  }

  void Draw() { m_Editor.Render("Code Editor##Scripting"); }

  static TextEditor::LanguageDefinition PythonLanguage() {
    TextEditor::LanguageDefinition lang;

    // --- Keywords ---
    static const char *const kKeywords[] = {
        "False",  "None",     "True",  "and",    "as",       "assert",
        "async",  "await",    "break", "class",  "continue", "def",
        "del",    "elif",     "else",  "except", "finally",  "for",
        "from",   "global",   "if",    "import", "in",       "is",
        "lambda", "nonlocal", "not",   "or",     "pass",     "raise",
        "return", "try",      "while", "with",   "yield"};
    for (auto &k : kKeywords)
      lang.mKeywords.insert(k);

    // --- Identifiers (builtins) ---
    static const char *const kIdentifiers[] = {
        "abs",        "all",       "any",        "bin",          "bool",
        "bytearray",  "bytes",     "callable",   "chr",          "classmethod",
        "compile",    "complex",   "delattr",    "dict",         "dir",
        "divmod",     "enumerate", "eval",       "exec",         "filter",
        "float",      "format",    "frozenset",  "getattr",      "globals",
        "hasattr",    "hash",      "help",       "hex",          "id",
        "input",      "int",       "isinstance", "issubclass",   "iter",
        "len",        "list",      "locals",     "map",          "max",
        "memoryview", "min",       "next",       "object",       "oct",
        "open",       "ord",       "pow",        "print",        "property",
        "range",      "repr",      "reversed",   "round",        "set",
        "setattr",    "slice",     "sorted",     "staticmethod", "str",
        "sum",        "super",     "tuple",      "type",         "vars",
        "zip",        "self",      "cls",        "__init__",     "__name__",
        "__main__",   "__doc__",   "__file__"};
    for (auto &id : kIdentifiers) {
      TextEditor::Identifier idnt;
      idnt.mDeclaration = "Built-in"; // shown in tooltip on hover
      lang.mIdentifiers.insert(std::make_pair(std::string(id), idnt));
    }

    // --- Types (kept small for Python; mostly for readability) ---
    static const char *const kTypes[] = {
        "int",   "float", "complex", "bool",      "str",    "bytes", "list",
        "tuple", "dict",  "set",     "frozenset", "object", "type",  "None"};
    for (auto &t : kTypes) {
      TextEditor::Identifier idnt;
      idnt.mDeclaration = "Built-in type";
      lang.mIdentifiers.insert(std::make_pair(std::string(t), idnt));
    }

    // --- Single-line comments: # ---
    lang.mSingleLineComment = "#";

    return lang;
  }

private:
  TextEditor m_Editor;
};
