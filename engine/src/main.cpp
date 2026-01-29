#include <engine/ecs/components/name.hpp>
#include <engine/ecs/components/transform.hpp>
#include <engine/ecs/components/velocity.hpp>
#include <engine/ecs/systems/debug_render_system.hpp>
#include <engine/ecs/systems/movement_system.hpp>
#include <engine/ecs/world.hpp>
#include <engine/platform/application_manager.hpp>

#include "engine/core/types.hpp"

using namespace engine;

class ECSDemo
{
public:
  ECSDemo() { setupWorld(); }

  void update(float deltaTime)
  {
    // Update all ECS systems
    m_world->updateSystems(deltaTime);

    // Educational: Print stats occasionally
    constexpr float STATS_INTERVAL = 2.0F;
    static float statsTimer = 0.0F;
    statsTimer += deltaTime;
    if (statsTimer >= STATS_INTERVAL) {
      EGE_INFO("World has {} entities", m_world->getEntityCount());
      statsTimer = 0.0F;
    }
  }

  void render()
  {
    // Future: Render entities
  }

private:
  Unique<World> m_world = std::make_unique<World>();

  void setupWorld()
  {
    EGE_INFO("Setting up ECS demo world...");

    // Register systems
    m_world->registerSystem<MovementSystem>();
    m_world->registerSystem<DebugRenderSystem>();

    // Create some entities
    constexpr Vec2 PLAYER_POS{100.0F, 100.0F};
    constexpr Vec2 ENEMY_POS{300.0F, 200.0F};
    constexpr Vec2 OBSTACLE_POS{400.0F, 300.0F};
    constexpr Vec2 PLAYER_VEL{50.0F, 0.0F};
    constexpr Vec2 ENEMY_VEL{0.0F, 30.0F};

    Entity player = m_world->createEntity();
    m_world->addComponent<Name>(player, "Player");
    m_world->addComponent<Transform>(player, PLAYER_POS);
    m_world->addComponent<Velocity>(player, PLAYER_VEL);

    Entity enemy = m_world->createEntity();
    m_world->addComponent<Name>(enemy, "Enemy");
    m_world->addComponent<Transform>(enemy, ENEMY_POS);
    m_world->addComponent<Velocity>(enemy, ENEMY_VEL);

    Entity obstacle = m_world->createEntity();
    m_world->addComponent<Name>(obstacle, "Obstacle");
    m_world->addComponent<Transform>(obstacle, OBSTACLE_POS);

    EGE_INFO("Created {} entities", m_world->getEntityCount());
  }
};

auto main() -> int
{
  EGE_INFO("=== ECS Educational Demo ===");

  ApplicationManager app;
  auto demo = std::make_shared<ECSDemo>();

  // Setup callbacks
  app.setUpdateCallback([&demo](float deltaTime) { demo->update(deltaTime); });
  app.setRenderCallback([&demo]() { demo->render(); });

  // Run
  app.run();

  return 0;
}
