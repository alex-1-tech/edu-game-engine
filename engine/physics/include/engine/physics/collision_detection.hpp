#pragma once

/**
 * @file    collision_detection.hpp
 * @brief   Collision detection algorithms for 2D physics
 * @author  alex-1-tech
 * @date    2026
 */

#include <fmt/format.h>

#include <array>
#include <vector>

#include "engine/ecs/components/transform.hpp"
#include "engine/ecs/physics_components/collider.hpp"
#include "engine/physics/collision_types.hpp"

EGE_NAMESPACE_BEGIN

class CollisionDetection final
{
public:
  [[nodiscard]] static auto detect(Entity entityAlpha, Entity entityBeta, World& world) -> CollisionResult;

  [[nodiscard]] static auto boxVsBox(const Transform& transformAlpha, const Collider& colliderAlpha, const Transform& transformBeta,
                                     const Collider& colliderBeta) -> CollisionResult;

  [[nodiscard]] static auto circleVsCircle(const Transform& transformAlpha, const Collider& colliderAlpha, const Transform& transformBeta,
                                           const Collider& colliderBeta) -> CollisionResult;

  [[nodiscard]] static auto boxVsCircle(const Transform& transformBox, const Collider& colliderBox, const Transform& transformCircle,
                                        const Collider& colliderCircle) -> CollisionResult;

  [[nodiscard]] static auto polygonVsPolygon(const Transform& transformAlpha, const Collider& colliderAlpha, const Transform& transformBeta,
                                             const Collider& colliderBeta) -> CollisionResult;

  [[nodiscard]] static auto edgeVsCircle(const Transform& transformEdge, const Collider& colliderEdge, const Transform& transformCircle,
                                         const Collider& colliderCircle) -> CollisionResult;

  [[nodiscard]] static auto satTest(const std::vector<Vec2>& verticesPolygonA, const std::vector<Vec2>& verticesPolygonB,
                                    const std::vector<Vec2>& axesNormals) -> std::pair<bool, CollisionResult>;

  [[nodiscard]] static auto continuousDetection(Entity entityAlpha, Entity entityBeta, World& world, f32 deltaTime) -> CollisionResult;

  [[nodiscard]] static auto calculateContactPoints(const CollisionResult& result, World& world) -> std::vector<Vec2>;

  [[nodiscard]] static auto calculateManifold(const CollisionResult& result, World& world) -> CollisionResult::ContactData;

  [[nodiscard]] static auto closestPointOnPolygon(const Vec2& point, const std::vector<Vec2>& polygon) -> Vec2;

  [[nodiscard]] static auto findClosestFeatures(const Transform& transformAlpha, const Collider& colliderAlpha,
                                                const Transform& transformBeta, const Collider& colliderBeta) -> std::pair<Vec2, Vec2>;

  class GJK final
  {
  public:
    struct SimplexData {
      static constexpr u32 MAX_SIMPLEX_POINTS = 3;
      static constexpr u32 FIRST_POINT_INDEX = 0;
      static constexpr u32 SECOND_POINT_INDEX = 1;
      static constexpr u32 THIRD_POINT_INDEX = 2;
      static constexpr u32 LINE_SEGMENT_SIZE = 2;
      static constexpr u32 TRIANGLE_SIZE = 3;

      [[nodiscard]] auto size() const -> u32 { return m_size; }
      [[nodiscard]] auto points() const -> std::array<Vec2, MAX_SIMPLEX_POINTS> { return m_points; }

      auto add(const Vec2& point) -> void;
      [[nodiscard]] auto containsOrigin(Vec2& direction) const -> bool;

    private:
      std::array<Vec2, MAX_SIMPLEX_POINTS> m_points{};
      u32 m_size = 0;
    };

    [[nodiscard]] static auto test(const Transform& transformAlpha, const Collider& colliderAlpha, const Transform& transformBeta,
                                   const Collider& colliderBeta) -> std::pair<bool, SimplexData>;

    [[nodiscard]] static auto distance(const Transform& transformAlpha, const Collider& colliderAlpha, const Transform& transformBeta,
                                       const Collider& colliderBeta) -> std::pair<f32, std::pair<Vec2, Vec2>>;

  private:
    [[nodiscard]] static auto support(const Transform& transform, const Collider& collider, const Vec2& direction) -> Vec2;

    [[nodiscard]] static auto tripleProduct(const Vec2& vectorAlpha, const Vec2& vectorBeta, const Vec2& vectorGamma) -> Vec2;
  };

  class EPA final
  {
  public:
    [[nodiscard]] static auto calculatePenetration(const GJK::SimplexData& simplex, const Transform& transformAlpha,
                                                   const Collider& colliderAlpha, const Transform& transformBeta,
                                                   const Collider& colliderBeta) -> std::pair<f32, Vec2>;
  };
};

EGE_NAMESPACE_END