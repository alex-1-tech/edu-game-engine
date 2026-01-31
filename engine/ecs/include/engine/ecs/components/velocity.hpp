#pragma once

/**
 * @file    components/velocity.hpp
 * @brief   Velocity component for linear and angular movement
 * @author  alex-1-tech
 * @date    2026
 */

#include "engine/core/math.hpp"
#include "engine/ecs/component.hpp"

EGE_NAMESPACE_BEGIN

class Velocity final : public Component<Velocity>
{
public:
  Velocity() = default;

  explicit Velocity(const Vec2& lin, f32 ang = ZERO_F)
      : m_linear(lin)
      , m_angular(ang)
  {
  }

  [[nodiscard]] auto linear() const -> const Vec2& { return m_linear; }
  [[nodiscard]] auto angular() const -> f32 { return m_angular; }

  void setLinear(const Vec2& lin) { m_linear = lin; }
  void setAngular(f32 ang) { m_angular = ang; }

  [[nodiscard]] auto speed() const -> f32 { return glm::length(m_linear); }

  [[nodiscard]] auto predictPosition(const Vec2& current, f32 deltaTime) const -> Vec2
  {
    return current + (m_linear * deltaTime);
  }

  [[nodiscard]] auto toString() const -> String override
  {
    return fmt::format("Velocity(linear=({:.1f}, {:.1f}), angular={:.1f}°/s)", m_linear.x, m_linear.y,
                       math::degrees(m_angular));
  }

  [[nodiscard]] auto explain() const -> String override
  {
    return fmt::format("Velocity Component:\n"
                       "├─ Linear: ({:.2f}, {:.2f}) u/s - movement per second\n"
                       "├─ Angular: {:.1f}°/s - rotation per second\n"
                       "└─ Speed: {:.2f} u/s - magnitude of movement",
                       m_linear.x, m_linear.y, math::degrees(m_angular), speed());
  }

private:
  Vec2 m_linear{ZERO_F, ZERO_F};
  f32 m_angular{ZERO_F};
};

EGE_NAMESPACE_END
