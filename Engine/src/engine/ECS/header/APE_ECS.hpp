#pragma once
#include <APE_Components.hpp>
#include <APE_FBO.hpp>
#include <APE_engine.hpp>
#include <APE_shader.hpp>
#include <entt/entt.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <memory>
// should return an entity
//
// class AddEntitySystem {
// public:
//   // everything default,
//   // AddEntitySystem(entt::registry &, const char*);//say it gets name name
//   void AddCube(entt::registry &);
//   void AddSphere(entt::registry &);
//
// private:
// };

class RenderSystem {
public:
  std::unique_ptr<FrameBuffer> RenderScene(std::unique_ptr<FrameBuffer>,
                                           std::unique_ptr<Engine>,
                                           entt::registry &);
};
