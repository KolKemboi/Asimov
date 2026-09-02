#include "APE_VBO.hpp"
#include <cstdio>
#include <vector>

void VertexBuffer::GenVertexBuffers(std::vector<Vertex> &vertices) {
  glGenBuffers(1, &this->m_VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);
}

void VertexBuffer::Clean() {
  glDeleteBuffers(1, &this->m_VertexBuffer);
  printf("VBO::CLEANED\n");
}
