#include "APE_shader.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vShaderFile.open(vertexShaderPath);
    fShaderFile.open(fragmentShaderPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    std::exit(1);
  }
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();
}

unsigned int Shader::GetShaderProgram() { return m_ShaderProgram; }

void Shader::_checkShaderCompilation(){}

/*
 * set the necessary values for this
 * that is mat4, vec3 and float, anything else can be cast into one of these
 */
void Shader::SetMat4(glm::mat4 matrix, const char *uniformName) {}

void Shader::SetVec3(glm::vec3 vector, const char *uniformName) {}

void Shader::SetFloat(float floatValue, const char *uniformName) {}
