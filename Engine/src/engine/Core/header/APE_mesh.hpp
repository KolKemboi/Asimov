#pragma once
#include <vector>
#ifndef __GLAD_GAURD__
#include <glad/glad.h>
#endif
#include "APE_IBO.hpp"
#include "APE_VAO.hpp"
#include "APE_VBO.hpp"
#include <APE_assetloader.hpp>
#include <glm/glm.hpp>

class Mesh {
public:
  Mesh(std::vector<float> &, size_t, std::vector<unsigned int> &, size_t);

  ModelLoader m_ModelLoader("");

  void Clean();

  void BindVAO();
  unsigned int ReturnMeshVertexArrayObject();

private:
  VertexBuffer m_VertexBuffer;
  VertexArray m_VertexArray;
  IndexBuffer m_IndexBuffer;
};
