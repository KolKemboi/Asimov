#include "APE_FBO.hpp"
#include <APE_ECS.hpp>
#include <algorithm>
#include <memory>

// std::unique_ptr<FrameBuffer>
// RenderSystem::RenderScene(std::unique_ptr<FrameBuffer> framebuffer,
//                           std::unique_ptr<Engine> renderer,
//                           entt::registry &registry) {
//
//   framebuffer->BindFrameBuffer();
//
//   // RenderBackground
//
//   auto view = registry.view<Transform, Renderable>();
//   // for (auto [entity, transform, renderable] : view.each()) {
//   // Render Entity
//   // }
//   framebuffer->UnBindFrameBuffer();
//
//   return framebuffer;
// }
//
// void AddEntitySystem::AddCube(entt::registry &registry) {
//   auto entity = registry.create();
//   // call some function "Add Cube mesh and Idx count"
//   registry.emplace<Name>(entity, "Cube_1");
//   registry.emplace<Transform>(entity);
//   registry.emplace<Material>(entity);
//   registry.emplace<Renderable>(entity);
//   // auto view = registry.view<Name>();
// }
