#pragma once
#ifndef __GLAD_GAURD__
#include <glad/glad.h>
#endif

class FrameBuffer {
public:
  // make FBO
  FrameBuffer(unsigned int, unsigned int);

  // resize FBO
  void ResizeFBO(unsigned int, unsigned int);

  // get the FBO unsigned int
  unsigned int ReturnFrameBuffer();

  // the Color Texture unsigned int
  unsigned int ReturnColorTexture();

  // bind and unbind the FBO
  void BindFrameBuffer();
  void UnBindFrameBuffer();

  // clean, like delete that shit
  void Clean();

  // public so that I can read these data
  unsigned int windowWidth, windowHeight;

private:
  // private variables,
  unsigned int m_FrameBuffer, m_ColorTexture, m_RenderBufferObject;

private:
};
