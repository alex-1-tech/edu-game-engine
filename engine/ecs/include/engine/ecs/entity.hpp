#pragma once

/**
 * @file    engine/ecs/entity.hpp
 * @brief   Entity definition for ECS
 * @author  alex-1-tech
 * @date    2026
 */

#include <fmt/format.h>

#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN

static constexpr u32 BITS_PER_COMPONENT = 32;
static constexpr u32 MAX_ENTITIES = 10000; ///< Maximum number of entities
static constexpr EntityId INVALID_ENTITY_ID = EntityId(0);
static constexpr EntityGeneration INITIAL_ENTITY_GENERATION = EntityGeneration(1);

class Entity
{
public:
  Entity() = default;
  Entity(EntityId entity_id, EntityGeneration gen = INITIAL_ENTITY_GENERATION)
      : m_id(entity_id)
      , m_generation(gen)
  {
  }

  [[nodiscard]] auto isValid() const -> bool { return m_id != INVALID_ENTITY_ID; }
  [[nodiscard]] auto getId() const -> EntityId { return m_id; }
  [[nodiscard]] auto getGeneration() const -> EntityGeneration { return m_generation; }

  auto operator==(const Entity& other) const -> bool { return m_id == other.m_id && m_generation == other.m_generation; }

  auto operator!=(const Entity& other) const -> bool { return !(*this == other); }

  [[nodiscard]] auto toString() const -> String { return fmt::format("Entity({}.{})", m_id, m_generation); }

private:
  EntityId m_id = INVALID_ENTITY_ID;
  EntityGeneration m_generation = INITIAL_ENTITY_GENERATION;
};

struct EntityHash {
  auto operator()(const Entity& entity) const -> usize
  {
    return std::hash<u64>{}((static_cast<u64>(entity.getId()) << BITS_PER_COMPONENT) | entity.getGeneration().get());
  }
};

EGE_NAMESPACE_END

namespace fmt
{

template<> struct formatter<engine::EntityId> : formatter<engine::u32> {
  auto format(const engine::EntityId& value, fmt::format_context& ctx) { return formatter<engine::u32>::format(value.get(), ctx); }
};

template<> struct formatter<engine::EntityGeneration> : formatter<engine::u32> {
  auto format(const engine::EntityGeneration& value, fmt::format_context& ctx) { return formatter<engine::u32>::format(value.get(), ctx); }
};

} // namespace fmt