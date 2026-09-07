#pragma once
#include <APE_Components.hpp>
#include <cstdio>
#include <entt/entt.hpp>
#include <reactphysics3d/components/RigidBodyComponents.h>
#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Transform.h>
#include <reactphysics3d/mathematics/Vector3.h>
#include <reactphysics3d/reactphysics3d.h>
#include <reactphysics3d/systems/BroadPhaseSystem.h>

namespace rp3d = reactphysics3d;

class AddColliderSystem {
public:
  void AddCollider(rp3d::PhysicsWorld *&world,
                   rp3d::PhysicsCommon &physicsCommon,
                   entt::registry &registry) {

    rp3d::Collider *bodyCollider;
    PhysicsData physics;

    auto selectedView = registry.view<Selected>();
    // auto bodyShape = registry.view<Shape>();

    for (entt::entity entity : selectedView) {
      printf("Added Collider\n");

      auto &t = registry.get<Transform>(entity);
      auto &shape = registry.get<Shape>(entity);

      physics.s_Position_C =
          rp3d::Vector3(t.s_Position.x, t.s_Position.y, t.s_Position.z);
      physics.s_Rotation_C = rp3d::Quaternion::fromEulerAngles(
          t.s_Rotation.x, t.s_Rotation.y, t.s_Rotation.z);

      physics.s_Scale_C = rp3d::Vector3(t.s_Scale.x, t.s_Scale.y, t.s_Scale.z);

      physics.s_BodyType = rp3d::BodyType::DYNAMIC;
      physics.s_Mass = 1.0;

      rp3d::Transform transform(physics.s_Position_C, physics.s_Rotation_C);

      PhysicsBody fzxbody;
      fzxbody.s_Body = world->createRigidBody(transform);
      fzxbody.s_Body->setType(physics.s_BodyType);

      if (shape == Shape::BOX) {
        physics.s_BoxShape = physicsCommon.createBoxShape(physics.s_Scale_C);
        bodyCollider = fzxbody.s_Body->addCollider(physics.s_BoxShape,
                                                   rp3d::Transform::identity());
      }

      if (shape == Shape::SPHERE) {
        physics.s_SphereShape =
            physicsCommon.createSphereShape(physics.s_Scale_C.x);
        bodyCollider = fzxbody.s_Body->addCollider(physics.s_SphereShape,
                                                   rp3d::Transform::identity());
      }

      fzxbody.s_Body->updateMassFromColliders();
      fzxbody.s_Body->setMass(physics.s_Mass);

      registry.emplace<PhysicsData>(entity, physics);
      registry.emplace<PhysicsBody>(entity, fzxbody);
    }
  }
};
