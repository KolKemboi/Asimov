#include "APE_ECS.hpp"
#include "APE_FBO.hpp"
#include <APE_window.hpp>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <imgui.h>
#include <memory>
#include <optional>
#include <utility>

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
  // this->m_Engine = std::make_unique<Engine>();

  this->m_AddEntitySystem = std::make_unique<AddEntitySystem>();
  this->m_ModelLoaderHelper =
      std::make_unique<ModelLoaderHelper>("models/primitives/primitives.obj");
  /*
   * ape object
   * load all the verts into m_MainVertexBuffer
   * load all the indices into m_MainIndicesBuffer
   * store the name of the model, start of the idxs and idxcount
   */

  unsigned int vertexOffset = 0;

  for (auto &object : this->m_ModelLoaderHelper->m_Objects) {

    std::string name = object.s_Name;
    // printf("%s\n", name.c_str());

    unsigned int startIndex = m_MainIndexBuffer.size();

    // --------------------
    // Vertices
    // --------------------

    for (auto &vert : object.s_Vertices) {
      m_MainVertexBuffer.push_back(vert.s_Position.x);
      m_MainVertexBuffer.push_back(vert.s_Position.y);
      m_MainVertexBuffer.push_back(vert.s_Position.z);

      m_MainVertexBuffer.push_back(vert.s_Normal.x);
      m_MainVertexBuffer.push_back(vert.s_Normal.y);
      m_MainVertexBuffer.push_back(vert.s_Normal.z);

      m_MainVertexBuffer.push_back(vert.s_TexCoords.x);
      m_MainVertexBuffer.push_back(vert.s_TexCoords.y);
    }

    // --------------------
    // Indices
    // --------------------

    for (auto idx : object.s_Indices) {
      m_MainIndexBuffer.push_back(idx + vertexOffset);
    }

    // Number of indices belonging to THIS object
    unsigned int indexCount = m_MainIndexBuffer.size() - startIndex;

    // Store:
    //   first index
    //   number of indices
    m_MainModelMapping[name] = std::make_pair(startIndex, indexCount);

    // Next object's vertices start here
    vertexOffset += object.s_Vertices.size();
  }
  for (const auto &val : m_MainVertexBuffer) {
    printf("%f", val);
  }

  printf("%s %d %d\n", "Cube", std::get<0>(m_MainModelMapping["Cube"]),
         std::get<1>(m_MainModelMapping["Cube"]));
  printf("%s %d %d\n", "Plane", std::get<0>(m_MainModelMapping["Plane"]),
         std::get<1>(m_MainModelMapping["Plane"]));
  printf("%s %d %d\n", "Sphere", std::get<0>(m_MainModelMapping["Sphere"]),
         std::get<1>(m_MainModelMapping["Sphere"]));
  // printf("%s %d %d\n", "Cylinder",
  // std::get<0>(m_MainModelMapping["Cylinder"]),
  //        std::get<1>(m_MainModelMapping["Cylinder"]));
  this->m_MainVAO = std::make_unique<VertexArray>();
  this->m_MainVBO = std::make_unique<VertexBuffer>();
  this->m_MainIBO = std::make_unique<IndexBuffer>();

  this->m_MainVAO->GenVertexArrays();
  this->m_MainVAO->BindVertexArray();
  this->m_MainVBO->GenVertexBuffers(m_MainVertexBuffer,
                                    m_MainVertexBuffer.size() * sizeof(float));

  this->m_MainIBO->GenIndexBuffers(m_MainIndexBuffer, m_MainIndexBuffer.size() *
                                                          sizeof(unsigned int));
  this->m_MainVAO->AttribPointerSetUp();
}

void APE_Window::_run() {

  this->m_MainShader->UseShader();
  this->m_MainVAO->BindVertexArray();

  while (!glfwWindowShouldClose(m_Window)) {
    // call the renderer and give it the frame buffer and a vector of objects
    // with the renderable component to render
    this->_miniInputSystem(this->m_Window);
    this->m_MainInterface->SetUpNewFrame();
    this->m_MainInterface->SetUpDocking();

    auto renderView = m_Registry.view<Renderable>();

    this->m_MainFrameBuffer->BindFrameBuffer();
    glClearColor(0.2, 0.2, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto [entity, renderable] : renderView.each()) {
      // printf("%d\n", renderable.s_IndexCount);
      // printf("%d\n", renderable.s_StartIndex);

      glDrawElements(GL_TRIANGLES, renderable.s_IndexCount, GL_UNSIGNED_INT,
                     (void *)(renderable.s_StartIndex * sizeof(unsigned int)));
      // glDrawElements(GL_TRIANGLES, renderable.s_IndexCount, GL_UNSIGNED_INT,
      //                (void *)0);
    }
    this->m_MainFrameBuffer->UnBindFrameBuffer();

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
    ImGui::Begin("Outliner");
    auto name_view = m_Registry.view<Name>();
    for (auto [entity, name] : name_view.each()) {
      ImGui::Text("%s", name.s_Name.c_str());
    }

    if (glfwGetKey(this->m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      if (glfwGetKey(this->m_Window, GLFW_KEY_A) == GLFW_PRESS)
        ImGui::OpenPopup("Add Object");

    // if (ImGui::Button("Open")) {
    // }

    if (ImGui::BeginPopupModal("Add Object", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      // ImGui::Text("Hello!");

      if (ImGui::Button("Cube")) {
        this->m_AddEntitySystem->AddCubeSystem(
            m_Registry, std::get<0>(m_MainModelMapping["Cube"]),
            std::get<1>(m_MainModelMapping["Cube"]));
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::Button("Plane")) {
        this->m_AddEntitySystem->AddPlaneSystem(
            m_Registry, std::get<0>(m_MainModelMapping["Plane"]),
            std::get<1>(m_MainModelMapping["Plane"]));
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::Button("Sphere")) {
        this->m_AddEntitySystem->AddSphereSystem(
            m_Registry, std::get<0>(m_MainModelMapping["Sphere"]),
            std::get<1>(m_MainModelMapping["Sphere"]));
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::Button("Cylinder")) {
        this->m_AddEntitySystem->AddCylinderSystem(
            m_Registry, std::get<0>(m_MainModelMapping["Cylinder"]),
            std::get<1>(m_MainModelMapping["Cylinder"]));
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }
    ImGui::End();
    ImGui::Begin("Properties");

    ImGui::Text("Basic Widgets");
    ImGui::Separator();

    ImGui::Button("Button");
    ImGui::SameLine();
    ImGui::SmallButton("Small");

    static bool check = false;
    ImGui::Checkbox("Enable feature", &check);

    static int radio = 0;
    ImGui::RadioButton("Option A", &radio, 0);
    ImGui::RadioButton("Option B", &radio, 1);

    static float value = 50.0f;
    ImGui::SliderFloat("Slider", &value, 0.0f, 100.0f);
    ImGui::DragFloat("Drag", &value, 0.5f, 0.0f, 100.0f);

    static char text[256] = {};
    ImGui::InputText("Name", text, sizeof(text));

    static int combo = 0;
    const char *options[] = {"First", "Second", "Third"};
    ImGui::Combo("Options", &combo, options, IM_ARRAYSIZE(options));

    static float color[4] = {1, 0, 0, 1};
    ImGui::ColorEdit4("Color", color);

    ImGui::ProgressBar(0.7f, ImVec2(250, 0));

    if (ImGui::CollapsingHeader("More")) {
      ImGui::Text("Extra content");
      ImGui::BulletText("Item one");
      ImGui::BulletText("Item two");
      ImGui::BulletText("Item three");
    }

    ImGui::End();

    this->m_MainInterface->NewRenderIMGUI();

    glfwSwapBuffers(this->m_Window);
    glfwPollEvents();
  }
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
