#pragma once

#include <APE_types.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

// should get something like a family tree, if root exists
// This loader is specifically for primitives
//
class ModelLoaderHelper {
public:
  APEObject m_Object;
  std::vector<APEObject> m_Objects;

  // the constructor takes a path and fills the APEobject
  ModelLoaderHelper(std::string const &);

private:
  void _loadModel(std::string const);
  void _processNode(aiNode *, const aiScene *);
  APEObject _processMesh(aiMesh *, const aiScene *);

private:
  std::string m_Dir;
};
