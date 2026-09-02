#include "APE_IBO.hpp"
#include <cstdio>
#include <vector>

void IndexBuffer::GenIndexBuffers(std::vector<unsigned int> &indices,
                                  size_t size) {
  glGenBuffers(1, &this->m_IndexBuffers);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IndexBuffers);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);
}

void IndexBuffer::Clean() {
  glDeleteBuffers(1, &this->m_IndexBuffers);
  printf("IBO::CLEANED\n");
}
