#pragma once

#include "../components/name.hpp"
#include "../components/transform.hpp"
#include "../system.hpp"

EGE_NAMESPACE_BEGIN

class DebugRenderSystem : public System<Transform>
{
public:
  [[nodiscard]] auto getName() const -> std::string override { return "DebugRenderSystem"; }

  [[nodiscard]] auto explain() const -> std::string override
  {
    return "Debug Render System:\n"
           "Visualizes entity positions in console/ASCII art.\n"
           "Educational: Shows how to inspect game state";
  }

protected:
  void process(Entity entity, World& world, Transform& transform, float deltaTime) override
  {
    (void)deltaTime;

    std::string entityName = entity.toString();
    auto* nameComp = world.template getComponent<Name>(entity);
    if (nameComp != nullptr) {
      entityName = nameComp->name();
    }

    EGE_DEBUG("Entity {} at ({:.1f}, {:.1f})", entityName, transform.position().x, transform.position().y);
  }
};
EGE_NAMESPACE_END
