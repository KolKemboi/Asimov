#include "APE_types.hpp"
#include <APE_loadmodelhelper.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <cstdio>
#include <string>
#include <vector>

ModelLoaderHelper::ModelLoaderHelper(std::string const &path) {
  this->_loadModel(path);
}

void ModelLoaderHelper::_loadModel(std::string path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    printf("ERROR::ASSIMP::%s", importer.GetErrorString());
    return;
  }

  m_Dir = path.substr(0, path.find_last_of('/'));
  this->_processNode(scene->mRootNode, scene);
}

void ModelLoaderHelper::_processNode(aiNode *node, const aiScene *scene) {
  // there could be 0 root node, but multiple children nodes

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    m_Objects.push_back(this->_processMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    this->_processNode(node->mChildren[i], scene);
  }
}
APEObject ModelLoaderHelper::_processMesh(aiMesh *mesh, const aiScene *scene) {
  std::string meshName;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector;

    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.s_Position = vector;

    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.s_Normal = vector;

    } else {
      vertex.s_Normal = glm::vec3(0.0f);
    }
    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.s_TexCoords = vec;
    } else {

      vertex.s_TexCoords = glm::vec2(0.0f);
    }
    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  m_Object.s_Vertices = vertices;
  m_Object.s_Indices = indices;
  m_Object.s_Name = (std::string)mesh->mName.C_Str();

  return m_Object;
}
