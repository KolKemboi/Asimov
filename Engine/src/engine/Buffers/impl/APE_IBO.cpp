#include "APE_IBO.hpp"
#include <cstdio>
#include <spdlog/spdlog.h>
#include <vector>

// Bro, IDK what to document here man,
// just know this works
// dont touch it
void IndexBuffer::GenIndexBuffers(std::vector<unsigned int> &indices,
                                  size_t size) {
#ifdef DEBUG__
  spdlog::set_level(spdlog::level::debug);
#else
  spdlog::set_level(spdlog::level::warn);
#endif

  glGenBuffers(1, &this->m_IndexBuffers);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IndexBuffers);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);
}

void IndexBuffer::Clean() {
  glDeleteBuffers(1, &this->m_IndexBuffers);
  spdlog::info("IBO::CLEANED");
}
