#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <reactphysics3d/collision/shapes/BoxShape.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Transform.h>
#include <reactphysics3d/mathematics/Vector3.h>
#ifndef _GLAD_GUARD_
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <reactphysics3d/reactphysics3d.h>
#include <vector>

using namespace reactphysics3d;

unsigned int WIDTH = 1920;
unsigned int HEIGHT = 1080;
unsigned int PixelsPerMetre = 100;
GLFWwindow *window;

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTex;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 transform;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "	gl_Position = model * vec4(aPos, 1.0);\n"
    "	//gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "	// gl_Position = vec4(aPos, 1.0);\n"
    "	ourColor = aColor;\n"
    "	TexCoord = aTex;\n"
    "}\n";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main(){\n"
    "	// FragColor = vec4(TexCoord, 0.0, 1.0);\n"
    "	FragColor = vec4( ourColor,  1.0);\n"
    "	// FragColor = texture(ourTexture, TexCoord);\n"
    "}\n";

std::vector<float> verts = {
    // positions          // colors           // texture coords
    // Front face
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom-right
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left

    // Back face
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // bottom-left
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // top-left
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right

    // Left face
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-right
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom-right

    // Right face
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // top-left
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // bottom-left

    // Top face
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top-left
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // bottom-right
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // bottom-left

    // Bottom face
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top-right
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // top-left
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // bottom-left
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // bottom-right
};

std::vector<unsigned int> indices = {
    0,  1,  2,  2,  3,  0,  // Front face
    4,  5,  6,  6,  7,  4,  // Back face
    8,  9,  10, 10, 11, 8,  // Left face
    12, 13, 14, 14, 15, 12, // Right face
    16, 17, 18, 18, 19, 16, // Top face
    20, 21, 22, 22, 23, 20  // Bottom face
};
void closeWindow(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
};

int success;
char infoLog[512];

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(WIDTH, HEIGHT, "", NULL, NULL);
  glfwMakeContextCurrent(window);

  gladLoadGL();
  glViewport(0, 0, WIDTH, HEIGHT);

  PhysicsCommon physicsCommon;
  PhysicsWorld *world = physicsCommon.createPhysicsWorld();
  world->setGravity(Vector3(0, -0.81, 0));

  unsigned int vao, vbo, ibo;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(),
               GL_STATIC_DRAW);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  unsigned int vertex, fragment, shader;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexShaderSource, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "Vertex Failed" << infoLog << std::endl;
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "Fragment Failed" << infoLog << std::endl;
  }

  shader = glCreateProgram();
  glAttachShader(shader, vertex);
  glAttachShader(shader, fragment);
  glLinkProgram(shader);

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  float position[3] = {0.2f, 0.2f, 0.2f};
  float scale[3] = {0.2f, 0.2f, 0.2f};

  glUseProgram(shader);
  glm::mat4 model;
  model = glm::mat4(1.0f);
  model = glm::scale(model, glm::vec3(0.2));
  model = glm::translate(model, glm::vec3(0));
  unsigned int modelLoc = glGetUniformLocation(shader, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

  Vector3 startPosition(0, 0, 0);
  Quaternion startOrn = Quaternion::identity();
  Transform startTransform(startPosition, startOrn);
  RigidBody *body = world->createRigidBody(startTransform);

  Vector3 boxSize(0.2, 0.2, 0.2);
  BoxShape *boxShape = physicsCommon.createBoxShape(boxSize);
  Transform identityTransform = Transform::identity();
  body->addCollider(boxShape, identityTransform);
  body->setMass(1);

  const float timeStep = 1.0f / 60.0f;

  while (!glfwWindowShouldClose(window)) {
    closeWindow(window);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2, 0.1, 0.3, 1.0);

    world->update(timeStep);
    Transform t = body->getTransform();
    Vector3 pos = t.getPosition();
    Quaternion rot = t.getOrientation();

    glm::vec3 position(pos.x, pos.y, pos.z);
    std::cout << pos.y << std::endl;

    glUseProgram(shader);

    // model = glm::scale(model, glm::vec3(0.2));
    model = glm::translate(model, glm::vec3(position));
    unsigned int modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  glDeleteVertexArrays(1, &vao);
  glfwTerminate();
}
