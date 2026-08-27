#pragma once
#include <vector>
#ifndef __GLAD_GAURD__
#include <glad/glad.h>
#endif
#include <APE_FBO.hpp>
#include <APE_engine.hpp>
#include <APE_interface.hpp>
#include <APE_shader.hpp>
#include <GLFW/glfw3.h>
#include <memory>
#include <optional>

/*
 * APE window is resposible for setting up the openGL context,
 * Provides a framebuffer, to which the renderer will render to
 * this calls the ECS to set up things such as
 * 	- Model Loading
 * 	- Physics Systems
 * 	- User Interface Systems
 *
 */
class APE_Window {
public:
  APE_Window(unsigned int, unsigned int, const char *);
  void RunEngine();
  void CleanUp();

private:
  unsigned int m_WindowWidth, m_WindowHeight;
  const char *m_WindowName;
  std::unique_ptr<FrameBuffer> m_MainFrameBuffer;
  std::unique_ptr<Shader> m_MainShader;
  std::unique_ptr<Interface> m_MainInterface;
	std::unique_ptr<Engine> m_Engine;
  GLFWwindow *m_Window;

  std::vector<GLFWwindow *> m_Windows;

private:
  void _setUpGLFWContext();
  void _destroyGLFWContext();
  void _miniInputSystem(GLFWwindow *);
  void _run();
  void _emptyWindowVector();

  std::optional<GLFWwindow *> _createWindow(unsigned int, unsigned int,
                                            const char *);
};
