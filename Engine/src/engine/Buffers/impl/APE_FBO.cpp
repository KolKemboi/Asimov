#include <APE_FBO.hpp>
#include <cstdio>

FrameBuffer::FrameBuffer(unsigned int Width, unsigned int Height)
    : windowWidth(Width), windowHeight(Height) {

  glGenFramebuffers(1, &m_FrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

  glGenTextures(1, &m_ColorTexture);
  glBindTexture(GL_TEXTURE_2D, m_ColorTexture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)windowWidth,
               (GLsizei)windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_ColorTexture, 0);

  glGenRenderbuffers(1, &m_RenderBufferObject);
  glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferObject);

  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                        (GLsizei)windowWidth, (GLsizei)windowHeight);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, m_RenderBufferObject);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    printf("ERROR::FRAMEBUFFER IS NOT COMPLETE\n");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// returns the color texture, for Imgui presentation
unsigned int FrameBuffer::ReturnColorTexture() { return this->m_ColorTexture; }

// binds the FBO
void FrameBuffer::BindFrameBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}
// unbinds the FBO
void FrameBuffer::UnBindFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

// cleans the FBO and reports it has been cleaned
void FrameBuffer::Clean() {
  glDeleteFramebuffers(1, &m_FrameBuffer);
  glDeleteTextures(1, &m_ColorTexture);
  glDeleteRenderbuffers(1, &m_RenderBufferObject);
  printf("FBO::CLEANED\n");
}
