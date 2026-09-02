#pragma once

#include <APE_FBO.hpp>
#include <APE_engine.hpp>
#include <APE_shader.hpp>
#include <entt/entt.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

struct Transform {
  glm::vec3 s_Position = glm::vec3(0.0f);
  glm::vec3 s_Rotation = glm::vec3(0.0f);
  glm::vec3 s_Scale = glm::vec3(1.0f);

  glm::mat4 GetModelMatrix() const {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, s_Position);
    modelMatrix = glm::scale(modelMatrix, s_Scale);

    return modelMatrix;
  }
};

enum class Type {
  LIGHT,
  MESH,
};
struct Material {
  glm::vec3 s_Color;
  Type s_Type; //

  Material(glm::vec3 color, Type type) : s_Color(color), s_Type(type) {};
};

struct Renderable {
  // need a VAO here
  unsigned int s_IndexCount;
  unsigned int s_VAO;

  Renderable(unsigned int VAO, unsigned int idxCount)
      : s_IndexCount(idxCount), s_VAO(VAO) {};
};

// name starts as name, next will be name_001 => name + count
struct Name {
  std::string s_Name;
};

// so that, add primitive increases this
// duplicate increases this
// importing will increase this automatically
struct ObjectCount {
  unsigned int s_Count = 1;
};
