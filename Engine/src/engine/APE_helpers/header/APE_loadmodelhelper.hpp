#pragma once

#include <APE_types.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

class ModelLoaderHelper {
public:
  APEObject m_Object;
  std::vector<APEObject> m_Objects;

  ModelLoaderHelper(std::string const &);

private:
  void _loadModel(std::string const);
  void _processNode(aiNode *, const aiScene *);
  APEObject _processMesh(aiMesh *, const aiScene *);

private:
  std::string m_Dir;
};
