#include "APE_Components.hpp"
#include "APE_Dispatcher.hpp"
#include <APE_AddEntitySystem.hpp>
#include <cstdio>
#include <cstring>
#include <string>

void AddEntitySystem::_helperFunction(entt::registry &reg, const char *objName,
                                      unsigned int vao, unsigned int idxCount) {

  // create a new entity
  auto entity = reg.create();
  // grab names and Object count component
  auto view = reg.view<Name, ObjectCount>();

  // the entity has not been created ---yet
  unsigned int ObjNumber = 1;
  unsigned int max_count = 0;

  // this is where the bug is -> solved the bug
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

  reg.emplace<Name>(entity, objName);          // set name
  reg.emplace<Transform>(entity);              // set default transformation
  reg.emplace<ObjectCount>(entity, ObjNumber); // add the object count
  reg.emplace<Material>(entity, glm::vec3(0.5f),
                        Type::MESH); // it defaults to mesh,
  reg.emplace<Renderable>(entity, vao,
                          idxCount); // add the renderable components

  // set the  shape type for collider selection
  if (strcmp(objName, "Cube") == 0)
    reg.emplace<Shape>(entity, Shape::BOX);
  if (strcmp(objName, "Sphere") == 0)
    reg.emplace<Shape>(entity, Shape::SPHERE);
  if (strcmp(objName, "Cylinder") == 0)
    reg.emplace<Shape>(entity, Shape::CYLINDER); // not defined yet, will crash
                                                 // with a weird error message
  if (strcmp(objName, "Capsule") == 0)
    reg.emplace<Shape>(entity, Shape::CAPSULE);
  if (strcmp(objName, "ConvexMesh") == 0)
    reg.emplace<Shape>(entity, Shape::CONVEXMESH); // will use a sphere type

  reg.emplace<Selected>(entity);
}

void AddEntitySystem::AddCubeSystem(entt::registry &registry, unsigned int VAO,
                                    unsigned int count) {
  this->_helperFunction(registry, "Cube", VAO, count); // add cube
  m_LocalDispatcher.Emitter(EventType::OBJECT_ADDED, "Cube has been added");
}
void AddEntitySystem::AddSphereSystem(entt::registry &registry,
                                      unsigned int VAO, unsigned int count) {
  this->_helperFunction(registry, "Sphere", VAO, count);
  m_LocalDispatcher.Emitter(EventType::OBJECT_ADDED, "Sphere has been added");
}
void AddEntitySystem::AddCylinderSystem(entt::registry &registry,
                                        unsigned int VAO, unsigned int count) {
  this->_helperFunction(registry, "Cylinder", VAO, count);
}
void AddEntitySystem::AddPlaneSystem(entt::registry &registry, unsigned int VAO,
                                     unsigned int count) {
  this->_helperFunction(registry, "Plane", VAO, count);
}
void AddEntitySystem::AddCapsuleSystem(entt::registry &registry,
                                       unsigned int VAO, unsigned int count) {
  this->_helperFunction(registry, "Capsule", VAO, count);
  m_LocalDispatcher.Emitter(EventType::OBJECT_ADDED, "Capsule has been added");
}
void AddEntitySystem::AddConvexMeshSystem(entt::registry &registry,
                                          unsigned int VAO,
                                          unsigned int count) {
  this->_helperFunction(registry, "ConvexMesh", VAO, count);
  m_LocalDispatcher.Emitter(EventType::OBJECT_ADDED, "Convex has been added");
}
