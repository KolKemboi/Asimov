#include "APE_Components.hpp"
#include "APE_FBO.hpp"
#include "APE_camera.hpp"
#include "APE_eventsystem.hxx"
#include "APE_inputsystem.hxx"
#include "APE_interface.hxx"
#include "APE_meshmakerhelper.hpp"
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
#include <tuple>

// set up window data given and set up GLFW context
APE_Window::APE_Window(unsigned int windowWidth, unsigned int windowHeight,
                       const char *windowName)
    : m_WindowWidth(windowWidth), m_WindowHeight(windowHeight),
      m_WindowName(windowName) {
  this->_setUpGLFWContext();
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
    this->m_Windows.push_back(this->m_Window);
  } else {
    printf("ERROR::WINDOW_CREATION\n"); // read on SPDLOG
    this->_destroyGLFWContext();
    std::exit(1);
  }
  glfwMakeContextCurrent(this->m_Window);

  // GLAD LOADING ERROR CHECK
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("ERROR::GLAD ERROR::FAILED TO INIT GLAD\n");
    glfwTerminate();
    std::exit(1);
  }

  glViewport(0, 0, (GLsizei)m_WindowWidth, (GLsizei)m_WindowHeight);
  glEnable(GL_DEPTH_TEST); // for proper 3d rendering

  // for object outlining
  // glDepthFunc(GL_LESS);
  // glEnable(GL_STENCIL_TEST);
  // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  // these dont need to be in the context set up
  // set up shader and framebuffer
  this->m_MainShader = std::make_shared<Shader>(
      "shaders/default/vertex.glsl", "shaders/default/fragment.glsl");

  this->m_MainFrameBuffer =
      std::make_unique<FrameBuffer>(m_WindowWidth, m_WindowHeight);

  this->_setUpPrimitives();

  // Shift->A add primitives
  // it is an Imgui pop up window
  this->m_AddObjectPopUp.SetDispatcher(m_Dispatcher);
  this->m_AddObjectPopUp.SetUpPrimitiveData(_CubePrimitive, _CylinderPrimitive,
                                            _SpherePrimitive, _CapsulePrimitive,
                                            _ConvexMeshPrimitive);

  m_Camera.SetUpCamera(m_CamPos, m_CamUp, -90.0f, 0.0f);

  // input system -> set up everything required for the singleton
  InputSystem::instance().SetVars(m_Camera, m_EventSystem);
  glfwSetKeyCallback(m_Window, InputSystem::KeyCallbackFunc);
  glfwSetMouseButtonCallback(m_Window, InputSystem::MouseButtonCallbackFunc);
  glfwSetCursorPosCallback(m_Window, InputSystem::MouseCallbackFunc);
  glfwSetScrollCallback(m_Window, InputSystem::ScrollCallbackFunc);

  // ensure this runs after the Callback functions
  this->m_MainInterface = std::make_unique<Interface>(this->m_Window);

  // physics
  m_PhysicsWorld = m_PhysicsCommon.createPhysicsWorld();
}

void APE_Window::_run() {

  // will probably use one shader
  this->m_MainShader->UseShader();

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
  while (!glfwWindowShouldClose(m_Window)) {

    // activate physics, should be a UI thing
    for (auto key : m_EventSystem.m_KeysPressed) {
      if (key == KeyPress::P)
        worldrun = false;
      if (key == KeyPress::C)
        worldrun = true;
    }

    this->m_MainShader->SetMat4(
        this->m_Camera.GetViewMatrix(),
        "view"); // view matrix setup, Model View Projection matrix
    this->m_MainShader->SetVec3(m_Camera.GetPosition(),
                                "viewPos"); // for some type of phong shading
    this->m_MainShader->SetVec3(
        m_Camera.GetPosition(),
        "lightPos"); // set lightpos to be cam pos, blender style

    // delta time calculation
    float currTime = glfwGetTime();
    deltaTime = currTime - lastTime;
    lastTime = currTime;
    m_Camera.ResetViewSmooth(deltaTime); // HOME key enables cam to return to
                                         // original pos, using glm::lerp

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

    // probably should be moved somewhere else
    // meanwhile, delete and duplicate abilities
    for (auto key : m_EventSystem.m_KeysPressed) {
      if (key == KeyPress::D) {
        for (auto mod : m_EventSystem.m_ModKeys) {
          if (mod == ModKeys::SHIFT) {
            m_DuplicateSystem.AddDuplicate(m_Registry,
                                           m_Dispatcher); // duplicate
          }
        }
      }
      if (key == KeyPress::DELETE) {
        // bug was here, now fixed
        m_RemoveEntity.RemoveEntity(m_Registry, m_PhysicsWorld); // delete
      }
    }

    // USER interface
    this->m_MainInterface->SetUpNewFrame();
    this->m_MainInterface->SetUpDocking();

    // 			properties window
    // Render properties
    m_Properties.MakeProperties(m_Registry, m_MainShader, lightColor,
                                m_Dispatcher);
    // physics properties
    m_Properties.MakePhysicsProperties(m_Registry, m_PhysicsCommon,
                                       m_PhysicsWorld, m_Dispatcher);

    // set up the projection matrix,
    // this->m_MainShader->SetMat4(projection, "projection");

    // needs to be changed -> use dispatcher to call this when Shift->A is
    // called
    this->m_AddObjectPopUp.SetUpPopUp(this->m_Window, this->m_Registry,
                                      m_PhysicsWorld, m_PhysicsCommon);

    // render call -> render first, then blit the framebuffer, so that the
    // rendered buffer is shown immediately, not the previous buffer as
    // originally put
    m_RenderSystem.RenderEntities(m_MainFrameBuffer, m_Registry, m_MainShader);

    // the viewport setup
    m_Viewport.View(this->m_MainFrameBuffer, m_Camera, m_Registry, m_MainShader,
                    m_EventSystem, m_Dispatcher);

    // selection system -> outliner section
    m_Selection.Selection(m_Registry);

    // m_RenderCollider.RenderColliders(m_MainFrameBuffer, m_Registry,
    // m_MainShader);

    this->m_MainInterface->NewRenderIMGUI(); // render the imgui windows

    // clear the vectors in the input event system
    m_EventSystem.m_KeysPressed.clear();
    m_EventSystem.m_ModKeys.clear();
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
    this->m_MeshMaker = std::make_unique<MeshMakerHelper>(primitive);
    auto tup = m_MeshMaker->ReturnObjectData();
    for (auto &data : tup) {
      if (std::strcmp(data.first.c_str(), "Cube") == 0) {
        _CubePrimitive = data.second;
        printf("Found Cube\n");
      } else if (strcmp(data.first.c_str(), "Cylinder") == 0) {
        _CylinderPrimitive = data.second;
        printf("Found Cylinder\n");
      } else if (std::strcmp(data.first.data(), "Sphere") == 0) {
        _SpherePrimitive = data.second;
        printf("Found Sphere\n");
      } else if (strcmp(data.first.c_str(), "Capsule") == 0) {
        _CapsulePrimitive = data.second;
        printf("Found Capsule\n");
      } else if (std::strcmp(data.first.data(), "ConvexMesh") == 0) {
        _ConvexMeshPrimitive = data.second;
        printf("Found ConvexMesh\n");
      }
    }
  }

  primitives.clear();
}

// clean windows
void APE_Window::_emptyWindowVector() {
  for (GLFWwindow *&window : this->m_Windows) {
    glfwDestroyWindow(window);
    window = nullptr;
    // should be replaced by spdLOG
    printf("DELETED::WINDOW::%d\n", (int)this->m_Windows.size());
  }
  this->m_Windows.clear();
}

// I am not sure I have cleaned everything, but, VALGRIND tells me no memory
// leaks, so maybe RAII???
void APE_Window::CleanUp() {
  m_PhysicsCommon.destroyPhysicsWorld(m_PhysicsWorld);
  this->m_MeshMaker->Clean();
  this->m_MainInterface->DestroyIMGUIContext();
  this->m_MainInterface = nullptr;
  this->m_MainFrameBuffer->Clean();
  this->_emptyWindowVector();
  this->_destroyGLFWContext();
  printf("APE_WINDOW::CLEANED\n");
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
