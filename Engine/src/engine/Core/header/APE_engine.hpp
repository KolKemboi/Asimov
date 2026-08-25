#pragma once
#include <memory>
#ifndef __GLAD_GUARD__
#include <glad/glad.h>
#endif
#include <APE_FBO.hpp>
#include <APE_mesh.hpp>
#include <APE_shader.hpp>
#include <GLFW/glfw3.h>
#include <vector>

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
	std::unique_ptr<FrameBuffer> m_MainFrameBuffer;
  std::vector<float> verts;
  std::vector<unsigned int> indices;
  std::unique_ptr<Mesh> m_Mesh; // using this because I need the context
                                // initialized first, before running anything

private:
  void _initGlfwWindowUtils();
  void _runAPEEngine();
  void _cleanGlfwWindowUtils();

  void _miniInputSystem(GLFWwindow *);
};
