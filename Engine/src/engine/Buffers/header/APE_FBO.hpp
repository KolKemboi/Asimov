#pragma once
#ifndef __GLAD_GAURD__
#include <glad/glad.h>
#endif

class FrameBuffer {
public:
  FrameBuffer(unsigned int, unsigned int);

  unsigned int ReturnFrameBuffer();

	unsigned int ReturnColorTexture();

  void BindFrameBuffer();
  void UnBindFrameBuffer();

  void Clean();

	unsigned int windowWidth, windowHeight;
private:
  unsigned int m_FrameBuffer, m_ColorTexture, m_RenderBufferObject;

private:
};
