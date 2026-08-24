#pragma once
#include <memory>
#ifndef __GLAD_GUARD__
#include <glad/glad.h>
#endif
#include <APE_shader.hpp>
#include <GLFW/glfw3.h>

/*
 * Set up windowing
 * Set up glad
 * Get a window
 *
 */
class Engine {
public:
  Engine(const char *);
  void Run();
  void Clean();

private:
  GLFWwindow *m_Window;
  const char *m_WindowName;
  unsigned int m_WindowWidth, m_WindowHeight;

  std::shared_ptr<Shader> m_DefaultShader;

private:
  void _initGlfwWindowUtils();
  void _runAPEEngine();
  void _cleanGlfwWindowUtils();

  void _miniInputSystem(GLFWwindow *);
};
