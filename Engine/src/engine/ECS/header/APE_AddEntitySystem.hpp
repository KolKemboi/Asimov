#pragma once
#include <APE_Dispatcher.hpp>
#include <APE_Components.hpp>

class AddEntitySystem {
public:
  AddEntitySystem(Dispatcher &dispatcher) : m_LocalDispatcher(dispatcher) {};
  void AddCubeSystem(entt::registry &, unsigned int, unsigned int);
  void AddSphereSystem(entt::registry &, unsigned int, unsigned int);
  void AddCylinderSystem(entt::registry &, unsigned int, unsigned int);
  void AddPlaneSystem(entt::registry &, unsigned int, unsigned int);
  void AddCapsuleSystem(entt::registry &, unsigned int, unsigned int);
  void AddConvexMeshSystem(entt::registry &, unsigned int, unsigned int);

private:
  Dispatcher &m_LocalDispatcher;
  void _helperFunction(entt::registry &, const char *, unsigned int,
                       unsigned int);
};
