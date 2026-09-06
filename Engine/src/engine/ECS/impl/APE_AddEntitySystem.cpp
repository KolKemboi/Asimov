#include "APE_Components.hpp"
#include <APE_AddEntitySystem.hpp>
#include <cstdio>
#include <string>

void AddEntitySystem::_helperFunction(entt::registry &reg, const char *objName,
                                      unsigned int vao, unsigned int idxCount) {
  auto entity = reg.create();
  auto view = reg.view<Name, ObjectCount>();

  // the entity has not been created ---yet
  unsigned int ObjNumber = 1;
  unsigned int max_count = 0;

  // this is where the bug is
  for (auto [ent, name, count] : view.each()) {
    // name and count
    if (strcmp(name.s_Name.c_str(), objName) == 0) {
      if (count.s_Count > max_count)
        max_count = count.s_Count;
      ObjNumber = max_count + 1;
    }
    // default selection
    bool isSelected = reg.all_of<Selected>(ent);
    auto selectedView = reg.view<Selected>();
    for (auto [selectedEntity] : selectedView.each()) {
      reg.remove<Selected>(selectedEntity);
    }
  }
  printf("Object count %d\n", ObjNumber);

  reg.emplace<Name>(entity, objName);
  reg.emplace<Transform>(entity);
  reg.emplace<ObjectCount>(entity, ObjNumber);
  reg.emplace<Material>(entity, glm::vec3(0.5f), Type::MESH);
  reg.emplace<Renderable>(entity, vao, idxCount);
	reg.emplace<Selected>(entity);
}

void AddEntitySystem::AddCubeSystem(entt::registry &registry, unsigned int VAO,
                                    unsigned int count) {
  this->_helperFunction(registry, "Cube", VAO, count);
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
void AddEntitySystem::AddCapsuleSystem(entt::registry &registry,
                                       unsigned int VAO, unsigned int count) {
  this->_helperFunction(registry, "Capsule", VAO, count);
  printf("ADD_CAPSULE RUN\n");
}
void AddEntitySystem::AddConvexMeshSystem(entt::registry &registry,
                                          unsigned int VAO,
                                          unsigned int count) {
  this->_helperFunction(registry, "ConvexMesh", VAO, count);
  printf("ADD_CONVEXMESH RUN\n");
}
