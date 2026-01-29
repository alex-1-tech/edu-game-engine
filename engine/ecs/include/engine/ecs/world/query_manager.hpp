#pragma once

/**
 * @file    engine/ecs/world/query_manager.hpp
 * @brief   Query manager for ECS world
 * @author  alex-1-tech
 * @date    2026
 */

#include <optional>
#include <tuple>
#include <vector>

#include "engine/core/logging.hpp"
#include "engine/ecs/entity.hpp"
#include "engine/ecs/world/component_manager.hpp"
#include "engine/ecs/world/entity_manager.hpp"

EGE_NAMESPACE_BEGIN

class QueryManager
{
public:
  QueryManager(EntityManager& entityManager, ComponentManager& componentManager)
      : m_entity_manager(entityManager)
      , m_component_manager(componentManager)
  {
  }
  QueryManager(const QueryManager&) = delete;
  QueryManager(QueryManager&&) = delete;
  auto operator=(const QueryManager&) -> QueryManager& = delete;
  auto operator=(QueryManager&&) -> QueryManager& = delete;
  ~QueryManager() = default;

  [[nodiscard]] auto getAllEntities() -> std::vector<Entity>;

  template<typename... Components> [[nodiscard]] auto getEntities() -> std::vector<Entity>;

  template<typename... Components> [[nodiscard]] auto getEntitiesWith() -> std::vector<Entity>;

  template<typename... Components> [[nodiscard]] auto getEntitiesWithAny() -> std::vector<Entity>;

  template<typename... Components> [[nodiscard]] auto getEntitiesWithout() -> std::vector<Entity>;

  template<typename... Components, typename Func> void forEach(Func&& func);

  template<typename... Components, typename Func, typename Predicate>
  void forEachIf(Func&& func, Predicate&& predicate);

  template<typename... Components> [[nodiscard]] auto hasAll(Entity entity) const -> bool;

  template<typename... Components> [[nodiscard]] auto hasAny(Entity entity) const -> bool;

  template<typename... Components> [[nodiscard]] auto hasNone(Entity entity) const -> bool;

  [[nodiscard]] auto getComponentMask(Entity entity) const -> ComponentMask;

  template<typename... Components> [[nodiscard]] auto count() -> usize;

  template<typename... Components> [[nodiscard]] auto first() -> std::optional<Entity>;

  template<typename... Components>
  [[nodiscard]] auto getComponents(Entity entity) -> std::tuple<Components*...>;

  template<typename... Components>
  [[nodiscard]] auto getComponents(Entity entity) const -> std::tuple<const Components*...>;

private:
  EntityManager& m_entity_manager;
  ComponentManager& m_component_manager;

  template<typename... Components> [[nodiscard]] auto buildComponentMask() -> ComponentMask;

  void collectEntitiesWithMask(std::vector<Entity>& result, ComponentMask requiredMask);
  void collectEntitiesWithAnyMask(std::vector<Entity>& result, ComponentMask anyMask);
  void collectEntitiesWithoutMask(std::vector<Entity>& result, ComponentMask excludedMask);

  [[nodiscard]] auto validateEntity(Entity entity) const -> bool;
  [[nodiscard]] auto getEntityRecord(Entity entity) const -> const EntityRecord*;
};

template<typename... Components> auto QueryManager::getEntities() -> std::vector<Entity>
{
  if constexpr (sizeof...(Components) == 0) {
    return getAllEntities();
  }
  return getEntitiesWith<Components...>();
}

template<typename... Components> auto QueryManager::getEntitiesWith() -> std::vector<Entity>
{
  std::vector<Entity> result;

  if constexpr (sizeof...(Components) == 0) {
    return getAllEntities();
  }

  ComponentMask requiredMask = buildComponentMask<Components...>();
  if (requiredMask == 0) {
    return result;
  }

  collectEntitiesWithMask(result, requiredMask);
  return result;
}

template<typename... Components> auto QueryManager::getEntitiesWithAny() -> std::vector<Entity>
{
  std::vector<Entity> result;

  ComponentMask anyMask = buildComponentMask<Components...>();
  if (anyMask == 0) {
    return result;
  }

  collectEntitiesWithAnyMask(result, anyMask);
  return result;
}

template<typename... Components> auto QueryManager::getEntitiesWithout() -> std::vector<Entity>
{
  std::vector<Entity> result;

  ComponentMask excludedMask = buildComponentMask<Components...>();
  collectEntitiesWithoutMask(result, excludedMask);
  return result;
}

template<typename... Components, typename Func> void QueryManager::forEach(Func&& func)
{
  static_assert(sizeof...(Components) > 0, "forEach requires at least one component type");

  auto entities = getEntitiesWith<Components...>();

  for (Entity entity : entities) {
    if (!validateEntity(entity)) {
      continue;
    }

    std::tuple<Components*...> components = getComponents<Components...>(entity);

    bool allValid = true;
    std::apply([&allValid](auto*... ptrs) { ((allValid = allValid && (ptrs != nullptr)), ...); }, components);

    if (allValid) {
      std::apply([&](auto*... ptrs) { std::forward<Func>(func)(entity, *ptrs...); }, components);
    } else {
      EGE_WARN("Entity {} skipped in forEach: missing one or more components", entity.toString());
    }
  }
}

template<typename... Components, typename Func, typename Predicate>
void QueryManager::forEachIf(Func&& func, Predicate&& predicate)
{
  auto entities = getEntitiesWith<Components...>();

  for (Entity entity : entities) {
    if (!validateEntity(entity)) {
      continue;
    }

    std::tuple<Components*...> components = getComponents<Components...>(entity);

    bool allValid = true;
    std::apply([&allValid](auto*... ptrs) { ((allValid = allValid && (ptrs != nullptr)), ...); }, components);

    if (!allValid) {
      continue;
    }

    bool shouldProcess = std::apply(
        [&](auto*... ptrs) { return std::forward<Predicate>(predicate)(entity, *ptrs...); }, components);

    if (shouldProcess) {
      std::apply([&](auto*... ptrs) { std::forward<Func>(func)(entity, *ptrs...); }, components);
    }
  }
}

template<typename... Components> auto QueryManager::hasAll(Entity entity) const -> bool
{
  if (!validateEntity(entity)) {
    return false;
  }

  const EntityRecord* record = getEntityRecord(entity);
  if (!record) {
    return false;
  }

  ComponentMask requiredMask = buildComponentMask<Components...>();
  return (record->mask & requiredMask) == requiredMask;
}

template<typename... Components> auto QueryManager::hasAny(Entity entity) const -> bool
{
  if (!validateEntity(entity)) {
    return false;
  }

  const EntityRecord* record = getEntityRecord(entity);
  if (!record) {
    return false;
  }

  ComponentMask anyMask = buildComponentMask<Components...>();
  return (record->mask & anyMask) != 0;
}

template<typename... Components> auto QueryManager::hasNone(Entity entity) const -> bool
{
  if (!validateEntity(entity)) {
    return true;
  }

  const EntityRecord* record = getEntityRecord(entity);
  if (!record) {
    return true;
  }

  ComponentMask excludedMask = buildComponentMask<Components...>();
  return (record->mask & excludedMask) == 0;
}

template<typename... Components> auto QueryManager::count() -> usize
{
  return getEntitiesWith<Components...>().size();
}

template<typename... Components> auto QueryManager::first() -> std::optional<Entity>
{
  auto entities = getEntitiesWith<Components...>();
  if (entities.empty()) {
    return std::nullopt;
  }
  return entities.front();
}

template<typename... Components> auto QueryManager::getComponents(Entity entity) -> std::tuple<Components*...>
{
  return std::make_tuple(m_component_manager.getComponent<Components>(entity)...);
}

template<typename... Components>
auto QueryManager::getComponents(Entity entity) const -> std::tuple<const Components*...>
{
  return std::make_tuple(m_component_manager.getComponent<Components>(entity)...);
}

template<typename... Components> auto QueryManager::buildComponentMask() -> ComponentMask
{
  ComponentMask mask = 0;

  ((mask |= (BIT_MASK_0 << m_component_manager.getComponentTypeId<Components>())), ...);

  return mask;
}

inline auto QueryManager::getAllEntities() -> std::vector<Entity>
{
  return m_entity_manager.getAllEntities();
}

inline void QueryManager::collectEntitiesWithMask(std::vector<Entity>& result, ComponentMask requiredMask)
{
  for (usize index = 0; index < m_entity_manager.getCapacity(); ++index) {
    const auto& record = m_entity_manager.getEntityRecord(EntityId(static_cast<u32>(index)));
    if (!record.alive) {
      continue;
    }

    if ((record.mask & requiredMask) == requiredMask) {
      result.emplace_back(EntityId(static_cast<u32>(index)), EntityGeneration(record.generation));
    }
  }
}

inline void QueryManager::collectEntitiesWithAnyMask(std::vector<Entity>& result, ComponentMask anyMask)
{
  for (usize index = 0; index < m_entity_manager.getCapacity(); ++index) {
    const auto& record = m_entity_manager.getEntityRecord(EntityId(static_cast<u32>(index)));
    if (!record.alive) {
      continue;
    }

    if ((record.mask & anyMask) != 0) {
      result.emplace_back(EntityId(static_cast<u32>(index)), EntityGeneration(record.generation));
    }
  }
}

inline void QueryManager::collectEntitiesWithoutMask(std::vector<Entity>& result, ComponentMask excludedMask)
{
  for (usize index = 0; index < m_entity_manager.getCapacity(); ++index) {
    const auto& record = m_entity_manager.getEntityRecord(EntityId(static_cast<u32>(index)));
    if (!record.alive) {
      continue;
    }

    if ((record.mask & excludedMask) == 0) {
      result.emplace_back(EntityId(static_cast<u32>(index)), EntityGeneration(record.generation));
    }
  }
}

inline auto QueryManager::validateEntity(Entity entity) const -> bool
{
  return m_entity_manager.isEntityValid(entity);
}

inline auto QueryManager::getEntityRecord(Entity entity) const -> const EntityRecord*
{
  if (!validateEntity(entity)) {
    return nullptr;
  }

  return &m_entity_manager.getEntityRecord(entity.getId());
}

inline auto QueryManager::getComponentMask(Entity entity) const -> ComponentMask
{
  const EntityRecord* record = getEntityRecord(entity);
  return (record != nullptr) ? record->mask : 0;
}

EGE_NAMESPACE_END