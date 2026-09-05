#include <APE_TransformSystem.hpp>

void TransformSystem::TransformEntity(entt::registry &reg) {

  auto view = reg.view<Transform>();

  for (auto [ent, transform] : view.each()) {
    // TRANSLATE
    // ROTATE
    // SCALE
    //
  }
}
