#include "APE_mesh.hpp"
#include <cstdio>
#include <vector>

/*
 * when the mesh is called, these are created immediately
 */
Mesh::Mesh(std::vector<float> &vertices, size_t vertCount,
           std::vector<unsigned int> &indices, size_t indexCount) {

  m_VertexArray.GenVertexArrays();
  m_VertexArray.BindVertexArray();
  m_VertexBuffer.GenVertexBuffers(vertices, vertCount);
  m_IndexBuffer.GenIndexBuffers(indices, indexCount);
  m_VertexArray.AttribPointerSetUp();//dont forget to call this
}

unsigned int Mesh::ReturnMeshVertexArrayObject() {
  return m_VertexArray.GetVAO();
}

void Mesh::BindVAO() { m_VertexArray.BindVertexArray(); }

void Mesh::Clean() {
  m_VertexArray.Clean();
  m_VertexBuffer.Clean();
  m_IndexBuffer.Clean();
  printf("MESH::CLEANED\n");
}
