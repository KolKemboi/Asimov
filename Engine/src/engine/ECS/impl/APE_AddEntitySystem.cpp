#include "APE_Components.hpp"
#include <APE_AddEntitySystem.hpp>

void AddEntitySystem::AddCubeSystem(entt::registry &registry,
                                    unsigned int start, unsigned int count) {
  auto entity = registry.create();
  registry.emplace<Name>(entity, "Cube");
  registry.emplace<Transform>(entity);
  registry.emplace<Material>(entity);
  registry.emplace<Renderable>(entity, start, count);
}
void AddEntitySystem::AddSphereSystem(entt::registry &registry,
                                      unsigned int start, unsigned int count) {
  auto entity = registry.create();
  registry.emplace<Name>(entity, "Sphere");
  registry.emplace<Transform>(entity);
  registry.emplace<Material>(entity);
  registry.emplace<Renderable>(entity, start, count);
}
void AddEntitySystem::AddCylinderSystem(entt::registry &registry,
                                        unsigned int start,
                                        unsigned int count) {
  auto entity = registry.create();
  registry.emplace<Name>(entity, "Cylinder");
  registry.emplace<Transform>(entity);
  registry.emplace<Material>(entity);
  registry.emplace<Renderable>(entity, start, count);
}
void AddEntitySystem::AddPlaneSystem(entt::registry &registry,
                                     unsigned int start, unsigned int count) {
  auto entity = registry.create();
  registry.emplace<Name>(entity, "Plane");
  registry.emplace<Transform>(entity);
  registry.emplace<Material>(entity);
  registry.emplace<Renderable>(entity, start, count);
}
