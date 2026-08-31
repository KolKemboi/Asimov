#version 460 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 Normals;
layout(location = 2) in vec2 TextureCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  // gl_Position = vec4(VertexPosition, 1.0f);
  gl_Position = projection * view * vec4(VertexPosition, 1.0f);
  // gl_Position = projection * view * model * vec4(VertexPosition, 1.0f);
}
