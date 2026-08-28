#pragma once
#include <APE_FBO.hpp>
#include <APE_engine.hpp>
#include <APE_shader.hpp>
#include <entt/entt.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <memory>

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
  // std::shared_ptr<Shader> s_Shader;
  glm::vec3 s_Color;

  Material(glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f)) : s_Color(color) {};
};

struct Renderable {
  // unsigned int s_IndexCount;
  // Material s_Material;
};

struct Name {
  std::string s_Name;
};

// should return an entity
class Registry {
public:
  // I need to fill the missing boxes,
  // I need to make a mesh
  // make a material for it -> Default Material??
  // make its transform -> not really used now
  // add its physics
  // Give it a name
  void MakeEntity(entt::registry &);
  void RemoveEntity(entt::registry &);
};

class AddEntitySystem {
public:
  // everything default,
  // AddEntitySystem(entt::registry &, const char*);//say it gets name name
  void AddCube(entt::registry &);
  void AddSphere(entt::registry &);

private:
};

class RenderSystem {
public:
  std::unique_ptr<FrameBuffer> RenderScene(std::unique_ptr<FrameBuffer>,
                                           std::unique_ptr<Engine>,
                                           entt::registry &);
};
