#pragma once

#include <APE_FBO.hpp>
#include <APE_engine.hpp>
#include <APE_shader.hpp>
#include <entt/entt.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

struct Transform {
  glm::vec3 s_Position = glm::vec3(0.0f);
  glm::vec3 s_Rotation = glm::vec3(0.0f);
  glm::vec3 s_Scale = glm::vec3(1.0f);

  glm::mat4 GetModelMatrix() const {
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    return modelMatrix;
  }
};

struct Material {
  glm::vec3 s_Color;

  Material(glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f)) : s_Color(color) {};
};

struct Renderable {
	//need a VAO here
  unsigned int s_IndexCount;
  unsigned int s_StartIndex;

  Renderable(unsigned int startIdx, unsigned int idxCount)
      : s_IndexCount(idxCount), s_StartIndex(startIdx) {};
};

struct Name {
  std::string s_Name;
};
