#pragma once

#include "APE_FBO.hpp"
#include "APE_camera.hpp"
#include "APE_eventsystem.hxx"
#include <APE_Components.hpp>
#include <ImGuizmo.h>
#include <cstdio>
#include <entt/entt.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <memory>
#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Vector3.h>

class Viewport {
public:
  bool isComplete = false;
  ImGuizmo::MODE mode = ImGuizmo::WORLD;

  void View(std::unique_ptr<FrameBuffer> &framebuffer, Camera &camera,
            entt::registry &reg, std::shared_ptr<Shader> &shader,
            EventSystem &eventSystem) {
    ImGuizmo::BeginFrame();

    ImGui::Begin("Viewport");

    ImVec2 Pos = ImGui::GetWindowPos();
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

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    shader->SetMat4(projection, "projection");

    // Center the image in the viewport
    ImVec2 cursor = ImGui::GetCursorPos();

    ImGui::SetCursorPos(ImVec2(cursor.x + (avail.x - imguiWidth) * 0.5f,
                               cursor.y + (avail.y - imguiHeight) * 0.5f));

    ImGui::Image((ImTextureID)(intptr_t)framebuffer->ReturnColorTexture(),
                 avail, ImVec2(0, 1), ImVec2(1, 0));

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(Pos.x, Pos.y, avail.x, avail.y);

    static ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;

    for (auto key : eventSystem.m_KeysPressed) {
      if (key == KeyPress::W)
        operation = ImGuizmo::TRANSLATE;
      if (key == KeyPress::E)
        operation = ImGuizmo::ROTATE;
      if (key == KeyPress::R)
        operation = ImGuizmo::SCALE;
      if (key == KeyPress::G) {
        for (auto mod : eventSystem.m_ModKeys) {
          if (mod == ModKeys::SHIFT) {
            mode = ImGuizmo::WORLD;
          }
        }
      }
      if (key == KeyPress::L) {
        for (auto mod : eventSystem.m_ModKeys) {
          if (mod == ModKeys::SHIFT) {
            mode = ImGuizmo::LOCAL;
          }
        }
      }
      if (key == KeyPress::X) {
        for (auto mod : eventSystem.m_ModKeys) {
          if (mod == ModKeys::SHIFT) {
            operation = ImGuizmo::ROTATE_X;
          }
        }
      }
      if (key == KeyPress::Y) {
        for (auto mod : eventSystem.m_ModKeys) {
          if (mod == ModKeys::SHIFT) {
            operation = ImGuizmo::ROTATE_Y;
          }
        }
      }
      if (key == KeyPress::Z) {
        for (auto mod : eventSystem.m_ModKeys) {
          if (mod == ModKeys::SHIFT) {
            operation = ImGuizmo::ROTATE_Z;
          }
        }
      }
    }

    glm::mat4 view = camera.GetViewMatrix();

    auto selected = reg.view<Selected>();

    for (auto entity : selected) {
      auto &transform = reg.get<Transform>(entity);
      auto &name = reg.get<Name>(entity);
      auto &count = reg.get<ObjectCount>(entity);

      glm::mat4 model = transform.GetModelMatrix();

      ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
                           operation, mode, glm::value_ptr(model));
      if (ImGuizmo::IsUsing()) {

        float pos[3];
        float rot[3];

        float sca[3];
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(model), pos, rot,
                                              sca);

        transform.s_Position = glm::vec3(pos[0], pos[1], pos[2]);
        transform.s_Rotation = glm::vec3(rot[0], rot[1], rot[2]);
        transform.s_Scale = glm::vec3(sca[0], sca[1], sca[2]);

        isComplete = true;
      } else if (!ImGuizmo::IsUsing() && isComplete) {
        printf("Transformed! %s_%d\n", name.s_Name.c_str(), count.s_Count);

        printf("NEW POS=> %f %f %f\n", transform.s_Position.x,
               transform.s_Position.y, transform.s_Position.z);

        printf("NEW ROT=> %f %f %f\n", transform.s_Rotation.x,
               transform.s_Rotation.y, transform.s_Rotation.z);

        printf("NEW SCA=> %f %f %f\n", transform.s_Scale.x, transform.s_Scale.y,
               transform.s_Scale.z);

        isComplete = false;
      }
    }

    ImGui::End();
  }

private:
};
