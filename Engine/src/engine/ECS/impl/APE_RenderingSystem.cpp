#include "APE_FBO.hpp"
#include <APE_Components.hpp>
#include <APE_RenderingSystem.hpp>
#include <cstdio>
#include <memory>
#include <unordered_map>
#include <vector>

/*
 * what do I need to render
 * transform
 * VAO
 * index count
 * material
 */
struct RenderRequirement {
  unsigned int s_IndexCount;
  Material s_Material;
  Transform s_Transform;
  explicit RenderRequirement(unsigned int indexCount, Material material,
                             Transform transform)
      : s_IndexCount(indexCount), s_Material(material),
        s_Transform(transform) {};
};

void RenderSystem::RenderEntities(std::unique_ptr<FrameBuffer> &frameBuffer,
                                  entt::registry &registry) {

  auto view = registry.view<Renderable, Material, Transform>();

  /*
   * OPTIMIZATION
   * Std::unordered map<unsigned int, std::vector<entt>>
   * first entity, grab vao, make it a key, push to vector
   * second, check VAO, not same, new key push to new vector
   * 	-> push to first key
   */
  // this stupid logic took me 2 hrs to write
  std::unordered_map<unsigned int, std::vector<RenderRequirement>> batches;

  for (auto [entity, renderable, material, transform] : view.each()) {

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
  glClear(GL_COLOR_BUFFER_BIT);

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
      glDrawElements(GL_TRIANGLES, renderReq.s_IndexCount, GL_UNSIGNED_INT, 0);
    }
  }

  frameBuffer->UnBindFrameBuffer();
  batches.clear();
}
