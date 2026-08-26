#include <APE_window.hpp>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <imgui.h>
#include <memory>
#include <optional>

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
  this->m_MainShader = std::make_unique<Shader>(
      "shaders/default/vertex.glsl", "shaders/default/fragment.glsl");
  this->m_MainFrameBuffer =
      std::make_unique<FrameBuffer>(m_WindowWidth, m_WindowHeight);

  this->m_MainInterface = std::make_unique<Interface>(this->m_Window);
}

void APE_Window::_run() {
  this->m_MainShader->UseShader();
  while (!glfwWindowShouldClose(m_Window)) {
    // call the renderer and give it the frame buffer and a vector of objects
    // with the renderable component to render
    this->_miniInputSystem(this->m_Window);
    this->m_MainInterface->SetUpNewFrame();
    this->m_MainInterface->SetUpDocking();

		ImGui::Begin("ViewPort");
		ImGui::End();
		ImGui::Begin("Outliner");
		ImGui::End();
		ImGui::Begin("Properties");
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
