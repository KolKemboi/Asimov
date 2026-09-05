#pragma once

#include "APE_FBO.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <memory>

class Viewport {
public:
  void View(std::unique_ptr<FrameBuffer> &framebuffer, glm::mat4 &projection) {
    ImGui::Begin("Viewport");

    ImVec2 avail = ImGui::GetContentRegionAvail();

    unsigned int imguiWidth = (unsigned int)avail.x;
    unsigned int imguiHeight = (unsigned int)avail.y;

    if (imguiWidth != framebuffer->windowWidth ||
        imguiHeight != framebuffer->windowHeight) {
      // resize framebuffer
      framebuffer->Clean();
      framebuffer->ResizeFBO(imguiWidth, imguiHeight);
    }

    glViewport(0, 0, framebuffer->windowWidth, framebuffer->windowHeight);
    const float aspect = (float)imguiWidth / (float)imguiHeight;

    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    // Center the image in the viewport
    ImVec2 cursor = ImGui::GetCursorPos();

    ImGui::SetCursorPos(ImVec2(cursor.x + (avail.x - imguiWidth) * 0.5f,
                               cursor.y + (avail.y - imguiHeight) * 0.5f));

    ImGui::Image((ImTextureID)(intptr_t)framebuffer->ReturnColorTexture(),
                 avail, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
  }

private:
};
