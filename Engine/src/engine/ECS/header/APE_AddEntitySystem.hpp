#pragma once
#include <APE_Components.hpp>
#include <APE_Dispatcher.hpp>

class AddEntitySystem {
public:
  AddEntitySystem(Dispatcher &dispatcher) : m_LocalDispatcher(dispatcher) {};
  // add cube and set collider
  void AddCubeSystem(entt::registry &, unsigned int, unsigned int);
  // add sphere and set collider
  void AddSphereSystem(entt::registry &, unsigned int, unsigned int);
  // add cylinder, no collider exists for it
  void AddCylinderSystem(entt::registry &, unsigned int, unsigned int);
  // plane
  void AddPlaneSystem(entt::registry &, unsigned int, unsigned int);
  // add cube and its collider
  void AddCapsuleSystem(entt::registry &, unsigned int, unsigned int);
  // add collider, then add a sphere collider
  void AddConvexMeshSystem(entt::registry &, unsigned int, unsigned int);

private:
  Dispatcher &m_LocalDispatcher;
  void _helperFunction(entt::registry &, const char *, unsigned int,
                       unsigned int);
};
