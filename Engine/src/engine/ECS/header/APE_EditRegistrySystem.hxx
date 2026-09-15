#pragma once
#include "APE_Components.hpp"
#include <entt/entt.hpp>
#include <reactphysics3d/engine/PhysicsCommon.h>

class RemoveEntitySystem {
public:
  // delete selected object
  void RemoveEntity(entt::registry &registry, 
                    rp3d::PhysicsWorld *&world) {
    // need to remove collider in the cache
    auto selectedView = registry.view<Selected>();
    for (auto entity : selectedView) {
      // removed the bug, make sure to flush the rigid body too from the world
      PhysicsBody fzxbody = registry.get<PhysicsBody>(entity);
      world->destroyRigidBody(fzxbody.s_Body);
      registry.destroy(entity);
    }
  }
};
