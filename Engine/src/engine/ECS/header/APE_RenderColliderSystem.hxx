
#pragma once
#include <memory>
#ifndef __GLAD_GUARD__
#include <glad/glad.h>
#endif
#include <APE_Components.hpp>
#include <APE_FBO.hpp>
#include <APE_shader.hpp>
#include <entt/entt.hpp>

class RenderColliderSystem {
public:
  struct RenderRequirement {
    unsigned int s_IndexCount;
    Material s_Material;
    Transform s_Transform;
    explicit RenderRequirement(unsigned int indexCount, Material material,
                               Transform transform)
        : s_IndexCount(indexCount), s_Material(material),
          s_Transform(transform) {};
  };

  void RenderColliders(std::unique_ptr<FrameBuffer> &frameBuffer,
                       entt::registry &registry,
                       std::shared_ptr<Shader> &shader) {

    auto view = registry.view<Renderable, Material, PhysicsData>();

    /*
     * OPTIMIZATION
     * Std::unordered map<unsigned int, std::vector<entt>>
     * first entity, grab vao, make it a key, push to vector
     * second, check VAO, not same, new key push to new vector
     * 	-> push to first key
     */
    // this stupid logic took me 2 hrs to write
    std::unordered_map<unsigned int, std::vector<RenderRequirement>> batches;

    for (auto [entity, renderable, material, data] : view.each()) {
      Transform transform;

      transform.s_Position.x = data.s_Position_C.x;
      transform.s_Position.y = data.s_Position_C.y;
      transform.s_Position.z = data.s_Position_C.z;

      transform.s_Rotation.x = data.s_Rotation_C.x;
      transform.s_Rotation.y = data.s_Rotation_C.y;
      transform.s_Rotation.z = data.s_Rotation_C.z;

      transform.s_Scale.x = data.s_Scale_C.x + 0.1;
      transform.s_Scale.y = data.s_Scale_C.y + 0.1;
      transform.s_Scale.z = data.s_Scale_C.z + 0.1;

      material.s_Color = glm::vec3(1.0f, 0.0f, 0.0f);
      // make a req and fill with the required data
      RenderRequirement req =
          RenderRequirement{renderable.s_IndexCount, material, transform};

      // grab the VAO-> this is the key
      unsigned int VAO = renderable.s_VAO;

      // add to the group, initial set up
      if (batches.size() == 0) {
        std::vector<RenderRequirement> firstReq;
        firstReq.push_back(req);
        batches[VAO] = firstReq;

      } else {
        if (batches.find(VAO) == batches.end()) {
          // creates a new key-value pair
          std::vector<RenderRequirement> firstReq;
          firstReq.push_back(req);
          batches[VAO] = firstReq;
        } else {
          // adds to an existing key-value pair
          batches.at(VAO).push_back(req);
        }
      }
    }

    // render loop
    frameBuffer->BindFrameBuffer();
    glClearColor(0.2, 0.1, 0.3, 1.0f); // background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* On render cycle
     * grab the VAO from the std::unordered map, bind,
     * render the std::vector
     * repeat
     */
    for (auto batch : batches) {
      // bind VAO
      glBindVertexArray(batch.first);
      for (auto renderReq : batch.second) {
        // apply model transforms
        shader->SetMat4(renderReq.s_Transform.GetModelMatrix(), "model");
        shader->SetVec3(renderReq.s_Material.s_Color, "color");
        glDrawElements(GL_LINES, renderReq.s_IndexCount, GL_UNSIGNED_INT, 0);
      }
    }

    frameBuffer->UnBindFrameBuffer();
    batches.clear();
  }
};
