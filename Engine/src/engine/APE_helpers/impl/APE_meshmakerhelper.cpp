#include "APE_IBO.hpp"
#include "APE_VAO.hpp"
#include "APE_types.hpp"
#include <APE_meshmakerhelper.hpp>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

MeshMakerHelper::MeshMakerHelper(std::string path) : m_ModelLoaderHelper(path) {
  // check if the model has more than one object
  // m_ModelLoader.size() = 1
  // return a VAO and and Index count
  // m_ModelLoader.size() > 1
  // return a std_unordered map of std::pair(VAO, IndexCount)
  // call two different functions that have different return types,
  // automatically
  // Primitives will automatically have 1 object,
  // assume imported files have mutiple VAOs
  // or, just return a std::pair with a str_name
  // m_VertexArray.GenVertexArrays();
  // m_VertexArray.BindVertexArray();
  // m_VertexBuffer.GenVertexBuffers(m_ModelLoaderHelper.m_Object.s_Vertices);

  for (APEObject &object : m_ModelLoaderHelper.m_Objects) {
    this->_fillObjectData(object);
  }
};

void MeshMakerHelper::_fillObjectData(APEObject &object) {
  // this should take an obj ref, grab the name
  // grab verts and make a VAO
  // Count the idxs
  // add to the m_NameVertexArrayObject
  std::string name = object.s_Name;
  VertexArray vertArray;
  VertexBuffer vertBuffer;
  IndexBuffer idxBuffer;

  vertArray.GenVertexArrays();
  vertArray.BindVertexArray();
  vertBuffer.GenVertexBuffers(object.s_Vertices);
  idxBuffer.GenIndexBuffers(object.s_Indices, object.s_Indices.size());
  vertArray.AttribPointerSetUp(); // I need to stop forgetting to call you

  // should return something like
  // but primitives only return one -> I can Assert that
  // {
  // 		{"cube" : {1}, {36}},
  // 		{"cube" : {1}, {36}}
  // }
  // for non primitives, something different is to be used
  m_NameVertexArrayIndexCount[name] =
      std::make_tuple(vertArray.GetVAO(), object.s_Indices.size());

  // what is the use of these
  m_VertexArray.push_back(vertArray);
  m_VertexBuffer.push_back(vertBuffer);
  m_IndexBuffer.push_back(idxBuffer);
}

std::unordered_map<std::string, std::tuple<unsigned int, unsigned int>>
MeshMakerHelper::ReturnObjectData() {
  return m_NameVertexArrayIndexCount;
};

void MeshMakerHelper::Clean() {
  for (VertexArray &arr : m_VertexArray)
    arr.Clean();

  m_VertexArray.clear();

  for (VertexBuffer &arr : m_VertexBuffer)
    arr.Clean();

  m_VertexBuffer.clear();

  for (IndexBuffer &arr : m_IndexBuffer)
    arr.Clean();

  m_IndexBuffer.clear();
}
