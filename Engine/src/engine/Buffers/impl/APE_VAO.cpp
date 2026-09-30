#include "APE_VAO.hpp"
#include <APE_types.hpp>
#include <cstddef>
#include <cstdio>
#include <spdlog/spdlog.h>

VertexArray::VertexArray() {
#ifdef DEBUG__
  spdlog::set_level(spdlog::level::debug);
#else
  spdlog::set_level(spdlog::level::warn);
#endif
}

void VertexArray::GenVertexArrays() {
  glGenVertexArrays(1, &this->m_VertexArray);
}

void VertexArray::BindVertexArray() { glBindVertexArray(this->m_VertexArray); }

void VertexArray::AttribPointerSetUp() {
  // positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // normals
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)offsetof(Vertex, s_Normal));
  glEnableVertexAttribArray(1);

  // texture coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)offsetof(Vertex, s_TexCoords));
  glEnableVertexAttribArray(2);
}

unsigned int VertexArray::GetVAO() { return this->m_VertexArray; }

void VertexArray::Clean() {
  glDeleteVertexArrays(1, &this->m_VertexArray);
  spdlog::info("VAO::CLEANED");
}
