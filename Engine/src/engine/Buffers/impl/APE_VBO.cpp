#include "APE_VBO.hpp"
#include <cstdio>
#include <spdlog/spdlog.h>
#include <vector>

// bro, it works, okay
void VertexBuffer::GenVertexBuffers(std::vector<Vertex> &vertices) {

#ifdef DEBUG__
  spdlog::set_level(spdlog::level::debug);
#else
  spdlog::set_level(spdlog::level::warn);
#endif
  glGenBuffers(1, &this->m_VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);
}

void VertexBuffer::Clean() {
  glDeleteBuffers(1, &this->m_VertexBuffer);
  spdlog::info("VBO::CLEANED");
}
