#pragma once
#include "APE_Dispatcher.hpp"
#include <APE_Components.hpp>
#include <cstdio>
#include <string>

class DuplicateMeshSystem {
public:
  // add entity system, but renamed
  void AddDuplicate(entt::registry &registry, Dispatcher &dispatcher) {

    auto selectedView = registry.view<Selected>();

    for (auto [entity] : selectedView.each()) {

      entt::entity ent = registry.create();
      const auto &[name, count, trans, mat, renderable, shape] =
          registry
              .get<Name, ObjectCount, Transform, Material, Renderable, Shape>(
                  entity);

      //--TODO: look up better naming here
      // reverted to adding count
      auto view = registry.view<Name, ObjectCount>();

      // the entity has not been created ---yet
      unsigned int ObjNumber = 1;
      unsigned int max_count = 0;

      // this is where the bug is
      for (auto [ent, n, count] : view.each()) {

        // name and count
        if (strcmp(n.s_Name.c_str(), name.s_Name.c_str()) == 0) {
          if (count.s_Count > max_count)
            max_count = count.s_Count;
          ObjNumber = max_count + 1;
        }
      }

      // make the thing with the things, unless there's a a bug, or anything
      // changes, dont touch this
      registry.emplace<Name>(ent, name);
      registry.emplace<Transform>(ent, trans);
      registry.emplace<ObjectCount>(ent, ObjNumber);
      registry.emplace<Material>(ent, mat);
      registry.emplace<Renderable>(ent, renderable);
      registry.emplace<Shape>(ent, shape);
      registry.remove<Selected>(entity);
      registry.emplace<Selected>(ent);

      // this is to make its collider
      dispatcher.Emitter(EventType::OBJECT_ADDED, "Selected object Duplicated");
    }
  }
};
