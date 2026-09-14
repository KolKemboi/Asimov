#pragma once
#include "APE_Dispatcher.hpp"
#include "APE_properties.hxx"
#include "APE_viewport.hxx"
#include <APE_confirmpopup.hxx>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <tuple>
#include <vector>
#ifndef __GLAD_GAURD__
#include <glad/glad.h>
#endif
#include <APE_AddCollider.hxx>
#include <APE_DuplicateSystem.hxx>
#include <APE_EditRegistrySystem.hxx>
#include <APE_FBO.hpp>
#include <APE_IBO.hpp>
#include <APE_RenderColliderSystem.hxx>
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

// --INFO: this class should be renamed as Resource Center
// a monolith of some sort
namespace rp3d = reactphysics3d; // prevent using reactphysics3d everywhere

class APE_Window {
public:
  APE_Window(unsigned int, unsigned int, const char *);
  void RunEngine();
  void CleanUp();

private:
  // this is for viewport rendering
  std::unique_ptr<FrameBuffer>
      m_MainFrameBuffer; // for rendering to, to place on an IMGUI window

  // prevent multiple program usage, a super shader that
  // needs to be attached once, and uniforms affect how
  // it works
  std::shared_ptr<Shader> m_MainShader;
  std::unique_ptr<Interface> m_MainInterface; // Imgui SetUp
  Viewport m_Viewport;     // for the frame buffer and Imguizmo widgets
  Properties m_Properties; // stores editable properties of selected models ->
                           // both physics and render properties
  ConfirmPopUp m_ConfirmPopUp;

  SelectionSystem m_Selection;           // sets up the outliner,
  DuplicateMeshSystem m_DuplicateSystem; // Shift-D ability, -> blender style
  EventSystem m_EventSystem; // this event system is more of input events

  // Window specifics
  GLFWwindow *m_Window;
  std::vector<GLFWwindow *>
      m_Windows; // stores ptrs to the windows present, for ease of cleaning
  unsigned int m_WindowWidth, m_WindowHeight;
  const char *m_WindowName;

  // ECS stuff
  entt::registry
      m_Registry; // super registry holding components of every APE entity
  RenderSystem m_RenderSystem;
  RenderColliderSystem m_RenderCollider;
  RemoveEntitySystem m_RemoveEntity; // Delete to remove selected entities

  Camera m_Camera;         // main Engine camera
  Dispatcher m_Dispatcher; // a super class that calls other functions when
                           // other functions run

  // for primitives
  std::unique_ptr<MeshMakerHelper> m_MeshMaker;
  AddObjectPopUp m_AddObjectPopUp;

  // Physics stuff
  rp3d::PhysicsCommon m_PhysicsCommon;
  rp3d::PhysicsWorld *m_PhysicsWorld;
  AddColliderSystem m_AddCollider;

private:
  // I can guarantee these, the primitives will be in a specific order
  // add other prims -> plane, capsule,
  std::tuple<unsigned int, unsigned int>
      _CubePrimitive; // std::get<0>(_CubePrimitive) -> gets you the 1st U_INT
  std::tuple<unsigned int, unsigned int> _SpherePrimitive;
  std::tuple<unsigned int, unsigned int> _CylinderPrimitive;
  std::tuple<unsigned int, unsigned int> _ConvexMeshPrimitive;
  std::tuple<unsigned int, unsigned int> _CapsulePrimitive;

  // initial camera positions and view setting
  glm::vec3 m_CamPos = glm::vec3(0.0f, 0.0f, -10.0f);
  glm::vec3 m_CamUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 m_View = glm::mat4(1.0f);

  // private methods to set up this class
  void _setUpPrimitives();
  void _setUpGLFWContext();
  void _destroyGLFWContext();
  void _run();

  void _emptyWindowVector();

  // for making windows, only one window needs to exist, others can fail
  std::optional<GLFWwindow *> _createWindow(unsigned int, unsigned int,
                                            const char *);
};
