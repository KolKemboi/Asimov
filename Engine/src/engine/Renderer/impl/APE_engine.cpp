#include "APE_engine.hpp"
#include "APE_FBO.hpp"
#include "APE_mesh.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <memory>

Engine::Engine() {
  this->verts = {
      // positions          // colors           // texture coords
      0.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 0
      0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // 1
      -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // 2
      -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 3

      0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 4
      0.0f,  -1.0f, 1.0f, 1.0f, 0.0f, // 5
      1.0f,  -1.0f, 1.0f, 1.0f, 1.0f, // 6
      1.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 7
  };
  this->indices = {
      // Front face
      0, 1, 2, 2, 3, 0,

      4, 5, 6, 6, 7, 4,
  };

  /*
   * plane 1 verts starts from 0 to 19
   * plane 1 indices starts from 0 to 5
   *
   * plane 2 verts starts from 20 to 39
   * plane 2 indices starts from 6 to 11
   */

  this->m_Mesh =
      std::make_unique<Mesh>(verts, verts.size() * sizeof(float), indices,
                             indices.size() * sizeof(float));
}

void Engine::RenderBackground(){
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
}
void Engine::Render() {

  this->m_Mesh->BindVAO();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
}
