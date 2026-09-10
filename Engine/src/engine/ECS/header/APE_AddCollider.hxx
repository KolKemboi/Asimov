#pragma once
#include "APE_Dispatcher.hpp"
#include <APE_Components.hpp>
#include <cstdio>
#include <entt/entt.hpp>
#include <glm/trigonometric.hpp>
#include <reactphysics3d/components/RigidBodyComponents.h>
#include <reactphysics3d/engine/PhysicsCommon.h>
#include <reactphysics3d/engine/PhysicsWorld.h>
#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Transform.h>
#include <reactphysics3d/mathematics/Vector3.h>
#include <reactphysics3d/reactphysics3d.h>
#include <reactphysics3d/systems/BroadPhaseSystem.h>

namespace rp3d = reactphysics3d;

class AddColliderSystem {
public:
  void SetColliderRequirements(rp3d::PhysicsWorld *&world,
                               rp3d::PhysicsCommon &phyCom,
                               entt::registry &registry) {
    m_LocalWorld = &world;
    m_LocalPhysicsCommon = &phyCom;
    m_LocalRegistry = &registry;
  }

  void SetDispatcher(Dispatcher &dispatcher) {
    dispatcher.Subscriber(EventType::OBJECT_ADDED,
                          [this](const std::string &data) {
                            printf("%s\n", data.c_str());
                            AddCollider();
                          });

    dispatcher.Subscriber(
        EventType::OBJECT_MODIFIED, [this](const std::string &data) {
          // delete phys data and phys body
          // also check the selected
          auto selectedView = m_LocalRegistry->view<Selected>();
          for (entt::entity entity : selectedView) {
            // auto data = m_LocalRegistry->get<typename Type>(const entity_type
            // entt)
            auto body = m_LocalRegistry->get<PhysicsBody>(entity);
            rp3d::PhysicsWorld *newWorld = *m_LocalWorld;
            body.s_Body->removeCollider(body.s_Collider);
            newWorld->destroyRigidBody(body.s_Body);

            m_LocalRegistry->remove<PhysicsData>(entity);
            m_LocalRegistry->remove<PhysicsBody>(entity);
            AddCollider();
            // printf("Collider Added\n");
          }
        });
  }

private:
  rp3d::PhysicsWorld **m_LocalWorld;
  rp3d::PhysicsCommon *m_LocalPhysicsCommon;
  entt::registry *m_LocalRegistry;

public:
  void AddCollider() {

    rp3d::Collider *bodyCollider;
    PhysicsData physics;

    auto selectedView = m_LocalRegistry->view<Selected>();

    for (entt::entity entity : selectedView) {

      auto &t = m_LocalRegistry->get<Transform>(entity);

      auto &shape = m_LocalRegistry->get<Shape>(entity);

      physics.s_Position_C =
          rp3d::Vector3(t.s_Position.x, t.s_Position.y, t.s_Position.z);

      physics.s_Rotation_C = rp3d::Quaternion::fromEulerAngles(
          glm::radians( t.s_Rotation.x ), glm::radians( t.s_Rotation.y ), glm::radians( t.s_Rotation.z ));

      physics.s_Mass = 1.0;

      rp3d::Transform transform(physics.s_Position_C, physics.s_Rotation_C);

      PhysicsBody fzxbody;
      rp3d::PhysicsWorld *newWorld = *m_LocalWorld;

      fzxbody.s_Body = newWorld->createRigidBody(transform);

      if (shape == Shape::BOX) {
        physics.s_BodyType = rp3d::BodyType::STATIC;

        physics.s_Scale_C =
            rp3d::Vector3(t.s_Scale.x / 2, t.s_Scale.y / 2, t.s_Scale.z / 2);
        physics.s_BoxShape =
            m_LocalPhysicsCommon->createBoxShape(physics.s_Scale_C);

        bodyCollider = fzxbody.s_Body->addCollider(physics.s_BoxShape,
                                                   rp3d::Transform::identity());
        fzxbody.s_Collider = bodyCollider;
      }

      if (shape == Shape::SPHERE) {

        physics.s_BodyType = rp3d::BodyType::DYNAMIC;

        physics.s_Scale_C =
            rp3d::Vector3(t.s_Scale.x, t.s_Scale.y, t.s_Scale.z);
        physics.s_SphereShape =
            m_LocalPhysicsCommon->createSphereShape(physics.s_Scale_C.x / 2);
        bodyCollider = fzxbody.s_Body->addCollider(physics.s_SphereShape,
                                                   rp3d::Transform::identity());
        fzxbody.s_Collider = bodyCollider;
      }

      fzxbody.s_Body->setType(physics.s_BodyType);

      fzxbody.s_Body->updateMassFromColliders();
      fzxbody.s_Body->setMass(physics.s_Mass);

      m_LocalRegistry->emplace<PhysicsData>(entity, physics);
      m_LocalRegistry->emplace<PhysicsBody>(entity, fzxbody);
    }
  }
};
