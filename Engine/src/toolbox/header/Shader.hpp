#ifndef _GLAD_GUARD_
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
  Shader(const char *, const char *);
  void ShaderCleanup();

  unsigned int GetShaderProgram() const;
  void UseShaderProgram() const;

  void SetMat4(const char *, glm::mat4 &) const;
  void SetVec3(const char *, glm::vec3 &) const;
  void SetFloat(const char *, float &) const;

private:
  unsigned int m_ShaderProgram, m_Vertex, m_Fragment;

private:
  void _checkShaderValidity(unsigned int, const char *);
};
