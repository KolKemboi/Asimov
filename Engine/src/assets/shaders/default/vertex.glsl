#version 460 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 Normals;
layout(location = 2) in vec2 TextureCoordinate;

uniform float angleX;
uniform float angleY;
uniform float angleZ;

void main()
{
  vec3 p = VertexPosition;

  // -------------------------
  // Rotate around X axis
  // -------------------------
  float cosX = cos(10.0);
  float sinX = sin(10.0);

  p = vec3(
      p.x,
      p.y * cosX - p.z * sinX,
      p.y * sinX + p.z * cosX
    );

  // -------------------------
  // Rotate around Y axis
  // -------------------------
  float cosY = cos(10.0f);
  float sinY = sin(10.0f);

  p = vec3(
      p.x * cosY + p.z * sinY,
      p.y,
      -p.x * sinY + p.z * cosY
    );

  // -------------------------
  // Rotate around Z axis
  // -------------------------
  float cosZ = cos(10.0f);
  float sinZ = sin(10.0f);

  p = vec3(
      p.x * cosZ - p.y * sinZ,
      p.x * sinZ + p.y * cosZ,
      p.z
    );

  gl_Position = vec4(p, 1.0);
}
