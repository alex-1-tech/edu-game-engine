#pragma once
#include "engine/core/math.hpp"
#include "engine/ecs/component.hpp"
#include "engine/ecs/components/transform.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/graphics/types.hpp"

EGE_NAMESPACE_BEGIN

enum class ColliderType : u8 {
  BOX = 0, // AABB (Axis-Aligned Bounding Box)
  CIRCLE,  // Circle
  POLYGON, // Convex polygon (SAT)
  EDGE,    // Segment (for platforms)
  COMPOUND // Composite Collider
};

struct BoxShape {
  Vec2 halfExtents;
};
struct CircleShape {
  f32 radius;
};
struct PolygonShape {
  std::vector<Vec2> vertices;
  std::vector<Vec2> normals;
};
struct EdgeShape {
  Vec2 start, end;
};
using ColliderShape = std::variant<BoxShape, CircleShape, PolygonShape, EdgeShape>;

constexpr u32 BASE_COLLISION_MASK = 0xFFFF;
struct ColliderData {
  ColliderType type = ColliderType::BOX;
  ColliderShape shape;

  // Offset relative to the center of the transformer
  Vec2 offset{ZERO_F, ZERO_F};

  // Flags
  bool isTrigger = false; // Events only, no physical reaction
  bool isSensor = false;  // For collision-free detection

  // Collision layers (bitmask)
  u32 collisionLayer = ONE;                // Object layer
  u32 collisionMask = BASE_COLLISION_MASK; // Which layers collides with

  // Precomputed data for optimization
  mutable Rect cachedAABB; // Cached AABB in world coordinates
  mutable bool aabbDirty = true;
};

class Collider : public Component<Collider>
{
public:
  // Method
  void setAsBox(f32 width, f32 height, const Vec2& center = Vec2(0, 0));
  void setAsCircle(f32 radius, const Vec2& center = Vec2(0, 0));
  void setAsPolygon(const std::vector<Vec2>& vertices);
  void setAsEdge(const Vec2& start, const Vec2& end);

  // Accessors
  [[nodiscard]] auto getType() const -> ColliderType { return m_data.type; }
  [[nodiscard]] auto getOffset() const -> Vec2 { return m_data.offset; }
  [[nodiscard]] auto getRadius() const -> f32
  {
    if (std::holds_alternative<CircleShape>(m_data.shape)) {
      return std::get<CircleShape>(m_data.shape).radius;
    }
    return 0.0F;
  }
  [[nodiscard]] auto getBoxHalfExtents() const -> Vec2
  {
    if (std::holds_alternative<BoxShape>(m_data.shape)) {
      return std::get<BoxShape>(m_data.shape).halfExtents;
    }
    return {0, 0};
  }
  [[nodiscard]] auto getEdgePoints() const -> std::pair<Vec2, Vec2>
  {
    if (std::holds_alternative<EdgeShape>(m_data.shape)) {
      const auto& edge = std::get<EdgeShape>(m_data.shape);
      return {edge.start, edge.end};
    }
    return {Vec2(0, 0), Vec2(0, 0)};
  }
  [[nodiscard]] auto getPolygonVertices() const -> std::vector<Vec2>
  {
    if (std::holds_alternative<PolygonShape>(m_data.shape)) {
      return std::get<PolygonShape>(m_data.shape).vertices;
    }
    return {};
  }

  // Geometrical in Apostille
  [[nodiscard]] auto getWorldVertices(const Transform& transform) const -> std::vector<Vec2>;
  [[nodiscard]] auto getAABB(const Transform& transform) const -> Rect;
  [[nodiscard]] auto getSupportPoint(const Vec2& direction, const Transform& transform) const -> Vec2;

  // SAT (Separating Axis Theorem)
  [[nodiscard]] auto getNormals(const Transform& transform) const -> std::vector<Vec2>;
  [[nodiscard]] auto projectOntoAxis(const Vec2& axis, const Transform& transform) const -> std::pair<f32, f32>; // min и max projections

  // Visualization
  void debugDraw(Renderer* renderer, const Transform& transform, const Color& color) const;


private:
  ColliderData m_data;

  void computePolygonNormals();
  void updateCachedData(const Transform& transform) const;
};

EGE_NAMESPACE_END