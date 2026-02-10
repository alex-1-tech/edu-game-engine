#pragma once
#include "engine/core/types.hpp"
#include "engine/ecs/component.hpp"

EGE_NAMESPACE_BEGIN


constexpr f32 BASE_DENSITY = 1.0F;
constexpr f32 BASE_RESISTANCE = 0.3F;
constexpr f32 BASE_STATIC_FRICTION = 0.5F;
constexpr f32 BASE_DYNAMIC_FRICTION = 0.3F;

constexpr f32 BASE_BOUNCINESS = 0.5F;
constexpr f32 BASE_HEALTH = 100.0F;

struct PhysicsMaterialData {
  String name = "Default";

  // Mechanical properties
  f32 density = BASE_DENSITY;                  // Density (kg/m2 for 2D)
  f32 restriction = BASE_RESISTANCE;           // Elasticity
  f32 staticFriction = BASE_STATIC_FRICTION;   // Resting friction
  f32 dynamicFriction = BASE_DYNAMIC_FRICTION; // Sliding friction

  // Additional properties
  f32 bounciness = BASE_BOUNCINESS; // Additional bounce rate
  bool isDestructible = false;
  f32 health = BASE_HEALTH;

  // Categories of materials (for sounds/effects)
  enum class Category : u8 { METAL, WOOD, STONE, RUBBER, GLASS, ICE, CUSTOM };
  Category category = Category::CUSTOM;
};

class PhysicsMaterial : public Component<PhysicsMaterial>
{
public:
  [[nodiscard]] auto calculateMass(f32 area) const -> f32 { return m_data.density * area; }

  [[nodiscard]] auto toString() const -> String override;
  [[nodiscard]] auto explain() const -> String override;

  // Static presets
  static auto metal() -> PhysicsMaterial;
  static auto wood() -> PhysicsMaterial;
  static auto rubber() -> PhysicsMaterial;
  static auto ice() -> PhysicsMaterial;

private:
  PhysicsMaterialData m_data;
};

EGE_NAMESPACE_END