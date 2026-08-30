#pragma once
#include <APE_Components.hpp>

class AddEntitySystem {
public:
  void AddCubeSystem(entt::registry &, unsigned int, unsigned int);
  void AddSphereSystem(entt::registry &, unsigned int, unsigned int);
  void AddCylinderSystem(entt::registry &, unsigned int, unsigned int);
  void AddPlaneSystem(entt::registry &, unsigned int, unsigned int);

private:
  void _helperFunction(entt::registry &, const char *,
                       unsigned int , unsigned int );
};
