#pragma once
#include <APE_IBO.hpp>
#include <APE_VAO.hpp>
#include <APE_VBO.hpp>
#include <APE_loadmodelhelper.hpp>
#include <APE_types.hpp>
#include <tuple>
#include <unordered_map>
#include <utility>

class MeshMakerHelper {
public:
  MeshMakerHelper(std::string);
  ModelLoaderHelper m_ModelLoaderHelper;

  void Clean();

  std::unordered_map<std::string, std::tuple<unsigned int, unsigned int>>
  ReturnObjectData();

private:
  // this works well
  std::unordered_map<std::string, std::tuple<unsigned int, unsigned int>>
      m_NameVertexArrayIndexCount;

  void _fillObjectData(APEObject &object);

  // these are useless -> actually, these are what are to be cleaned later,
  // like a dustbin of sorts, that holds the VAO, VBO and IBO for cleaning
  std::vector<VertexArray> m_VertexArray;
  std::vector<VertexBuffer> m_VertexBuffer;
  std::vector<IndexBuffer> m_IndexBuffer;
};
