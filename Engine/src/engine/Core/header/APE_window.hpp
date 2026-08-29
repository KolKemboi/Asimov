#pragma once
#include <tuple>
#include <unordered_map>
#include <vector>
#ifndef __GLAD_GAURD__
#include <glad/glad.h>
#endif
#include <APE_AddEntitySystem.hpp>
#include <APE_FBO.hpp>
#include <APE_IBO.hpp>
#include <APE_VAO.hpp>
#include <APE_VBO.hpp>
#include <APE_engine.hpp>
#include <APE_interface.hpp>
#include <APE_loadmodelhelper.hpp>
#include <APE_shader.hpp>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>
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
  std::shared_ptr<Shader> m_MainShader;
  std::unique_ptr<Interface> m_MainInterface;
  // std::unique_ptr<Engine> m_Engine;
  GLFWwindow *m_Window;


  std::unique_ptr<AddEntitySystem> m_AddEntitySystem;

  std::unique_ptr<ModelLoaderHelper> m_ModelLoaderHelper;

  std::vector<GLFWwindow *> m_Windows;

  entt::registry m_Registry;

private:

  void _setUpGLFWContext();
  void _destroyGLFWContext();
  void _miniInputSystem(GLFWwindow *);
  void _run();
  void _emptyWindowVector();

  std::optional<GLFWwindow *> _createWindow(unsigned int, unsigned int,
                                            const char *);
};
