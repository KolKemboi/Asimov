#include "APE_Components.hpp"
#include "APE_FBO.hpp"
#include "APE_meshmakerhelper.hpp"
#include <APE_window.hpp>
#include <GLFW/glfw3.h>
#include <ImGuiFileDialog.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <imgui.h>
#include <imgui_node_editor.h>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tuple>

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
    printf("ERROR::WINDOW_CREATION\n");
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

  // set up shader and framebuffer
  this->m_MainShader = std::make_shared<Shader>(
      "shaders/default/vertex.glsl", "shaders/default/fragment.glsl");
  this->m_MainFrameBuffer =
      std::make_unique<FrameBuffer>(m_WindowWidth, m_WindowHeight);

  this->m_MainInterface = std::make_unique<Interface>(this->m_Window);

  this->_setUpPrimitives();

  // --TODO: FIX THE SMART PTR TO NORMAL
  // dunno why this is a smart ptr, will fix this
  this->m_AddEntitySystem = std::make_unique<AddEntitySystem>();

  for (unsigned int i = 0; i < 1; i++) {
    this->m_AddEntitySystem->AddSphereSystem(m_Registry,
                                             std::get<0>(_SpherePrimitive),
                                             std::get<1>(_SpherePrimitive));

    // this->m_AddEntitySystem->AddCubeSystem(
    //     m_Registry, std::get<0>(_CubePrimitive),
    //     std::get<1>(_CubePrimitive));

    this->m_AddEntitySystem->AddCylinderSystem(m_Registry,
                                               std::get<0>(_CylinderPrimitive),
                                               std::get<1>(_CylinderPrimitive));
  }
  this->m_Camera = std::make_unique<Camera>(m_CamPos, m_CamUp);
}

void APE_Window::_run() {

  this->m_MainShader->UseShader();
  this->m_MainShader->SetMat4(this->m_Camera->GetViewMatrix(), "view");

  glm::mat4 projection;
  projection = glm::perspective(
      glm::radians(45.0f),
      (float)this->m_WindowWidth / (float)this->m_WindowHeight, 0.1f, 100.0f);

  while (!glfwWindowShouldClose(m_Window)) {
    // call the renderer and give it the frame buffer and a vector of objects
    // with the renderable component to render
    this->_miniInputSystem(this->m_Window);
    this->m_MainInterface->SetUpNewFrame();
    this->m_MainInterface->SetUpDocking();
    this->m_MainShader->SetMat4(projection, "projection");

    // auto view = m_Registry.view<Name, ObjectCount>();
    // for (auto [ent, name, count] : view.each()) {
    //   printf("%s -> %d \n", name.s_Name.c_str(), count.s_Count);
    // }

    m_RenderSystem.RenderEntities(m_MainFrameBuffer, m_Registry);

    ImGui::Begin("Viewport");

    ImVec2 avail = ImGui::GetContentRegionAvail();

    // Desired aspect ratio (16:9)
    const float aspect = 16.0f / 9.0f;

    // Compute the largest image that fits while preserving the aspect ratio
    float imageWidth = avail.x;
    float imageHeight = imageWidth / aspect;

    if (imageHeight > avail.y) {
      imageHeight = avail.y;
      imageWidth = imageHeight * aspect;
    }

    // Center the image in the viewport
    ImVec2 cursor = ImGui::GetCursorPos();

    ImGui::SetCursorPos(ImVec2(cursor.x + (avail.x - imageWidth) * 0.5f,
                               cursor.y + (avail.y - imageHeight) * 0.5f));

    ImGui::Image(
        (ImTextureID)(intptr_t)this->m_MainFrameBuffer->ReturnColorTexture(),
        ImVec2(imageWidth, imageHeight), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    this->m_MainInterface->NewRenderIMGUI();

    glfwSwapBuffers(this->m_Window);
    glfwPollEvents();
  }
}

void APE_Window::_setUpPrimitives() {
  // load the primitives on start
  std::vector<std::string> primitives = {
      "models/primitives/Cylinder.obj",
      "models/primitives/Cube.obj",
      "models/primitives/Sphere.obj",
  };

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
        printf("Found Spehere\n");
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

  this->_emptyWindowVector();

  this->_destroyGLFWContext();
  printf("APE_WINDOW::CLEANED\n");
}

void APE_Window::RunEngine() { this->_run(); }

void APE_Window::_destroyGLFWContext() { glfwTerminate(); }

void APE_Window::_miniInputSystem(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

std::optional<GLFWwindow *> APE_Window::_createWindow(unsigned int width,
                                                      unsigned int height,
                                                      const char *name) {

  GLFWwindow *window = glfwCreateWindow(width, height, name, NULL, NULL);
  if (window == NULL) {
    return nullptr;
  }
  return window;
}
