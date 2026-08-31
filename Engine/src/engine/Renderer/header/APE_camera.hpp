#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#ifndef __GLAD_GAURD__
#include <glad/glad.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 6.0f;
const float SENSITIVITY = 0.25f;
const float ZOOM = 45.0f;

class Camera {
public:
  Camera(glm::vec3 , glm::vec3 );

  glm::mat4 GetViewMatrix();

private:
  glm::vec3 m_Position, m_Front, m_Up, m_Target;

private:
};
