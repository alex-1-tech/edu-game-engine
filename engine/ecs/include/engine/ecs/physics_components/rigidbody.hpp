#pragma once
#include "engine/core/math.hpp"
#include "engine/core/types.hpp"
#include "engine/ecs/component.hpp"

EGE_NAMESPACE_BEGIN

constexpr f32 BASE_RESTITUTION = 0.3F;
constexpr f32 BASE_FRICTION = 0.2F;

struct RigidBodyData {
  // Linear motion
  Vec2 velocity{ZERO_F, ZERO_F};
  Vec2 acceleration{ZERO_F, ZERO_F};
  Vec2 force{ZERO_F, ZERO_F};

  // Rotational motion
  f32 angularVelocity = ZERO_F;
  f32 angularAcceleration = ZERO_F;
  f32 torque = ZERO_F;

  // Mass and inertia
  f32 mass = ONE_F;
  f32 invMass = ONE_F;    // 1/mass for optimization
  f32 inertia = ONE_F;    // Moment of inertia
  f32 invInertia = ONE_F; // 1/inertia

  // Свойства материала
  f32 restitution = BASE_RESTITUTION; // Elasticity (0.0-1.0)
  f32 friction = BASE_FRICTION;       // Coefficient of friction

  // Status flags
  bool useGravity = true;
  bool isKinematic = false; // Controlled by code, not physics
  bool isStatic = false;    // It doesn't move at all
  bool isSleeping = false;  // Optimization: sleeping object
};

class RigidBody : public Component<RigidBody>
{
public:
  // Methods
  void applyForce(const Vec2& forceVec, const Vec2& point = Vec2(0, 0));
  void applyImpulse(const Vec2& impulse, const Vec2& contactPoint);
  void applyTorque(f32 torqueValue);
  void clearForces();

  // Calculations
  void calculateInertia(f32 width, f32 height, f32 mass);
  void calculateCircleInertia(f32 radius, f32 mass);

  [[nodiscard]] auto getKineticEnergy() const -> f32;
  [[nodiscard]] auto getMomentum() const -> Vec2;

  [[nodiscard]] auto toString() const -> String override;
  [[nodiscard]] auto explain() const -> String override;

private:
  RigidBodyData m_data;
};

EGE_NAMESPACE_END