#pragma once

#include <APE_types.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
// #include <optional>
#include <string>
#include <vector>

// struct Vertex {
//   glm::vec3 Position;
//   glm::vec3 Normal;
//   glm::vec2 TexCoords;
// };
//
// struct APEObject {
// 	 std::string name;
//   std::vector<Vertex> vertices;
//   std::vector<unsigned int> indices;
// };
// process APE objects
// unwrap the verts, inds into one std::vector
// name: idx_start, idx_count

class ModelLoader {
public:
  APEObject m_APEObject;
  std::vector<APEObject> m_APEObjects;

  ModelLoader(std::string const &);

private:
  void _loadModel(std::string const);
  void _processNode(aiNode *, const aiScene *);
  APEObject _processModel(aiMesh *,
                          const aiScene *); // could be a std::optional<>

private:
  std::string m_Directory;
};
