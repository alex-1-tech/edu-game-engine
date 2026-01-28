#pragma once

/**
 * @file    systems/movement_system.hpp
 * @brief   Movement system for updating entity transforms based on velocity
 * @author  alex-1-tech
 * @date    2026
 */

#include "engine/core/logging.hpp"
#include "engine/ecs/components/transform.hpp"
#include "engine/ecs/components/velocity.hpp"
#include "engine/ecs/system.hpp"

EGE_NAMESPACE_BEGIN

constexpr int LOG_EVERY_N_FRAMES = 60;


class MovementSystem : public System<Transform, Velocity>
{
public:
  [[nodiscard]] auto getName() const -> String override { return "MovementSystem"; }

  [[nodiscard]] auto explain() const -> String override
  {
    return "Movement System:\n"
           "Processes entities with Transform and Velocity components.\n"
           "Updates position using: position += velocity * deltaTime\n"
           "Educational: Demonstrates Euler integration";
  }

protected:
  void process(Entity entity, World& /*world*/, Transform& transform, Velocity& velocity,
               float deltaTime) override
  {
    Vec2 oldPosition = transform.position();

    transform.setPosition(transform.position() + velocity.linear() * deltaTime);
    transform.setRotation(transform.rotation() + (velocity.angular() * deltaTime));

    static int frameCounter = 0;
    if (frameCounter++ % LOG_EVERY_N_FRAMES == 0) {
      float distance = glm::length(transform.position() - oldPosition);
      EGE_EXPLAIN("MovementSystem: Entity {} moved {:.2f} units\n"
                  "  From: ({:.1f}, {:.1f})\n"
                  "  To:   ({:.1f}, {:.1f})\n"
                  "  Velocity: ({:.1f}, {:.1f}) u/s",
                  entity.toString(), distance, oldPosition.x, oldPosition.y, transform.position().x,
                  transform.position().y, velocity.linear().x, velocity.linear().y);
    }
  }
};

EGE_NAMESPACE_END