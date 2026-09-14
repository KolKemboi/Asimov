#pragma once
#include <memory>
#ifndef __GLAD_GUARD__
#include <glad/glad.h>
#endif
#include <APE_FBO.hpp>
#include <APE_shader.hpp>
#include <entt/entt.hpp>

class RenderSystem {
public:
  // take fbo, render to it
  void RenderEntities(std::unique_ptr<FrameBuffer> &, entt::registry &,
                      std::shared_ptr<Shader> &);
};
