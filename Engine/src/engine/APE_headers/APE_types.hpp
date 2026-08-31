#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Vertex {
  glm::vec3 s_Position;
  glm::vec3 s_Normal;
  glm::vec2 s_TexCoords;
};

struct APEObject {
  std::string s_Name;
  std::vector<Vertex> s_Vertices;
  std::vector<unsigned int> s_Indices;
};
