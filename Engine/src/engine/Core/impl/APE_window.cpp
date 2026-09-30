#include "APE_Components.hpp"
#include "APE_FBO.hpp"
#include "APE_KeyEvents.hpp"
#include "APE_camera.hpp"
#include "APE_eventsystem.hxx"
#include "APE_inputsystem.hxx"
#include "APE_interface.hxx"
#include "APE_meshmakerhelper.hpp"
#include <APE_menubar.hxx>
#include <APE_window.hpp>
#include <GLFW/glfw3.h>
#include <ImGuiFileDialog.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <glm/ext/vector_float3.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_node_editor.h>
#include <memory>
#include <optional>
#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Vector3.h>
#include <string>
#include <sys/types.h>
#include <tuple>

// set up window data given and set up GLFW context
APE_Window::APE_Window(unsigned int windowWidth, unsigned int windowHeight,
                       const char *windowName)
    : m_WindowWidth(windowWidth), m_WindowHeight(windowHeight),
      m_WindowName(windowName) {
  this->_setUpGLFWContext();

#ifdef DEBUG__
  spdlog::set_level(spdlog::level::debug);
#else
  spdlog::set_level(spdlog::level::warn);
#endif
}

void APE_Window::_setUpGLFWContext() {
  // set up glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_RESIZABLE,
                 GL_FALSE); // take away the ability to resize the window,
                            // useful in tiling window managers

  // create the window or error, in this case, exit from the app totally
  // app requires a first UI
  if (std::optional<GLFWwindow *> window =
          this->_createWindow(m_WindowWidth, m_WindowHeight, m_WindowName)) {
    this->m_Window = *window;
    this->m_Windows_Vector.push_back(this->m_Window);
  } else {
    spdlog::error("ERROR::WINDOW_CREATION"); // read on SPDLOG
    this->_destroyGLFWContext();
    std::exit(1);
  }
  glfwMakeContextCurrent(this->m_Window);

  // GLAD LOADING ERROR CHECK
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::error("ERROR::GLAD ERROR::FAILED TO INIT GLAD");
    glfwTerminate();
    std::exit(1);
  }

  glViewport(0, 0, (GLsizei)m_WindowWidth, (GLsizei)m_WindowHeight);
  glEnable(GL_DEPTH_TEST); // for proper 3d rendering

  // for object outlining
  glDepthFunc(GL_LESS);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  // these dont need to be in the context set up
  // set up shader and framebuffer
  this->m_MainShader_SharedPtr = std::make_shared<Shader>(
      "shaders/default/vertex.glsl", "shaders/default/fragment.glsl");

  this->m_MainFrameBuffer_UniquePtr =
      std::make_unique<FrameBuffer>(m_WindowWidth, m_WindowHeight);

  this->_setUpPrimitives();

  // Shift->A add primitives
  // it is an Imgui pop up window
  this->m_AddObjectPopUp.SetDispatcher(m_Dispatcher);
  this->m_AddObjectPopUp.SetUpPrimitiveData(
      _CubePrimitive_Tuple, _CylinderPrimitive_Tuple, _SpherePrimitive_Tuple,
      _CapsulePrimitive_Tuple, _ConvexMeshPrimitive_Tuple);

  m_Camera.SetUpCamera(m_CamPos, m_CamUp, -90.0f, 0.0f);

  // input system -> set up everything required for the singleton
  InputSystem::instance().SetVars(m_Camera, m_EventSystem);
  glfwSetKeyCallback(m_Window, InputSystem::KeyCallbackFunc);
  glfwSetMouseButtonCallback(m_Window, InputSystem::MouseButtonCallbackFunc);
  glfwSetCursorPosCallback(m_Window, InputSystem::MouseCallbackFunc);
  glfwSetScrollCallback(m_Window, InputSystem::ScrollCallbackFunc);

  // ensure this runs after the Callback functions
  this->m_MainInterface_UniquePtr = std::make_unique<Interface>(this->m_Window);

  // physics
  m_PhysicsWorld = m_PhysicsCommon.createPhysicsWorld();

  // robots
  this->m_RobotMaker_UniquePtr =
      std::make_unique<RobotMaker>(this->m_Dispatcher);
}

void APE_Window::_run() {

  // will probably use one shader
  this->m_MainShader_SharedPtr->UseShader();

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f),
                                (float)m_WindowWidth / (float)m_WindowHeight,
                                0.1f, 100.0f);
  // probably need a better time tracking
  // chrono maybe
  float deltaTime = 0.0f;
  float timeScale = 0.5f;
  float lastTime = glfwGetTime();

  // sets up rp3d ptrs to the main world and physics common, and a ptr to the
  // registry
  m_AddCollider.SetColliderRequirements(m_PhysicsWorld, m_PhysicsCommon,
                                        m_Registry);
  m_AddCollider.SetDispatcher(m_Dispatcher); // event dispatcher

  m_Camera.SetTarget(glm::vec3(0.0f));
  m_Camera.SetInitialState(m_CamPos, glm::vec3(0.0f), -90.0f, 0.0f);

  // DUMMY DATA FOR LIGHTS
  glm::vec3 lightColor = glm::vec3(1.0f);
  // this->m_MainShader->SetVec3(lightColor, "lightColor");

  unsigned int count = 0;

  bool worldrun = false;

  // Coding font

  while (!glfwWindowShouldClose(m_Window)) {

    // activate physics, should be a UI thing
    if (m_EventSystem.Keys & (int)Alicia::P) {
      worldrun = false;
    }
    if (m_EventSystem.Keys & (int)Alicia::C) {
      worldrun = true;
    }

    this->m_MainShader_SharedPtr->SetMat4(
        this->m_Camera.GetViewMatrix(),
        "view"); // view matrix setup, Model View Projection matrix
    this->m_MainShader_SharedPtr->SetVec3(
        m_Camera.GetPosition(),
        "viewPos"); // for some type of phong shading
    this->m_MainShader_SharedPtr->SetVec3(
        m_Camera.GetPosition(),
        "lightPos"); // set lightpos to be cam pos, blender style

    // delta time calculation
    float currTime = glfwGetTime();
    deltaTime = currTime - lastTime;
    lastTime = currTime;
    m_Camera.ResetViewSmooth(deltaTime); // HOME key enables cam to return to
                                         // original pos, using glm::lerp

    // check if mouse is clicked

    // update it here, check if the transforms arent the same,
    // I am stupid, I am stupid -> Coll Leclerc
    if (worldrun) {
      // use colider to update positions
      m_PhysicsWorld->update(deltaTime);

      // grab transform, physics body and physics data, update the physics body
      // using physics grab the new transform, retrieve the position, then
      // update the physics body and mesh position
      auto view = m_Registry.view<Transform, PhysicsBody, PhysicsData>();
      for (auto [entity, transform, fzxBody, fzxData] : view.each()) {

        // grab new pos
        const rp3d::Transform &newTrans = fzxBody.s_Body->getTransform();
        const rp3d::Vector3 &newPosition = newTrans.getPosition();

        // update physics data pos
        fzxData.s_Position_C = newPosition;

        // update mesh position
        // mesh pos is a glm::vec3 while new transform is a rp3d::Vector3 hence
        // the value wise update, anywhere rp3d::Vector3 and glm::vec3 will need
        // an interchange of data, this will be the format,
        transform.s_Position.x = newPosition.x;
        transform.s_Position.y = newPosition.y;
        transform.s_Position.z = newPosition.z;
      }
    }
    // USER interface
    this->m_MainInterface_UniquePtr->SetUpNewFrame();
    this->m_MainInterface_UniquePtr->SetUpDocking();

    this->m_MainInterface_UniquePtr->SetLayoutTab([&](ImGuiID dock) {
      ImGui::Begin("Main Viewport");
      m_Viewport.View(m_MainFrameBuffer_UniquePtr, m_Camera, m_Registry,
                      m_MainShader_SharedPtr, m_EventSystem, m_Dispatcher);
      ImGui::End();

      m_Properties.MakeProperties(m_Registry, m_MainShader_SharedPtr,
                                  lightColor, m_Dispatcher);
      m_Properties.MakePhysicsProperties(m_Registry, m_PhysicsCommon,
                                         m_PhysicsWorld, m_Dispatcher);

      m_Selection.Selection(m_Registry);
    });

    m_MainInterface_UniquePtr->SetScriptingTab([&](ImGuiID dock) {
      ImGui::Begin("Code Viewport");
      m_Viewport.View(m_MainFrameBuffer_UniquePtr, m_Camera, m_Registry,
                      m_MainShader_SharedPtr, m_EventSystem, m_Dispatcher);
      ImGui::End();
      ImGui::Begin("Code Editor");
      m_ScriptingTab.Draw();
      ImGui::End();
    });

    m_MainInterface_UniquePtr->SetNodeEditorTab([&](ImGuiID dock) {
      ImGui::Begin("Node Viewport");
      m_Viewport.View(m_MainFrameBuffer_UniquePtr, m_Camera, m_Registry,
                      m_MainShader_SharedPtr, m_EventSystem, m_Dispatcher);
      ImGui::End();

      ImGui::Begin("Node Editor");
      m_NodeEditor.Draw();
      ImGui::End();
    });

    // // probably should be moved somewhere else
    // // meanwhile, delete and duplicate abilities
    if (m_EventSystem.Keys & (int)Alicia::SHIFT &&
        m_EventSystem.Keys & (int)Alicia::D) {
      m_DuplicateSystem.AddDuplicate(m_Registry,
                                     m_Dispatcher); // duplicate
    }
    if (m_EventSystem.Keys & (int)Alicia::SHIFT &&
        m_EventSystem.Keys & (int)Alicia::X) {
      ImGui::OpenPopup("Delete Object");
    }

    if (m_ConfirmPopUp.ConfirmDelete())
      //     // bug was here, now fixed
      m_RemoveEntity.RemoveEntity(m_Registry, m_PhysicsWorld); // delete

    SetUpMenuBar(m_Window, m_Dispatcher);

    // set up the projection matrix,
    // this->m_MainShader->SetMat4(projection, "projection");

    // needs to be changed -> use dispatcher to call this when Shift->A is
    // called
    this->m_AddObjectPopUp.SetUpPopUp(this->m_Window, this->m_Registry,
                                      m_PhysicsWorld, m_PhysicsCommon);

    // render call -> render first, then blit the framebuffer, so that the
    // rendered buffer is shown immediately, not the previous buffer as
    // originally put
    m_RenderSystem.RenderEntities(m_MainFrameBuffer_UniquePtr, m_Registry,
                                  m_MainShader_SharedPtr);
    //
    // m_RenderCollider.RenderColliders(m_MainFrameBuffer, m_Registry,
    // m_MainShader);

    this->m_MainInterface_UniquePtr
        ->NewRenderIMGUI(); // render the imgui windows

    // clear the vectors in the input event system
    // m_EventSystem.m_KeysPressed.clear();
    // m_EventSystem.m_ModKeys.clear();
    m_EventSystem.Keys = 0;
    glfwSwapBuffers(this->m_Window);
    glfwPollEvents();
  }
}

void APE_Window::_setUpPrimitives() {
  // load the primitives on start
  std::vector<std::string> primitives = {
      "models/primitives/Cylinder.obj",   "models/primitives/Cube.obj",
      "models/primitives/Sphere.obj",     "models/primitives/Capsule.obj",
      "models/primitives/ConvexMesh.obj",
  };

  // this logic works well with primitives
  // Does not need to be changed to fit non primitive
  // DONT TOUCH THIS!!!!
  for (auto &primitive : primitives) {
    this->m_MeshMaker_UniquePtr = std::make_unique<MeshMakerHelper>(primitive);
    auto tup = m_MeshMaker_UniquePtr->ReturnObjectData();
    for (auto &data : tup) {
      if (std::strcmp(data.first.c_str(), "Cube") == 0) {
        _CubePrimitive_Tuple = data.second;
        spdlog::info("Found Cube");
      } else if (strcmp(data.first.c_str(), "Cylinder") == 0) {
        _CylinderPrimitive_Tuple = data.second;
        spdlog::info("Found Cylinder");
      } else if (std::strcmp(data.first.data(), "Sphere") == 0) {
        _SpherePrimitive_Tuple = data.second;
        spdlog::info("Found Sphere");
      } else if (strcmp(data.first.c_str(), "Capsule") == 0) {
        _CapsulePrimitive_Tuple = data.second;
        spdlog::info("Found Capsule");
      } else if (std::strcmp(data.first.data(), "ConvexMesh") == 0) {
        _ConvexMeshPrimitive_Tuple = data.second;
        spdlog::info("Found ConvexMesh");
      }
    }
  }
  primitives.clear();
}

// clean windows
void APE_Window::_emptyWindowVector() {
  for (GLFWwindow *&window : this->m_Windows_Vector) {
    glfwDestroyWindow(window);
    window = nullptr;
    // should be replaced by spdLOG
    spdlog::info("DELETED::WINDOW::", (int)this->m_Windows_Vector.size());
  }
  this->m_Windows_Vector.clear();
}

// I am not sure I have cleaned everything, but, VALGRIND tells me no memory
// leaks, so maybe RAII???
void APE_Window::CleanUp() {
  m_PhysicsCommon.destroyPhysicsWorld(m_PhysicsWorld);
  this->m_MeshMaker_UniquePtr->Clean();
  this->m_MainInterface_UniquePtr->DestroyIMGUIContext();
  this->m_MainInterface_UniquePtr = nullptr;
  this->m_MainFrameBuffer_UniquePtr->Clean();
  this->_emptyWindowVector();
  this->_destroyGLFWContext();
  spdlog::info("APE_WINDOW::CLEANED");
}

void APE_Window::RunEngine() { this->_run(); }

void APE_Window::_destroyGLFWContext() { glfwTerminate(); }

// make a window, if not made, return a nullptr
std::optional<GLFWwindow *> APE_Window::_createWindow(unsigned int width,
                                                      unsigned int height,
                                                      const char *name) {

  GLFWwindow *window = glfwCreateWindow(width, height, name, NULL, NULL);
  if (window == NULL) {
    return nullptr;
  }
  return window;
}
