#pragma once

#include <cstddef>
#include <vector>
#ifndef __GLAD_GAURD__
#include <glad/glad.h>
#endif
#include "APE_VAO.hpp"
#include <GLFW/glfw3.h>

class IndexBuffer {
public:
  void GenIndexBuffers(std::vector<unsigned int> &, size_t);
  void Clean();

private:
  unsigned int m_IndexBuffers;
};
