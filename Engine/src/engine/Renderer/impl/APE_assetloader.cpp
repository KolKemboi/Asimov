#include <APE_assetloader.hpp>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cstdio>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <vector>

ModelLoader::ModelLoader(std::string const &path) { this->_loadModel(path); }

void ModelLoader::_loadModel(std::string const path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    printf("ERROR::ASSIMP::%s", importer.GetErrorString());
    return;
  }

  m_Directory = path.substr(0, path.find_last_of('/'));
  this->_processNode(scene->mRootNode, scene);
}

void ModelLoader::_processNode(aiNode *node, const aiScene *scene) {

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    m_APEObjects.push_back(this->_processModel(mesh, scene));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    this->_processNode(node->mChildren[i], scene);
  }
}

// I am guaranteeing that it will only load a single object
// This is to test out the APEobject
APEObject ModelLoader::_processModel(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector;

    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].x;

    vertex.Position = vector;

    /// load normals else, fill with zeros
    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].x;

      vertex.Normal = vector;
    } else {
      vertex.Normal = glm::vec3(0.0f);
    }
    /// load tex coords else, fill with zeros
    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.TexCoords = vec;
    } else {
      vertex.TexCoords = glm::vec2(0.0f);
    }
    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  m_APEObject.vertices = vertices;
  m_APEObject.indices = indices;

  return m_APEObject;
}
