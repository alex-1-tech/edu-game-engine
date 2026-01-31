#pragma once

/**
 * @file    systems/debug_render_system.hpp
 * @brief   Debug system for visualizing entity positions
 * @author  alex-1-tech
 * @date    2026
 */

#include "engine/core/logging.hpp"
#include "engine/ecs/components/name.hpp"
#include "engine/ecs/components/transform.hpp"
#include "engine/ecs/system.hpp"

EGE_NAMESPACE_BEGIN

class DebugRenderSystem : public System<Transform>
{
public:
  [[nodiscard]] auto getName() const -> String override { return "DebugRenderSystem"; }

  [[nodiscard]] auto explain() const -> String override
  {
    return "Debug Render System:\n"
           "Visualizes entity positions in console/ASCII art.\n"
           "Educational: Shows how to inspect game state";
  }

protected:
  void process(Entity entity, World& world, Transform& transform, float deltaTime) override
  {
    (void)deltaTime;

    String entityName = entity.toString();
    auto* nameComp = world.template getComponent<Name>(entity);
    if (nameComp != nullptr) {
      entityName = nameComp->name();
    }

    EGE_DEBUG("Entity {} at ({:.1f}, {:.1f})", entityName, transform.position().x, transform.position().y);
  }
};
EGE_NAMESPACE_END
