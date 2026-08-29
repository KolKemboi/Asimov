#pragma once

#include <cstddef>
#include <vector>
#ifndef __GLAD_GAURD__
#include <glad/glad.h>
#endif
#include "APE_VAO.hpp"
#include <GLFW/glfw3.h>
#include <APE_types.hpp>

class VertexBuffer {
public:
  VertexBuffer() {}
  void GenVertexBuffers(std::vector<Vertex> &);
  void Clean();

private:
  unsigned int m_VertexBuffer;
};
