#version 460 core

out vec4 FragColor;
in vec3 normals;

void main() {
  // FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
  FragColor = vec4(normals, 1.0f);
}
