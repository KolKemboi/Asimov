#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

// A vertex has these three, pos, norm and tex,
// the rest are thrown away, binormals and tangents are thrown away
struct Vertex {
  glm::vec3 s_Position;
  glm::vec3 s_Normal;
  glm::vec2 s_TexCoords;
};

// an ape object has a name, verts and indices,
// name is specifically for selection system
struct APEObject {
  std::string s_Name;
  std::vector<Vertex> s_Vertices;
  std::vector<unsigned int> s_Indices;
};
