#include "APE_mesh.hpp"
#include <cstdio>
#include <iostream>
#include <vector>

/*
 * when the mesh is called, these are created immediately
 */



Mesh::Mesh(std::vector<float> &vertices, size_t vertCount,
           std::vector<unsigned int> &indices, size_t indexCount)
    : m_ModelLoader("models/primitives/cube.obj") {

  // m_VertexArray.GenVertexArrays();
  // m_VertexArray.BindVertexArray();
  // m_VertexBuffer.GenVertexBuffers(vertices, vertCount);
  // m_IndexBuffer.GenIndexBuffers(indices, indexCount);
  // m_VertexArray.AttribPointerSetUp(); // dont forget to call this

	// Debugging, to see if the APEObject works
  // for (auto &apeObject : m_ModelLoader.m_APEObjects) {
  //   for (auto vertex : apeObject.vertices) {
  //     std::cout << vertex.TexCoords.x << "  " << vertex.TexCoords.y
  //               << std::endl;
  //
  //     std::cout << vertex.Position.x << "  " << vertex.Position.y << "  "
  //               << vertex.Position.z << std::endl;
  //
  //     std::cout << vertex.Normal.x << "  " << vertex.Normal.y << "  "
  //               << vertex.Normal.z << std::endl;
  //   }
  // }
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
