#include <APE_camera.hpp>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/trigonometric.hpp>

void Camera::SetUpCamera(glm::vec3 position, glm::vec3 up, float yaw = YAW,
                         float pitch = PITCH) {
  this->m_Position = position;
  this->m_WorldUp = up;
  this->m_Yaw = yaw;
  this->m_Target = glm::vec3(0.0f);
  this->m_Pitch = pitch;
  this->m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
  this->m_MvtSpeed = SPEED;
  this->m_MouseSens = SENSITIVITY;
  this->_updateCameVectors();

  // for resetting purposes
  this->m_InitialPosition = position;
  this->m_InitialTarget = glm::vec3(0.0f);
  this->m_InitialYaw = yaw;
  this->m_InitialPitch = pitch;
};

glm::mat4 Camera::GetViewMatrix() {
  // printf("%f %f %f\n", m_Position.x, m_Position.y, m_Position.z);
  return glm::lookAt(m_Position, m_Target, m_Up);
};

void Camera::ProcessOrbit(float xOffset, float yOffset) {
  xOffset *= m_MouseSens;
  yOffset *= m_MouseSens;

  m_Yaw += xOffset;
  m_Pitch += yOffset;

  if (m_Pitch > 89.0f)
    this->m_Pitch = 89.0f;
  if (m_Pitch < -89.0f)
    this->m_Pitch = -89.0f;

  this->_updateCameVectors();

  float distance = glm::distance(m_Position, m_Target);
  m_Position = m_Target - m_Front * distance;
}
void Camera::ProcessPan(float xOffset, float yOffset) {
  float panSpeed = 0.005f;

  glm::vec3 panRight = -m_Right * (xOffset * panSpeed);
  glm::vec3 panUp = m_Up * (-yOffset * panSpeed);

  m_Position += panRight + panUp;
  m_Target += panRight + panUp;
}
void Camera::ProcessDolly(float yOffset) {

  float dollySpeed = 0.5f;

  glm::vec3 dir = glm::normalize(m_Target - m_Position);
  float distance = glm::distance(m_Position, m_Target);

  float newDistance = distance - (yOffset * dollySpeed);
  if (newDistance < 0.1f)
    newDistance = 0.1f;

  // printf("Poss %f %f %f\n", m_Position.x, m_Position.y, m_Position.z);
  m_Position = m_Target - dir * newDistance;
}
void Camera::SetInitialState(const glm::vec3 &pos, const glm::vec3 &target,
                             float yaw = YAW, float pitch = PITCH) {
  m_InitialPosition = pos;
  m_InitialTarget = target;
  m_InitialYaw = yaw;
  m_InitialPitch = pitch;
}
void Camera::ResetViewSmooth(float deltaTime) {
  if (m_IsResetting) {
    float speed = 2.0f;
    float t = glm::clamp(deltaTime * speed, 0.0f, 1.0f);

    m_Position = glm::lerp(m_Position, m_InitialPosition, t);
    m_Target   = glm::lerp(m_Target, m_InitialTarget, t);
    m_Yaw      = glm::lerp(m_Yaw, m_InitialYaw, t);
    m_Pitch    = glm::lerp(m_Pitch, m_InitialPitch, t);

    _updateCameVectors();

    if (glm::distance(m_Position, m_InitialPosition) < 0.5f) {
      m_Position = m_InitialPosition;
      m_Target = m_InitialTarget;
      m_Yaw = m_InitialYaw;
      m_Pitch = m_InitialPitch;
      m_IsResetting = false;

      _updateCameVectors();
    }
  }
}
void Camera::ResetViewToOrigin(float distance = 5.0f) {
  m_Target = glm::vec3(0.0f);
  m_Position = glm::vec3(0.0f, 0.0f, distance);
  m_Yaw = -90.0f;
  m_Pitch = 0.0f;
  _updateCameVectors();
}

void Camera::_updateCameVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  front.y = sin(glm::radians(m_Pitch));
  front.z = -sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

  m_Front = glm::normalize(front);
  m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
  m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
