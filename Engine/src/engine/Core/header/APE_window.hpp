#pragma once
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#ifndef __GLAD_GAURD__
#include <glad/glad.h>
#endif
#include <APE_AddEntitySystem.hpp>
#include <APE_FBO.hpp>
#include <APE_IBO.hpp>
#include <APE_RenderingSystem.hpp>
#include <APE_VAO.hpp>
#include <APE_VBO.hpp>
#include <APE_camera.hpp>
#include <APE_engine.hpp>
#include <APE_interface.hpp>
#include <APE_loadmodelhelper.hpp>
#include <APE_meshmakerhelper.hpp>
#include <APE_properties.hpp>
#include <APE_shader.hpp>
#include <APE_types.hpp>
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
  GLFWwindow *m_Window;
  std::unique_ptr<AddEntitySystem> m_AddEntitySystem;
  std::vector<GLFWwindow *> m_Windows;
  entt::registry m_Registry;
  std::unique_ptr<MeshMakerHelper> m_MeshMaker;

  RenderSystem m_RenderSystem;
  std::unique_ptr<Camera> m_Camera;
  Properties m_Properties;

private:
  // VAO,IndexCount
  std::tuple<unsigned int, unsigned int> _CubePrimitive;
  std::tuple<unsigned int, unsigned int> _SpherePrimitive;
  std::tuple<unsigned int, unsigned int> _CylinderPrimitive;

  glm::vec3 m_CamPos = glm::vec3(0.0f, 5.0f, -10.0f);
  glm::vec3 m_CamUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 m_View = glm::mat4(1.0f);

  void _setUpPrimitives();
  void _setUpGLFWContext();
  void _destroyGLFWContext();
  void _miniInputSystem(GLFWwindow *);
  void _run();
  void _emptyWindowVector();

  std::optional<GLFWwindow *> _createWindow(unsigned int, unsigned int,
                                            const char *);
};
