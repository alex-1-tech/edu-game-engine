#include <engine/ecs/components/name.hpp>
#include <engine/ecs/components/transform.hpp>
#include <engine/ecs/components/velocity.hpp>
#include <engine/ecs/graphics_components/shape.hpp>
#include <engine/ecs/graphics_components/sprite.hpp>
#include <engine/ecs/graphics_components/text.hpp>
#include <engine/ecs/systems/movement_system.hpp>
#include <engine/ecs/systems/render_system.hpp>
#include <engine/ecs/world.hpp>
#include <engine/graphics/sdl_renderer.hpp>
#include <engine/platform/application_manager.hpp>

#include "engine/core/config.hpp"
#include "engine/core/types.hpp"

using namespace engine;

namespace GameConstants
{
constexpr f32 PLAYER_POS_X = 100.0F;
constexpr f32 PLAYER_POS_Y = 100.0F;
constexpr f32 PLAYER_SCALE = 1.5F;
constexpr f32 PLAYER_VEL_X = 50.0F;
constexpr f32 PLAYER_SIZE = 40.0F;
constexpr f32 PLAYER_TEXT_SIZE = 12.0F;
constexpr f32 PLAYER_TEXT_OFFSET = -30.0F;

constexpr f32 ENEMY_POS_X = 300.0F;
constexpr f32 ENEMY_POS_Y = 200.0F;
constexpr f32 ENEMY_VEL_Y = 30.0F;
constexpr f32 ENEMY_RADIUS = 25.0F;
constexpr f32 ENEMY_TEXT_SIZE = 12.0F;
constexpr f32 ENEMY_TEXT_OFFSET = -35.0F;

constexpr f32 OBSTACLE_POS_X = 400.0F;
constexpr f32 OBSTACLE_POS_Y = 300.0F;
constexpr f32 OBSTACLE_OUTLINE = 3.0F;
constexpr f32 OBSTACLE_SIZE = 60.0F;

constexpr f32 TARGET_POS_X = 500.0F;
constexpr f32 TARGET_POS_Y = 150.0F;
constexpr f32 TARGET_VEL_X = -20.0F;
constexpr f32 TARGET_LINE_END_X = 50.0F;
constexpr f32 TARGET_LINE_END_Y = 50.0F;
constexpr f32 TARGET_TEXT_SIZE = 14.0F;
constexpr f32 TARGET_TEXT_OFFSET_X = 25.0F;
constexpr f32 TARGET_TEXT_OFFSET_Y = 30.0F;

constexpr f32 STATIC_POS_X = 600.0F;
constexpr f32 STATIC_POS_Y = 400.0F;
constexpr f32 STATIC_RECT_W = 80.0F;
constexpr f32 STATIC_RECT_H = 40.0F;
constexpr f32 STATIC_CIRCLE_RADIUS = 30.0F;
constexpr f32 STATIC_CIRCLE_OUTLINE = 2.0F;
} // namespace GameConstants

class ECSGraphicsDemo
{
public:
  ECSGraphicsDemo() { setupWorld(); }

  void update(float deltaTime) { m_world->updateSystems(deltaTime); }

  void render()
  {
    if (m_renderer) {
      m_renderer->clear(Color::darkGray());

      m_renderer->present();
    }
  }

private:
  Unique<World> m_world = std::make_unique<World>();
  Unique<SDLRenderer> m_renderer;

  void setupWorld()
  {
    EGE_INFO("Setting up ECS Graphics Demo...");

    ApplicationManager app(&EngineConfig::getInstance());
    if (auto* window = app.getWindow()) {
      m_renderer = std::make_unique<SDLRenderer>(window);
      if (!m_renderer->isValid()) {
        EGE_ERROR("Failed to create renderer");
        return;
      }
    }

    m_world->registerSystem<MovementSystem>();
    m_world->registerSystem<RenderSystem>(m_renderer.get());

    Entity player = m_world->createEntity();
    m_world->addComponent<Name>(player, "Player");
    m_world->addComponent<Transform>(player, Vec2(GameConstants::PLAYER_POS_X, GameConstants::PLAYER_POS_Y), 0.0F,
                                     Vec2(GameConstants::PLAYER_SCALE, GameConstants::PLAYER_SCALE));
    m_world->addComponent<Velocity>(player, Vec2(GameConstants::PLAYER_VEL_X, 0.0F));

    Shape playerShape;
    playerShape.type = Shape::Type::RECTANGLE;
    playerShape.color = Color::red();
    playerShape.filled = true;
    playerShape.size = Vec2(GameConstants::PLAYER_SIZE, GameConstants::PLAYER_SIZE);
    m_world->addComponent<Shape>(player, playerShape);

    Text playerText;
    playerText.content = "Player";
    playerText.color = Color::white();
    playerText.fontSize = GameConstants::PLAYER_TEXT_SIZE;
    playerText.offset = Vec2(0.0F, GameConstants::PLAYER_TEXT_OFFSET);
    m_world->addComponent<Text>(player, playerText);

    Entity enemy = m_world->createEntity();
    m_world->addComponent<Name>(enemy, "Enemy");
    m_world->addComponent<Transform>(enemy, Vec2(GameConstants::ENEMY_POS_X, GameConstants::ENEMY_POS_Y));
    m_world->addComponent<Velocity>(enemy, Vec2(0.0F, GameConstants::ENEMY_VEL_Y));

    Shape enemyShape;
    enemyShape.type = Shape::Type::CIRCLE;
    enemyShape.color = Color::green();
    enemyShape.filled = true;
    enemyShape.radius = GameConstants::ENEMY_RADIUS;
    m_world->addComponent<Shape>(enemy, enemyShape);

    Text enemyText;
    enemyText.content = "Enemy";
    enemyText.color = Color::white();
    enemyText.fontSize = GameConstants::ENEMY_TEXT_SIZE;
    enemyText.offset = Vec2(0.0F, GameConstants::ENEMY_TEXT_OFFSET);
    m_world->addComponent<Text>(enemy, enemyText);

    Entity obstacle = m_world->createEntity();
    m_world->addComponent<Name>(obstacle, "Obstacle");
    m_world->addComponent<Transform>(obstacle, Vec2(GameConstants::OBSTACLE_POS_X, GameConstants::OBSTACLE_POS_Y));

    Shape obstacleShape;
    obstacleShape.type = Shape::Type::RECTANGLE;
    obstacleShape.color = Color::blue();
    obstacleShape.filled = false;
    obstacleShape.outlineThickness = GameConstants::OBSTACLE_OUTLINE;
    obstacleShape.size = Vec2(GameConstants::OBSTACLE_SIZE, GameConstants::OBSTACLE_SIZE);
    m_world->addComponent<Shape>(obstacle, obstacleShape);

    Entity target = m_world->createEntity();
    m_world->addComponent<Name>(target, "Target");
    m_world->addComponent<Transform>(target, Vec2(GameConstants::TARGET_POS_X, GameConstants::TARGET_POS_Y));
    m_world->addComponent<Velocity>(target, Vec2(GameConstants::TARGET_VEL_X, 0.0F));

    Shape targetShape;
    targetShape.type = Shape::Type::LINE;
    targetShape.color = Color::yellow();
    targetShape.filled = false;
    targetShape.endPoint = Vec2(GameConstants::TARGET_LINE_END_X, GameConstants::TARGET_LINE_END_Y);
    m_world->addComponent<Shape>(target, targetShape);

    Text targetText;
    targetText.content = "Target";
    targetText.color = Color::yellow();
    targetText.fontSize = GameConstants::TARGET_TEXT_SIZE;
    targetText.offset = Vec2(GameConstants::TARGET_TEXT_OFFSET_X, GameConstants::TARGET_TEXT_OFFSET_Y);
    m_world->addComponent<Text>(target, targetText);

    Entity staticObj = m_world->createEntity();
    m_world->addComponent<Name>(staticObj, "Static");
    m_world->addComponent<Transform>(staticObj, Vec2(GameConstants::STATIC_POS_X, GameConstants::STATIC_POS_Y));

    Shape staticRectShape;
    staticRectShape.type = Shape::Type::RECTANGLE;
    staticRectShape.color = Color::cyan();
    staticRectShape.filled = true;
    staticRectShape.size = Vec2(GameConstants::STATIC_RECT_W, GameConstants::STATIC_RECT_H);
    m_world->addComponent<Shape>(staticObj, staticRectShape);

    Shape staticCircleShape;
    staticCircleShape.type = Shape::Type::CIRCLE;
    staticCircleShape.color = Color::purple();
    staticCircleShape.filled = false;
    staticCircleShape.outlineThickness = GameConstants::STATIC_CIRCLE_OUTLINE;
    staticCircleShape.radius = GameConstants::STATIC_CIRCLE_RADIUS;
    m_world->addComponent<Shape>(staticObj, staticCircleShape);

    EGE_INFO("Created {} entities with graphical components", m_world->getEntityCount());
  }
};

auto main() -> int
{
  EGE_INFO("=== ECS Graphics Educational Demo ===");

  EngineConfig::getInstance().intialize();

  ApplicationManager app(&EngineConfig::getInstance());
  auto demo = std::make_shared<ECSGraphicsDemo>();

  app.setUpdateCallback([&demo](float deltaTime) { demo->update(deltaTime); });

  app.setRenderCallback([&demo]() { demo->render(); });

  app.run();

  EGE_INFO("Demo completed successfully");
  return 0;
}