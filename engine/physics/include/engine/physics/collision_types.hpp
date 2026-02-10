#pragma once

/**
 * @file    collision_types.hpp
 * @brief   Broad phase collision detection system
 * @author  alex-1-tech
 * @date    2026
 */

#include <fmt/format.h>

#include <unordered_map>

#include "engine/core/math.hpp"
#include "engine/ecs/entity.hpp"
#include "engine/ecs/world.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/graphics/types.hpp"

EGE_NAMESPACE_BEGIN

// Collision test result
struct CollisionResult {
  bool collides = false;
  Entity entityA;
  Entity entityB;

  // Collision geometry
  Vec2 normal;           // Collision normal (from A to B)
  f32 depth = 0.0F;      // Penetration depth
  Vec2 contactPoint;     // Contact point (world space)
  Vec2 relativeVelocity; // Relative velocity at contact point

  // Contacts (for stability)
  struct ContactData {
    Vec2 point;
    Vec2 rA; // Vector from center of mass A to contact point
    Vec2 rB; // Vector from center of mass B to contact point
    f32 normalImpulse = 0.0F;
    f32 tangentImpulse = 0.0F;
  };

  std::vector<ContactData> contacts; // Multiple contact points

  // Collision mode
  enum class Mode : u8 {
    DISCRETE,   // Discrete detection
    CONTINUOUS, // Continuous (CCD for fast objects)
    TRIGGER     // Event only
  };

  Mode mode = Mode::DISCRETE;
};

struct BroadPhaseData {
  Entity entityA;
  Entity entityB;
  Rect aabbA;
  Rect aabbB;
  u32 layerA{};
  u32 layerB{};

  // Optimization: cached data
  mutable f32 cachedPriority = 0.0F; // For priority queue
};

class BroadPhase final
{
public:
  enum class Algorithm : u8 {
    BRUTE_FORCE,     // All vs all (O(n²))
    SWEEP_AND_PRUNE, // Sort and sweep
    SPATIAL_HASH,    // Spatial hash grid
    QUADTREE,        // Quadtree
    BVH              // Bounding Volume Hierarchy
  };

  explicit BroadPhase(Algorithm algo = Algorithm::SPATIAL_HASH)
      : m_data{.algorithm = algo}
  {
  }

  void update(const std::vector<Entity>& entities, World& world, f32 cellSize = ONE_F)
  {
    m_data.cellSize = cellSize;

    switch (m_data.algorithm) {
      case Algorithm::SPATIAL_HASH: updateSpatialHash(entities, world); break;
      case Algorithm::SWEEP_AND_PRUNE: updateSweepAndPrune(entities, world); break;
      case Algorithm::BRUTE_FORCE: updateBruteForce(entities, world); break;
      case Algorithm::QUADTREE: updateQuadtree(entities, world); break;
      case Algorithm::BVH: updateBVH(entities, world); break;
    }
  }
  [[nodiscard]] auto shouldTest() const -> bool
  {
    return (m_phaseData.layerA & m_phaseData.layerB) != 0; // Check layer masks
  }

  auto operator<(const BroadPhaseData& other) const -> bool { return m_phaseData.cachedPriority < other.cachedPriority; }
  [[nodiscard]] auto getPotentialCollisions() const -> std::vector<BroadPhaseData> { return m_data.potentialCollisions; }

  // Visualization for debugging
  void debugDraw(Renderer* renderer) const;

  void setAlgorithm(Algorithm algo) { m_data.algorithm = algo; }
  void setCellSize(f32 size) { m_data.cellSize = size; }

  [[nodiscard]] auto toString() const -> String
  {
    const char* algoStr = "Unknown";
    switch (m_data.algorithm) {
      case Algorithm::BRUTE_FORCE: algoStr = "Brute Force"; break;
      case Algorithm::SWEEP_AND_PRUNE: algoStr = "Sweep and Prune"; break;
      case Algorithm::SPATIAL_HASH: algoStr = "Spatial Hash"; break;
      case Algorithm::QUADTREE: algoStr = "Quadtree"; break;
      case Algorithm::BVH: algoStr = "BVH"; break;
    }

    return fmt::format("BroadPhase(algorithm={}, cellSize={:.2f}, potentialPairs={})", algoStr, m_data.cellSize,
                       m_data.potentialCollisions.size());
  }

private:
  struct SpatialCellData {
    std::vector<Entity> entities;
    Rect bounds;
  };

  struct SweepEntryData {
    Entity entity;
    f32 value{};
    bool isMin{};
    u32 index{};
  };

  struct BroadPhaseInternalData {
    Algorithm algorithm = Algorithm::SPATIAL_HASH;
    f32 cellSize = ONE_F;

    // For Spatial Hash
    std::unordered_map<i32, std::unordered_map<i32, SpatialCellData>> grid;

    // For Sweep and Prune
    std::vector<SweepEntryData> sweepListX;
    std::vector<SweepEntryData> sweepListY;

    // For Quadtree/BVH
    std::vector<BroadPhaseData> quadtreeResults;
    std::vector<BroadPhaseData> bvhResults;

    // Results
    std::vector<BroadPhaseData> potentialCollisions;
  };

  BroadPhaseInternalData m_data;
  BroadPhaseData m_phaseData;
  // Spatial Hash methods
  [[nodiscard]] auto hashPosition(const Vec2& pos) const -> std::pair<i32, i32>
  {
    return {static_cast<i32>(std::floor(pos.x / m_data.cellSize)), static_cast<i32>(std::floor(pos.y / m_data.cellSize))};
  }

  void updateSpatialHash(const std::vector<Entity>& entities, World& world);
  void updateSweepAndPrune(const std::vector<Entity>& entities, World& world);
  void updateBruteForce(const std::vector<Entity>& entities, World& world);
  void updateQuadtree(const std::vector<Entity>& entities, World& world);
  void updateBVH(const std::vector<Entity>& entities, World& world);

public:
  [[nodiscard]] auto algorithm() const -> Algorithm { return m_data.algorithm; }
  [[nodiscard]] auto cellSize() const -> f32 { return m_data.cellSize; }
  [[nodiscard]] auto data() const -> const BroadPhaseInternalData& { return m_data; }
};

EGE_NAMESPACE_END