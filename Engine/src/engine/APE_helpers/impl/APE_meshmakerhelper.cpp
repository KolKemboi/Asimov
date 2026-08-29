#include <APE_meshmakerhelper.hpp>

MeshMakerHelper::MeshMakerHelper(APEObject &objData, std::string path)
    : m_ModelLoaderHelper(path) {
  m_VertexArray.GenVertexArrays();
  m_VertexArray.BindVertexArray();
  m_VertexBuffer.GenVertexBuffers(m_ModelLoaderHelper.m_Object.s_Vertices);
};
