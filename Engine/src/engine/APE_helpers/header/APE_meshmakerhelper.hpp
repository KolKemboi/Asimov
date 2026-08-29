#pragma once
#include <APE_IBO.hpp>
#include <APE_VAO.hpp>
#include <APE_VBO.hpp>
#include <APE_loadmodelhelper.hpp>
#include <APE_types.hpp>

class MeshMakerHelper {
public:
  MeshMakerHelper(APEObject &, std::string);
  ModelLoaderHelper m_ModelLoaderHelper;

  void Clean();

  void BindVAO();

  unsigned int ReturnMeshVertexArrayObject();

private:
  VertexArray m_VertexArray;
  VertexBuffer m_VertexBuffer;
  IndexBuffer m_IndexBuffer;
};
