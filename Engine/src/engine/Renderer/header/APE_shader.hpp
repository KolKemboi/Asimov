#pragma once
#include <glm/ext/vector_float3.hpp>
#ifndef __GLAD_GUARD__
#include <glad/glad.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
  Shader(const char *, const char *);

  unsigned int GetShaderProgram();

  void UseShader();

  void Clean();

  void SetMat4(const glm::mat4 &, const char *);
  void SetVec3(const glm::vec3 &, const char *);
  void SetFloat(const float &, const char *);

private:
  void _checkShaderCompilation(unsigned int, std::string);

private:
  unsigned m_ShaderProgram, m_VertexShader, m_FragmentShader;
};
