#ifndef _GLAD_GUARD_
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

unsigned int WIDTH = 960;
unsigned int HEIGHT = 1080;
GLFWwindow *window;

void closeWindow(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
};

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(WIDTH, HEIGHT, "", NULL, NULL);
  glfwMakeContextCurrent(window);

  gladLoadGL();
  glViewport(0, 0, WIDTH, HEIGHT);

  while (!glfwWindowShouldClose(window)) {
    closeWindow(window);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2, 0.1, 0.3, 1.0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
}
