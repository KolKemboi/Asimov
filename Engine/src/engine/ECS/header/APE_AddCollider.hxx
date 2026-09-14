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
  // sets pointers to global world, physicsCommon and registry data
  void SetColliderRequirements(rp3d::PhysicsWorld *&world,
                               rp3d::PhysicsCommon &phyCom,
                               entt::registry &registry) {
    m_LocalWorld = &world;
    m_LocalPhysicsCommon = &phyCom;
    m_LocalRegistry = &registry;
  }

  void SetDispatcher(Dispatcher &dispatcher) {
    // adds a collider when the event is triggered
    dispatcher.Subscriber(EventType::OBJECT_ADDED,
                          [this](const std::string &data) { AddCollider(); });

    // Removes all the previous collider and body data and creates new ones
    dispatcher.Subscriber(
        EventType::OBJECT_MODIFIED, [this](const std::string &data) {
          // delete phys data and phys body
          // also check the selected
          auto selectedView = m_LocalRegistry->view<Selected>();
          for (entt::entity entity : selectedView) {
            // ensures there world and physics common do not have the previous
            // colliders in the system
            auto body = m_LocalRegistry->get<PhysicsBody>(entity);
            rp3d::PhysicsWorld *newWorld = *m_LocalWorld;
            body.s_Body->removeCollider(body.s_Collider);
            newWorld->destroyRigidBody(body.s_Body);

            // clears the registry of the Physics Data and Physics body, then
            // adds a new collider
            m_LocalRegistry->remove<PhysicsData>(entity);
            m_LocalRegistry->remove<PhysicsBody>(entity);
            AddCollider();
          }
        });
    dispatcher.Subscriber(
        EventType::COLLIDER_TYPE_MODIFIED, [this](const std::string &data) {
          // delete phys data and phys body
          // also check the selected
          auto selectedView = m_LocalRegistry->view<Selected>();
          for (entt::entity entity : selectedView) {
            // ensures there world and physics common do not have the previous
            // colliders in the system
            auto body = m_LocalRegistry->get<PhysicsBody>(entity);
            rp3d::PhysicsWorld *newWorld = *m_LocalWorld;
            body.s_Body->removeCollider(body.s_Collider);
            newWorld->destroyRigidBody(body.s_Body);

            // clears the registry of the Physics Data and Physics body, then
            // adds a new collider
            m_LocalRegistry->remove<PhysicsData>(entity);
            m_LocalRegistry->remove<PhysicsBody>(entity);
            AddCollider(data);
          }
        });
  }

private:
  // local pointer variables to global variables
  rp3d::PhysicsWorld *
      *m_LocalWorld; // ptr to a ptr, prevents possible memory copy
  rp3d::PhysicsCommon *m_LocalPhysicsCommon;
  entt::registry *m_LocalRegistry;

public:
  void AddCollider(const std::string &bodyType = "STATIC") {

    rp3d::Collider *bodyCollider;
    // creates an instance of physics data to be filled for the ECS
    PhysicsData fzxData;

    // set type, defaults to static
    if (bodyType == "STATIC")
      fzxData.s_BodyType = rp3d::BodyType::STATIC;
    if (bodyType == "KINEMATIC")
      fzxData.s_BodyType = rp3d::BodyType::KINEMATIC;
    if (bodyType == "DYNAMIC")
      fzxData.s_BodyType = rp3d::BodyType::DYNAMIC;

    // loops over the selected entities
    auto selectedView = m_LocalRegistry->view<Selected>();
    for (entt::entity entity : selectedView) {

      // takes transform and shape type from the selected entity
      auto &t = m_LocalRegistry->get<Transform>(entity);
      auto &shape = m_LocalRegistry->get<Shape>(entity);

      // grabs the transform data from the mesh and sets it as the physics
      // collider transforms
      fzxData.s_Position_C =
          rp3d::Vector3(t.s_Position.x, t.s_Position.y, t.s_Position.z);
      fzxData.s_Rotation_C = rp3d::Quaternion::fromEulerAngles(
          glm::radians(t.s_Rotation.x), glm::radians(t.s_Rotation.y),
          glm::radians(
              t.s_Rotation.z)); // glm::radians, idk why rp3d docs dont make
                                // this clear, but radians is the way to go

      // mass of the collider,
      fzxData.s_Mass = 1.0;
      // sets  transform
      rp3d::Transform transform(fzxData.s_Position_C, fzxData.s_Rotation_C);

      // creates a physicsbody instance to be filled
      PhysicsBody fzxBody;
      // dereferences the m_LocalWorld ptr, to used use as a ptr
      rp3d::PhysicsWorld *newWorld = *m_LocalWorld;
      fzxBody.s_Body = newWorld->createRigidBody(transform);

      // BOX shape type
      if (shape == Shape::BOX) {
        fzxData.s_Scale_C =
            rp3d::Vector3(t.s_Scale.x / 2, t.s_Scale.y / 2, t.s_Scale.z / 2);
        fzxData.s_BoxShape =
            m_LocalPhysicsCommon->createBoxShape(fzxData.s_Scale_C);
        bodyCollider = fzxBody.s_Body->addCollider(fzxData.s_BoxShape,
                                                   rp3d::Transform::identity());
        fzxBody.s_Collider = bodyCollider;
      }

      // SPHERE shape type
      if (shape == Shape::SPHERE) {
        fzxData.s_Scale_C =
            rp3d::Vector3(t.s_Scale.x, t.s_Scale.y, t.s_Scale.z);
        fzxData.s_SphereShape =
            m_LocalPhysicsCommon->createSphereShape(fzxData.s_Scale_C.x / 2);
        bodyCollider = fzxBody.s_Body->addCollider(fzxData.s_SphereShape,
                                                   rp3d::Transform::identity());
        fzxBody.s_Collider = bodyCollider;
      }

      // CONVEXMESH shape type used a shape primitive
      if (shape == Shape::CONVEXMESH) {
        fzxData.s_Scale_C =
            rp3d::Vector3(t.s_Scale.x, t.s_Scale.y, t.s_Scale.z);
        fzxData.s_SphereShape =
            m_LocalPhysicsCommon->createSphereShape(fzxData.s_Scale_C.x / 2);
        bodyCollider = fzxBody.s_Body->addCollider(fzxData.s_SphereShape,
                                                   rp3d::Transform::identity());
        fzxBody.s_Collider = bodyCollider;
      }

      // CAPSULE shape type
      if (shape == Shape::CAPSULE) {
        fzxData.s_Scale_C =
            rp3d::Vector3(t.s_Scale.x, t.s_Scale.y, t.s_Scale.z);
        fzxData.s_CapsuleShape = m_LocalPhysicsCommon->createCapsuleShape(
            fzxData.s_Scale_C.y / 2, fzxData.s_Scale_C.y / 2);

        bodyCollider = fzxBody.s_Body->addCollider(fzxData.s_CapsuleShape,
                                                   rp3d::Transform::identity());
        fzxBody.s_Collider = bodyCollider;
      }

      fzxBody.s_Body->setType(fzxData.s_BodyType);
      fzxBody.s_Body->updateMassFromColliders();
      fzxBody.s_Body->setMass(fzxData.s_Mass);

      // adds ths phys
      m_LocalRegistry->emplace<PhysicsData>(entity, fzxData);
      m_LocalRegistry->emplace<PhysicsBody>(entity, fzxBody);
    }
  }
};
