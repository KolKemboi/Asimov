#include "APE_shader.hpp"
#include "APE_engine.hpp"
#include "APE_mesh.hpp"
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

  this->m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(m_VertexShader, 1, &vShaderCode, NULL);
  glCompileShader(m_VertexShader);
  _checkShaderCompilation(m_VertexShader, "VERTEX");

  this->m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(m_FragmentShader, 1, &fShaderCode, NULL);
  glCompileShader(m_FragmentShader);
  _checkShaderCompilation(m_FragmentShader, "FRAGMENT");

  this->m_ShaderProgram = glCreateProgram();
  glAttachShader(m_ShaderProgram, m_VertexShader);
  glAttachShader(m_ShaderProgram, m_FragmentShader);
  glLinkProgram(m_ShaderProgram);
  _checkShaderCompilation(m_ShaderProgram, "PROGRAM");

  glDeleteShader(m_VertexShader);
  glDeleteShader(m_FragmentShader);
}

unsigned int Shader::GetShaderProgram() { return m_ShaderProgram; }

void Shader::_checkShaderCompilation(unsigned int shader, std::string type) {
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      printf("ERROR::SHADER COMPILATION ERROR::%s::%s\n", type.c_str(),
             infoLog);
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      printf("ERROR::PROGRAM LINKING ERROR::%s::%s\n", type.c_str(), infoLog);
    }
  }
}

void Shader::UseShader() { glUseProgram(m_ShaderProgram); }

void Shader::Clean() {
  glDeleteShader(m_ShaderProgram);
  printf("SHADER::CLEANED\n");
}

/*
 * set the necessary values for this
 * that is mat4, vec3 and float, anything else can be cast into one of these
 */
void Shader::SetMat4(const glm::mat4 &matrix, const char *uniformName) {
  glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, uniformName), 1,
                     GL_FALSE, &matrix[0][0]);
}

void Shader::SetVec3(const glm::vec3 &vector, const char *uniformName) {
  glUniform3fv(glGetUniformLocation(m_ShaderProgram, uniformName), 1,
               &vector[0]);
}

void Shader::SetFloat(const float &floatValue, const char *uniformName) {
  glUniform1f(glGetUniformLocation(m_ShaderProgram, uniformName), floatValue);
}
