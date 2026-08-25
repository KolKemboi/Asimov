#include "APE_engine.hpp"
#include "APE_mesh.hpp"
#include "APE_shader.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

Engine::Engine(const char *windowName) : m_WindowName(windowName) {
  this->_initGlfwWindowUtils();
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

  this->m_MainFrameBuffer =
      std::make_unique<FrameBuffer>(m_WindowWidth, m_WindowHeight);
}

void Engine::Run() { this->_runAPEEngine(); }

void Engine::Clean() {

  this->m_Mesh->Clean();

  this->_cleanGlfwWindowUtils();
}

/*
 * Creates the window variables
 */
void Engine::_initGlfwWindowUtils() {
  this->m_WindowWidth = 640;
  this->m_WindowHeight = 480;

  /*
   * Start
   */
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, false);

  this->m_Window = glfwCreateWindow(this->m_WindowWidth, this->m_WindowHeight,
                                    this->m_WindowName, NULL, NULL);

  // WINDOW FAILURE ERROR CHECK
  if (this->m_Window == NULL) {
    std::cerr << "ERROR::GLFW ERROR::CANNOT MAKE WINDOW" << std::endl;
    glfwTerminate();
    std::exit(1);
  }

  glfwMakeContextCurrent(this->m_Window);

  // GLAD LOADING ERROR CHECK
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "ERROR::GLAD ERROR::FAILED TO INIT GLAD" << std::endl;
    glfwTerminate();
    std::exit(1);
  }

  glViewport(0, 0, (GLsizei)m_WindowWidth, (GLsizei)m_WindowHeight);

  this->m_DefaultShader = std::make_shared<Shader>(
      "shaders/default/vertex.glsl", "shaders/default/fragment.glsl");
}

/*
 * mini InputSystem, basic
 */
void Engine::_miniInputSystem(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void Engine::_runAPEEngine() {
  this->m_DefaultShader->UseShader();

  while (!glfwWindowShouldClose(m_Window)) {

    // this->m_MainFrameBuffer->BindFrameBuffer();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
    this->m_Mesh->BindVAO();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
    // this->m_MainFrameBuffer->UnBindFrameBuffer();

    this->_miniInputSystem(this->m_Window);

    glfwPollEvents();
    glfwSwapBuffers(this->m_Window);
  }
}

/*
 * cleaning everything here
 */
void Engine::_cleanGlfwWindowUtils() {
  this->m_DefaultShader->Clean();
  this->m_DefaultShader = nullptr;
  glfwDestroyWindow(this->m_Window);
  this->m_Window = NULL;
  glfwTerminate();
}
