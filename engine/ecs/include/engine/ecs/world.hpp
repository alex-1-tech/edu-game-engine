#pragma once

/**
 * @file    engine/ecs/world.hpp
 * @brief   World class for ECS
 * @author  alex-1-tech
 * @date    2026
 */

#include <memory>
#include <vector>

#include "engine/core/logging.hpp"
#include "engine/ecs/entity.hpp"
#include "engine/ecs/world/component_manager.hpp"
#include "engine/ecs/world/entity_manager.hpp"
#include "engine/ecs/world/query_manager.hpp"
#include "engine/ecs/world/world_fwd.hpp"

EGE_NAMESPACE_BEGIN

class ISystem;
class World
{
public:
  World() = default;
  ~World() = default;

  World(const World&) = delete;
  auto operator=(const World&) -> World& = delete;

  World(World&&) = delete;
  auto operator=(World&&) -> World& = delete;

  /// Entity methods
  /// @{
#define mem m_entity_manager
  auto createEntity() -> Entity { return mem.createEntity(); }
  void destroyEntity(Entity entity) { mem.destroyEntity(entity); }
  template<typename T, typename... Args> auto addComponent(Entity entity, Args&&... args) -> T*;
  [[nodiscard]] auto getEntityCount() const -> usize { return mem.getEntityCount(); }
#undef mcm
  /// }@

  /// Component methods
  /// @{
#define mcm m_component_manager
  template<typename T> auto removeComponent(Entity entity) -> bool;
  template<typename T> auto getComponent(Entity entity) -> T* { return mcm.getComponent<T>(entity); }
  template<typename T> auto hasComponent(Entity entity) const -> bool { return mcm.hasComponent<T>(entity); }
#undef mcm
  /// }@

  /// Query methods
  /// @{
#define mqm m_query_manager
#define Co Components
  [[nodiscard]] auto query() -> QueryManager& { return mqm; }
  [[nodiscard]] auto query() const -> const QueryManager& { return mqm; }
  template<typename... Co> [[nodiscard]] auto queryEntities() -> std::vector<Entity> { return mqm.getEntities<Co...>(); }
  template<typename... Co> [[nodiscard]] auto queryEntitiesWith() -> std::vector<Entity> { return mqm.getEntitiesWith<Co...>(); }
  template<typename... Co, typename Func> void forEach(Func&& func) { mqm.forEach<Co...>(std::forward<Func>(func)); }
  template<typename... Co> [[nodiscard]] auto hasAllComponents(Entity entity) const -> bool { return mqm.hasAll<Co...>(entity); }
  [[nodiscard]] auto getComponentMask(Entity entity) const -> ComponentMask { return mqm.getComponentMask(entity); }
#undef mqm
#undef Co
  /// }@

  /// System methods
  /// @{
  template<typename T, typename... Args> auto registerSystem(Args&&... args) -> T*;
  void updateSystems(float deltaTime);
  /// }@

private:
  EntityManager m_entity_manager;
  ComponentManager m_component_manager;
  QueryManager m_query_manager{m_entity_manager, m_component_manager};
  std::vector<std::unique_ptr<ISystem>> m_systems;

  template<typename T> auto getComponentTypeId() -> u32 { return m_component_manager.getComponentTypeId<T>(); }
  auto getAllEntities() -> std::vector<Entity>;
  template<typename... Components> auto buildQueryMask() -> ComponentMask;
  void collectEntitiesWithMask(std::vector<Entity>& result, ComponentMask queryMask);
  auto hasAllComponentsCheck(Entity entity, ComponentMask queryMask) -> bool;
};


template<typename T, typename... Args> auto World::addComponent(Entity entity, Args&&... args) -> T*
{
  if (!m_entity_manager.isEntityValid(entity)) {
    EGE_ERROR("Cannot add component to invalid entity: {}", entity.toString());
    return nullptr;
  }

  const u32 componentId = m_component_manager.getComponentTypeId<T>();
  if (componentId == INVALID_COMPONENT_TYPE) {
    return nullptr;
  }

  T* component = m_component_manager.addComponent<T>(entity, std::forward<Args>(args)...);
  if (component) {
    auto& record = m_entity_manager.getEntityRecord(entity.getId());
    record.mask |= (BIT_MASK_0 << componentId);
  }

  return component;
}

template<typename T> auto World::removeComponent(Entity entity) -> bool
{
  if (!m_entity_manager.isEntityValid(entity)) {
    return false;
  }

  const u32 componentId = m_component_manager.getComponentTypeId<T>();
  if (componentId == INVALID_COMPONENT_TYPE) {
    return false;
  }

  bool removed = m_component_manager.removeComponent<T>(entity);
  if (removed) {
    auto& record = m_entity_manager.getEntityRecord(entity.getId());
    record.mask &= ~(BIT_MASK_0 << componentId);
  }
  return removed;
}


template<typename T, typename... Args> auto World::registerSystem(Args&&... args) -> T*
{
  auto system = std::make_unique<T>(std::forward<Args>(args)...);
  T* ptr = system.get();
  m_systems.push_back(std::move(system));

  EGE_INFO("Registered system: {}", ptr->getName());
  return ptr;
}

template<typename... Components> auto World::buildQueryMask() -> ComponentMask
{
  ComponentMask queryMask = 0;
  ((queryMask |= (BIT_MASK_0 << getComponentTypeId<Components>())), ...);
  return queryMask;
}
inline void World::collectEntitiesWithMask(std::vector<Entity>& result, ComponentMask queryMask)
{
  for (usize index = 0; index < m_entity_manager.getCapacity(); ++index) {
    const auto& record = m_entity_manager.getEntityRecord(EntityId(index));
    if (!record.alive) {
      continue;
    }

    if ((record.mask & queryMask) != queryMask) {
      continue;
    }

    Entity entity(EntityId(static_cast<u32>(index)), EntityGeneration(record.generation));

    if constexpr (ENABLE_EXTRA_CHECKS != 0U) {
      if (!hasAllComponentsCheck(entity, queryMask)) {
        continue;
      }
    }

    result.push_back(entity);
  }
}

inline auto World::hasAllComponentsCheck(Entity entity, ComponentMask queryMask) -> bool
{
  u32 bitPosition = 0;
  ComponentMask mask = queryMask;

  const auto& bit_to_type = m_component_manager.getBitToTypeMap();
  const auto& component_pools = m_component_manager.getComponentPools();

  while (mask != 0) {
    if ((mask & BIT_MASK_0) != 0) {
      auto bitIt = bit_to_type.find(bitPosition);
      if (bitIt != bit_to_type.end()) {
        auto poolIt = component_pools.find(bitIt->second);
        if (poolIt != component_pools.end()) {
          if (!poolIt->second->hasComponent(entity)) {
            return false;
          }
        } else {
          return false;
        }
      } else {
        return false;
      }
    }

    mask >>= ONE;
    ++bitPosition;
  }

  return true;
}

EGE_NAMESPACE_END
