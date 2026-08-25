#pragma once
#ifndef __GLAD_GUARD__
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

class VertexArray {
public:
  VertexArray();
	void GenVertexArrays();
  void AttribPointerSetUp();
  unsigned int GetVAO();
	void BindVertexArray();
  void Clean();

private:
  unsigned int m_VertexArray;
};
