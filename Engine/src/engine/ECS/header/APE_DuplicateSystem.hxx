#pragma once
#include "APE_Dispatcher.hpp"
#include <APE_Components.hpp>
#include <algorithm>
#include <cstdio>
#include <string>

class DuplicateMeshSystem {
public:
  void AddDuplicate(entt::registry &registry, Dispatcher &dispatcher) {
    auto selectedView = registry.view<Selected>();

    for (auto [entity] : selectedView.each()) {
      entt::entity ent = registry.create();
      const auto &[name, count, trans, mat, renderable, shape] =
          registry.get<Name, ObjectCount, Transform, Material, Renderable, Shape>(
              entity);
      printf("%s\n", name.s_Name.c_str());
      printf("%d\n", count.s_Count);

      //--TODO: look up better naming here
      std::string dup_name = name.s_Name + "_D";
      // std::string dup_name = name.s_Name;

      unsigned int max_count = 0;
      unsigned int objectNumber = 0;
      auto view = registry.view<Name, ObjectCount>();

      for (auto [entiti, name, count] : view.each()) {
        if (name.s_Name.find("_D")) {
          std::string newName = name.s_Name + "_D";
          dup_name = newName;
          if (count.s_Count > max_count) {
            max_count = count.s_Count;
          }
          objectNumber = max_count + 1;
        }
      }

      printf("%s\n", dup_name.c_str());

      registry.emplace<Name>(ent, dup_name);
      registry.emplace<Transform>(ent, trans);
      registry.emplace<ObjectCount>(ent, objectNumber);
      registry.emplace<Material>(ent, mat);
      registry.emplace<Renderable>(ent, renderable);
      registry.emplace<Shape>(ent, shape);
      registry.remove<Selected>(entity);
      registry.emplace<Selected>(ent);

      dispatcher.Emitter(EventType::OBJECT_ADDED,
                         "Selected object Duplicated");
    }
  }
};
