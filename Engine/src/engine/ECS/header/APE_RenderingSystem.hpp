#pragma once
#include <memory>
#ifndef __GLAD_GUARD__
#include <glad/glad.h>
#endif
#include <APE_FBO.hpp>
#include <entt/entt.hpp>
#include <APE_shader.hpp>

class RenderSystem {
public:
  void RenderEntities(std::unique_ptr<FrameBuffer> &, entt::registry &, std::shared_ptr<Shader>&);
};
