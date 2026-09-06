#pragma once
#include "APE_properties.hxx"
#include "APE_viewport.hxx"
#include <tuple>
#include <vector>
#ifndef __GLAD_GAURD__
#include <glad/glad.h>
#endif
#include <APE_AddEntitySystem.hpp>
#include <APE_DuplicateSystem.hxx>
#include <APE_FBO.hpp>
#include <APE_IBO.hpp>
#include <APE_RenderingSystem.hpp>
#include <APE_SelectionSystem.hxx>
#include <APE_VAO.hpp>
#include <APE_VBO.hpp>
#include <APE_addobjectpopup.hxx>
#include <APE_camera.hpp>
#include <APE_eventsystem.hxx>
#include <APE_inputsystem.hxx>
#include <APE_interface.hxx>
#include <APE_loadmodelhelper.hpp>
#include <APE_meshmakerhelper.hpp>
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
  // this is for viewport rendering
  std::unique_ptr<FrameBuffer> m_MainFrameBuffer;
  std::shared_ptr<Shader> m_MainShader;
  std::unique_ptr<Interface> m_MainInterface;
  Viewport m_Viewport;
  Properties m_Properties;

  SelectionSystem m_Selection;
  DuplicateMeshSystem m_DuplicateSystem;
	EventSystem m_EventSystem;

  // Window specifics
  GLFWwindow *m_Window;
  std::vector<GLFWwindow *> m_Windows;
  unsigned int m_WindowWidth, m_WindowHeight;
  const char *m_WindowName;

  // ECS stuff
  entt::registry m_Registry;
  RenderSystem m_RenderSystem;

  Camera m_Camera;

  // for primitives
  std::unique_ptr<MeshMakerHelper> m_MeshMaker;
  AddEntitySystem m_AddEntitySystem;
  AddObjectPopUp m_AddObjectPopUp;

private:
  // I can guarantee these, the primitives will be in a specific order
  // add other prims -> plane, capsule,
  std::tuple<unsigned int, unsigned int> _CubePrimitive;
  std::tuple<unsigned int, unsigned int> _SpherePrimitive;
  std::tuple<unsigned int, unsigned int> _CylinderPrimitive;
  std::tuple<unsigned int, unsigned int> _ConvexMeshPrimitive;
  std::tuple<unsigned int, unsigned int> _CapsulePrimitive;

  // initial camera positions and view setting
  glm::vec3 m_CamPos = glm::vec3(0.0f, 5.0f, -10.0f);
  glm::vec3 m_CamUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 m_View = glm::mat4(1.0f);

  void _setUpPrimitives();

  void _setUpGLFWContext();
  void _destroyGLFWContext();
  void _run();

  void _emptyWindowVector();

  // for making windows, only one window needs to exist, others can fail
  std::optional<GLFWwindow *> _createWindow(unsigned int, unsigned int,
                                            const char *);
};
