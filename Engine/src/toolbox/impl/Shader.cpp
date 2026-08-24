#include "../header/Shader.hpp"

Shader::Shader(const char *vertexShader, const char *fragmentShader) {}

void Shader::ShaderCleanup() {}

void Shader::UseShaderProgram() const { glUseProgram(m_ShaderProgram); }

unsigned int Shader::GetShaderProgram() const { return m_ShaderProgram; }
