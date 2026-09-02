#include <APE_camera.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up)
    : m_Position(position), m_Up(up) {
  m_Target = glm::vec3(0.0f);
};

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(m_Position, m_Target, m_Up);
};
