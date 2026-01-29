#pragma once

/**
 * @file    components/transform.hpp
 * @brief   Transform component for 2D position, rotation and scale
 * @author  alex-1-tech
 * @date    2026
 */

#include <fmt/format.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/core/math.hpp"
#include "engine/core/types.hpp"
#include "engine/ecs/component.hpp"
#include "engine/ecs/entity.hpp"

EGE_NAMESPACE_BEGIN

class Transform final : public Component<Transform>
{
public:
  static constexpr f32 DEFAULT_ROTATION = ZERO_F;
  static constexpr Vec2 DEFAULT_SCALE{ONE_F, ONE_F};

  static constexpr Vec3 ROTATION_AXIS_Z{ZERO_F, ZERO_F, ONE_F};
  static constexpr Vec3 POSITION_Z_OFFSET{ZERO_F, ZERO_F, ZERO_F};

  Transform() = default;

  explicit Transform(Vec2 pos, f32 rot = DEFAULT_ROTATION, Vec2 scl = DEFAULT_SCALE)
      : m_position(pos)
      , m_rotation(rot)
      , m_scale(scl)
  {
  }

  [[nodiscard]] auto position() const -> const Vec2& { return m_position; }
  [[nodiscard]] auto rotation() const -> f32 { return m_rotation; }
  [[nodiscard]] auto scale() const -> const Vec2& { return m_scale; }
  [[nodiscard]] auto parent() const -> EntityId { return m_parent; }

  void setPosition(const Vec2& pos) { m_position = pos; }
  void setRotation(float rot) { m_rotation = rot; }
  void setScale(const Vec2& scl) { m_scale = scl; }
  void setParent(EntityId parent) { m_parent = parent; }

  [[nodiscard]] auto toString() const -> String override
  {
    return fmt::format("Transform(pos=({:.1f}, {:.1f}), rot={:.1f}°, scale=({:.1f}, {:.1f}))", m_position.x,
                       m_position.y, math::degrees(m_rotation), m_scale.x, m_scale.y);
  }

  [[nodiscard]] auto explain() const -> String override
  {
    return fmt::format("Transform Component:\n"
                       "├─ Position: ({:.2f}, {:.2f})\n"
                       "├─ Rotation: {:.1f}° ({:.3f} rad)\n"
                       "├─ Scale: ({:.2f}, {:.2f})\n"
                       "└─ Parent EntityId: {}",
                       m_position.x, m_position.y, math::degrees(m_rotation), m_rotation, m_scale.x,
                       m_scale.y,
                       (m_parent != INVALID_ENTITY_ID) ? std::to_string(m_parent.get()) : String("None"));
  }

  [[nodiscard]] auto getWorldMatrix() const -> Mat4
  {
    Mat4 mat(ONE_F);

    mat = glm::translate(mat, Vec3(m_position, ZERO_F));
    mat = glm::rotate(mat, m_rotation, ROTATION_AXIS_Z);
    mat = glm::scale(mat, Vec3(m_scale, ONE_F));

    return mat;
  }

  [[nodiscard]] auto visualizeMatrix() const -> String
  {
    const Mat4 mat = getWorldMatrix();
    String out;

    for (glm::length_t row = 0; row < Mat4::length(); ++row) {
      out += "[ ";
      for (glm::length_t col = 0; col < Mat4::col_type::length(); ++col) {
        out += fmt::format("{:.2f} ", mat[col][row]);
      }
      out += "]\n";
    }

    return out;
  }

private:
  Vec2 m_position{ZERO_F, ZERO_F};
  f32 m_rotation{DEFAULT_ROTATION};
  Vec2 m_scale{DEFAULT_SCALE};
  EntityId m_parent{INVALID_ENTITY_ID};
};

EGE_NAMESPACE_END
