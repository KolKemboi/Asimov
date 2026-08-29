#include "APE_VAO.hpp"
#include <APE_types.hpp>
#include <cstddef>
#include <cstdio>

VertexArray::VertexArray() {}

void VertexArray::GenVertexArrays() {
  glGenVertexArrays(1, &this->m_VertexArray);
}

void VertexArray::BindVertexArray() { glBindVertexArray(this->m_VertexArray); }

void VertexArray::AttribPointerSetUp() {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)offsetof(Vertex, s_Normal));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)offsetof(Vertex, s_TexCoords));
  glEnableVertexAttribArray(2);
}

unsigned int VertexArray::GetVAO() { return this->m_VertexArray; }

void VertexArray::Clean() {
  printf("VAO::CLEANED\n");

  glDeleteVertexArrays(1, &this->m_VertexArray);
}
