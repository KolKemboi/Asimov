#version 460 core

out vec4 FragColor;
in vec3 normals;
in vec3 FragPos;
uniform vec3 color;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
  float ambientStrength = 0.8;
  float specularStrength = 0.5;
  vec3 ambient = ambientStrength * lightColor;

  vec3 norm = normalize(normals);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.2);
  vec3 diffuse = diff * lightColor;

  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float specular = pow(max(dot(viewDir, reflectDir), 0.0), 16);
  vec3 result = (ambient + diffuse) * color;

  // FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
  FragColor = vec4(result, 1.0f);
  // FragColor = vec4(normals, 1.0f);
}
