#pragma once
#include <memory>
#ifndef __GLAD_GUARD__
#include <glad/glad.h>
#endif
#include <APE_FBO.hpp>
#include <APE_mesh.hpp>
#include <APE_shader.hpp>
#include <GLFW/glfw3.h>
#include <vector>

/*
 * get a frame buffer,
 * render, return the frame buffer
 *
 */
class Engine {
public:
  Engine();
  std::unique_ptr<FrameBuffer> Render(std::unique_ptr<FrameBuffer>);

private:
  std::vector<float> verts;

  std::vector<unsigned int> indices;
  std::unique_ptr<Mesh> m_Mesh; // using this because I need the context
                                // initialized first, before running anything

private:
};
