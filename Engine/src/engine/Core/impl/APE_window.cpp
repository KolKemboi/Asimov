#include "APE_FBO.hpp"
#include "APE_camera.hpp"
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
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
  glEnable(GL_DEPTH_TEST);

  // these dont need to be in the context set up
  // set up shader and framebuffer
  this->m_MainShader = std::make_shared<Shader>(
      "shaders/default/vertex.glsl", "shaders/default/fragment.glsl");

  this->m_MainFrameBuffer =
      std::make_unique<FrameBuffer>(m_WindowWidth, m_WindowHeight);

  this->_setUpPrimitives();
  this->m_AddObjectPopUp.SetUpPrimitiveData(_CubePrimitive, _CylinderPrimitive,
                                            _SpherePrimitive, _CapsulePrimitive,
                                            _ConvexMeshPrimitive);

  m_Camera.SetUpCamera(m_CamPos, m_CamUp, -90.0f, 0.0f);

  // input system -> set up everything required for the singleton
  InputSystem::instance().SetVars(m_Camera);
  glfwSetKeyCallback(m_Window, InputSystem::KeyCallbackFunc);
  glfwSetMouseButtonCallback(m_Window, InputSystem::MouseButtonCallbackFunc);
  glfwSetCursorPosCallback(m_Window, InputSystem::MouseCallbackFunc);
  glfwSetScrollCallback(m_Window, InputSystem::ScrollCallbackFunc);

  // ensure this runs after the Callback functions
  this->m_MainInterface = std::make_unique<Interface>(this->m_Window);
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

  m_Camera.SetTarget(glm::vec3(0.0f));
  m_Camera.SetInitialState(m_CamPos, glm::vec3(0.0f), -90.0f, 0.0f);

  // DUMMY DATA FOR LIGHTS
  glm::vec3 lightColor = glm::vec3(1.0f);
  // this->m_MainShader->SetVec3(lightColor, "lightColor");

  while (!glfwWindowShouldClose(m_Window)) {

    this->m_MainShader->SetMat4(this->m_Camera.GetViewMatrix(), "view");
    this->m_MainShader->SetVec3(m_Camera.GetPosition(), "viewPos");
    this->m_MainShader->SetVec3(m_Camera.GetPosition(), "lightPos");
    // call the renderer and give it the frame buffer and a vector of objects
    // with the renderable component to render
    float currTime = glfwGetTime();
    deltaTime = currTime - lastTime;
    lastTime = currTime;
    m_Camera.ResetViewSmooth(deltaTime);

    // USER interface
    this->m_MainInterface->SetUpNewFrame();
    this->m_MainInterface->SetUpDocking();
    m_Properties.MakeProperties(m_Registry, m_MainShader, lightColor);
    this->m_MainShader->SetMat4(projection, "projection");
    this->m_AddObjectPopUp.SetUpPopUp(this->m_Window, this->m_Registry);
    m_Viewport.View(this->m_MainFrameBuffer, m_Camera, m_Registry,
                    m_MainShader);

    m_Selection.Selection(m_Registry);

    m_RenderSystem.RenderEntities(m_MainFrameBuffer, m_Registry, m_MainShader);

    this->m_MainInterface->NewRenderIMGUI();

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

void APE_Window::_emptyWindowVector() {

  for (GLFWwindow *&window : this->m_Windows) {
    glfwDestroyWindow(window);
    window = nullptr;
    printf("DELETED::WINDOW::%d\n", (int)this->m_Windows.size());
  }
  this->m_Windows.clear();
}

void APE_Window::CleanUp() {
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

std::optional<GLFWwindow *> APE_Window::_createWindow(unsigned int width,
                                                      unsigned int height,
                                                      const char *name) {

  GLFWwindow *window = glfwCreateWindow(width, height, name, NULL, NULL);
  if (window == NULL) {
    return nullptr;
  }
  return window;
}
