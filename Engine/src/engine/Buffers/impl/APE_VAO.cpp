#include "APE_VAO.hpp"
#include <cstdio>

VertexArray::VertexArray() {}

void VertexArray::GenVertexArrays() {
  glGenVertexArrays(1, &this->m_VertexArray);
}

void VertexArray::BindVertexArray() { glBindVertexArray(this->m_VertexArray); }

void VertexArray::AttribPointerSetUp() {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

unsigned int VertexArray::GetVAO() { return this->m_VertexArray; }

void VertexArray::Clean() {
  printf("VAO::CLEANED\n");

  glDeleteVertexArrays(1, &this->m_VertexArray);
}
