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
  void SetUpCamera(glm::vec3, glm::vec3, float, float);

  glm::mat4 GetViewMatrix();
  void ProcessOrbit(float, float);
  void ProcessPan(float, float);
  void ProcessDolly(float);
  void SetInitialState(const glm::vec3 &, const glm::vec3 &, float, float);
  void ResetViewSmooth(float);
  void ResetViewToOrigin(float);
  void StartResetSmooth() { m_IsResetting = true; }

  float GetZoom() const { return m_Zoom; }
  glm::vec3 GetPosition() const { return m_Position; }
  glm::vec3 GetTarget() const { return m_Target; }
  void SetTarget(const glm::vec3 &target) { m_Target = target; }

private:
  glm::vec3 m_Position, m_Front, m_Up, m_Target, m_Right, m_WorldUp;
  float m_Yaw, m_Pitch;
  float m_MvtSpeed, m_MouseSens, m_Zoom;

  glm::vec3 m_InitialPosition, m_InitialTarget;
  float m_InitialYaw, m_InitialPitch;
  bool m_IsResetting = false;

private:
  void _updateCameVectors();
};
