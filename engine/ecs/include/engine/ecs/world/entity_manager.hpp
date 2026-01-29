#pragma once

/**
 * @file    engine/ecs/world/entity_manager.hpp
 * @brief   Entity manager for ECS world
 * @author  alex-1-tech
 * @date    2026
 */

#include <vector>

#include "engine/ecs/entity.hpp"
#include "engine/ecs/world/world_fwd.hpp"

EGE_NAMESPACE_BEGIN

class World;

struct EntityRecord {
  u32 generation = INITIAL_ENTITY_GENERATION.get();
  bool alive = false;
  ComponentMask mask = 0;
};

class EntityManager
{
public:
  EntityManager() = default;
  ~EntityManager() = default;

  EntityManager(const EntityManager&) = default;
  EntityManager(EntityManager&&) = default;
  auto operator=(const EntityManager&) -> EntityManager& = default;
  auto operator=(EntityManager&&) -> EntityManager& = default;


  auto createEntity() -> Entity;
  void destroyEntity(Entity entity);
  [[nodiscard]] auto isEntityValid(Entity entity) const -> bool;
  [[nodiscard]] auto getEntityCount() const -> usize { return m_entity_count; }
  [[nodiscard]] auto getCapacity() const -> usize { return m_entity_records.size(); }

  auto getEntityRecord(EntityId identificator) -> EntityRecord& { return m_entity_records[identificator.get()]; }

  [[nodiscard]] auto getEntityRecord(EntityId identificator) const -> const EntityRecord& { return m_entity_records[identificator.get()]; }

  auto getAllEntities() -> std::vector<Entity>;

private:
  std::vector<EntityRecord> m_entity_records;
  std::vector<EntityId> m_free_list;
  EntityId m_next_entity_id = EntityId(FIRST_ENTITY_ID);
  usize m_entity_count = 0;
};

inline auto EntityManager::createEntity() -> Entity
{
  EntityId entityId;

  if (!m_free_list.empty()) {
    entityId = m_free_list.back();
    m_free_list.pop_back();

    auto& record = m_entity_records[entityId.get()];
    record.generation += ONE;
    record.alive = true;
    record.mask = 0;
  } else {
    entityId = m_next_entity_id;
    m_next_entity_id = EntityId(m_next_entity_id.get() + ONE);

    if (entityId.get() >= m_entity_records.size()) {
      m_entity_records.resize(entityId.get() + ONE);
    }

    auto& record = m_entity_records[entityId.get()];
    record.generation = INITIAL_ENTITY_GENERATION.get();
    record.alive = true;
    record.mask = 0;
  }

  ++m_entity_count;
  return {entityId, EntityGeneration(m_entity_records[entityId.get()].generation)};
}

inline void EntityManager::destroyEntity(Entity entity)
{
  const auto idx = static_cast<u32>(entity.getId());
  if (idx >= m_entity_records.size()) {
    return;
  }

  auto& record = m_entity_records[idx];
  if (!record.alive || record.generation != entity.getGeneration().get()) {
    return;
  }

  record.alive = false;
  m_free_list.push_back(entity.getId());
  --m_entity_count;
}

inline auto EntityManager::isEntityValid(Entity entity) const -> bool
{
  const auto idx = static_cast<u32>(entity.getId());
  if (idx >= m_entity_records.size()) {
    return false;
  }
  const auto& record = m_entity_records[idx];
  return record.alive && record.generation == entity.getGeneration().get();
}

inline auto EntityManager::getAllEntities() -> std::vector<Entity>
{
  std::vector<Entity> allEntities;
  allEntities.reserve(m_entity_count);

  for (usize index = 0; index < m_entity_records.size(); ++index) {
    const auto& record = m_entity_records[index];
    if (record.alive) {
      allEntities.emplace_back(EntityId(static_cast<u32>(index)), EntityGeneration(record.generation));
    }
  }
  return allEntities;
}

EGE_NAMESPACE_END
