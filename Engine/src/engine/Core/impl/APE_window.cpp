#include "APE_ECS.hpp"
#include "APE_FBO.hpp"
#include <APE_window.hpp>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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

  unsigned int start = 0;
  unsigned int offset = 0;

	//SHIT I ALSO NEED TO SHIFT THE INDICES BY THE MAX OF THE PREV ONE
  for (auto &object : this->m_ModelLoaderHelper->m_Objects) {
    std::string name = object.s_Name;
    for (auto vert : object.s_Vertices) {
      float posx = vert.s_Position.x;
      float posy = vert.s_Position.y;
      float posz = vert.s_Position.z;
      this->m_MainVertexBuffer.push_back(posx);
      this->m_MainVertexBuffer.push_back(posy);
      this->m_MainVertexBuffer.push_back(posz);

      float normx = vert.s_Normal.x;
      float normy = vert.s_Normal.y;
      float normz = vert.s_Normal.z;
      this->m_MainVertexBuffer.push_back(normx);
      this->m_MainVertexBuffer.push_back(normy);
      this->m_MainVertexBuffer.push_back(normz);

      float texx = vert.s_TexCoords.x;
      float texy = vert.s_TexCoords.y;
      this->m_MainVertexBuffer.push_back(texx);
      this->m_MainVertexBuffer.push_back(texy);
    }
    for (auto &idx : object.s_Indices) {
      this->m_MainIndexBuffer.push_back(idx);
    }
    start = offset;
    offset = this->m_MainIndexBuffer.size();

    this->m_MainModelMapping[name] =
        std::make_pair(start, this->m_MainIndexBuffer.size());
  }

  /// DEBUGGGG
  // for (auto &map : this->m_MainModelMapping) {
  //   printf("%s\n", map.first.c_str());
  //   printf("%d\n", std::get<0>(map.second));
  //   printf("%d\n", std::get<1>(map.second));
  // }

  this->m_MainVAO = std::make_unique<VertexArray>();
  this->m_MainVAO->BindVertexArray();
  this->m_MainVBO = std::make_unique<VertexBuffer>();
  this->m_MainVBO->GenVertexBuffers(m_MainVertexBuffer,
                                    sizeof(m_MainVertexBuffer));

  this->m_MainIBO = std::make_unique<IndexBuffer>();
  this->m_MainIBO->GenIndexBuffers(m_MainIndexBuffer,
                                   sizeof(m_MainIndexBuffer));
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

    // auto view = m_Registry.view<Name, Transform, Material>();
    // for (auto [entity, name, transform, material] : view.each()) {
    //   printf("%s\n", name.s_Name.c_str());
    //   printf("pos.x %f pos.y %f pos.z %f \n", transform.s_Position.x,
    //          transform.s_Position.y, transform.s_Position.z);
    //   printf("mat.col.r %f mat.col.g %f mat.col.b %f \n", material.s_Color.r,
    //          material.s_Color.g, material.s_Color.b);
    // }

    auto renderView = m_Registry.view<Renderable>();

    this->m_MainFrameBuffer->BindFrameBuffer();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2, 0.2, 0.3f, 1.0f);
    for (auto [entity, renderable] : renderView.each()) {
      printf("%d\n", renderable.s_IndexCount);
      printf("%d\n", renderable.s_StartIndex);

      glDrawElements(GL_TRIANGLES, renderable.s_IndexCount, GL_UNSIGNED_INT,
                     (void *)(renderable.s_StartIndex * sizeof(unsigned int)));
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

      // ImGui::SameLine();

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
