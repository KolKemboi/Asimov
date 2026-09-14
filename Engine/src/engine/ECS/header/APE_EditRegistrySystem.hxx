#pragma once
#include "APE_Components.hpp"
#include <entt/entt.hpp>

class RemoveEntitySystem {
public:
  // delete selected object
  void RemoveEntity(entt::registry &registry) {
    auto selectedView = registry.view<Selected>();
    for (auto entity : selectedView) {
      registry.destroy(entity);
    }
  }
};
