#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct APEObject {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};
