#include "APE_Components.hpp"
#include <APE_AddEntitySystem.hpp>
#include <cstdio>

void AddEntitySystem::_helperFunction(entt::registry &reg, const char *objName,
                                      unsigned int vao, unsigned int count) {
  auto entity = reg.create();
  auto view = reg.view<Name, ObjectCount>();

  // the entity has not been created ---yet
  unsigned int ObjNumber = 1;

  for (auto [ent, name, count] : view.each()) {
    if (strcmp(name.s_Name.c_str(), objName) == 0) {
      ObjNumber = count.s_Count + 1;
      printf("Object count %d\n", ObjNumber);
    }
  }

  reg.emplace<Name>(entity, objName);
  reg.emplace<Transform>(entity);
  reg.emplace<ObjectCount>(entity, ObjNumber);
  reg.emplace<Material>(entity);
  reg.emplace<Renderable>(entity, vao, count);
}

void AddEntitySystem::AddCubeSystem(entt::registry &registry, unsigned int VAO,
                                    unsigned int count) {
  this->_helperFunction(registry, "Cube", VAO, count);
  // add 1 to the count
  printf("ADD_CUBE RUN\n");
}
void AddEntitySystem::AddSphereSystem(entt::registry &registry,
                                      unsigned int VAO, unsigned int count) {
  this->_helperFunction(registry, "Sphere", VAO, count);
  printf("ADD_SPHERE RUN\n");
}
void AddEntitySystem::AddCylinderSystem(entt::registry &registry,
                                        unsigned int VAO, unsigned int count) {
  this->_helperFunction(registry, "Cylinder", VAO, count);
  printf("ADD_CYLINDER RUN\n");
}
void AddEntitySystem::AddPlaneSystem(entt::registry &registry, unsigned int VAO,
                                     unsigned int count) {
  this->_helperFunction(registry, "Plane", VAO, count);
  printf("ADD_PLANE RUN\n");
}
