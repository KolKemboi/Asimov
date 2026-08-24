#include "APE_engine.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

Engine::Engine(const char *windowName) : m_WindowName(windowName) {
  this->_initGlfwWindowUtils();
}

void Engine::Run() { this->_runAPEEngine(); }

void Engine::Clean() { this->_cleanGlfwWindowUtils(); }

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
  while (!glfwWindowShouldClose(m_Window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.1f, 0.3f, 1.0f);

    this->_miniInputSystem(this->m_Window);

    glfwSwapBuffers(this->m_Window);
    glfwPollEvents();
  }
}

/*
 * cleaning everything here
 */
void Engine::_cleanGlfwWindowUtils() {
  glfwDestroyWindow(this->m_Window);
  this->m_Window = NULL;
  glfwTerminate();
}
