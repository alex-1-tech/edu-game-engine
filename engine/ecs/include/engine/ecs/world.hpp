#pragma once

#include <boost/range/algorithm/find.hpp>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "engine/core/logging.hpp"
#include "engine/ecs/component_pool.hpp"
#include "engine/ecs/entity.hpp"

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

  auto createEntity() -> Entity;
  void destroyEntity(Entity entity);
  auto isEntityValid(Entity entity) const -> bool;
  auto getEntityCount() const -> usize { return m_entity_count; }

  template<typename T, typename... Args> auto addComponent(Entity entity, Args&&... args) -> T*;

  template<typename T> auto removeComponent(Entity entity) -> bool;

  template<typename T> auto getComponent(Entity entity) -> T*;

  template<typename T> auto getComponent(Entity entity) const -> const T*;

  template<typename T> auto hasComponent(Entity entity) const -> bool;

  template<typename T, typename... Args> auto registerSystem(Args&&... args) -> T*;

  void updateSystems(float deltaTime);

  template<typename... Components> auto queryEntities() -> std::vector<Entity>;

  template<typename... Components, typename Func> void forEach(Func&& func);

private:
  struct EntityRecord {
    u32 generation = INITIAL_ENTITY_GENERATION.get();
    bool alive = false;
    std::vector<std::type_index> component_types;
  };

  std::vector<EntityRecord> m_entity_records;
  std::vector<EntityId> m_free_list;
  EntityId m_next_entity_id = EntityId(FIRST_ENTITY_ID);
  usize m_entity_count = 0;

  struct ComponentPoolBase {
    ComponentPoolBase() = default;
    virtual ~ComponentPoolBase() = default;

    ComponentPoolBase(const ComponentPoolBase&) = delete;
    auto operator=(const ComponentPoolBase&) -> ComponentPoolBase& = delete;

    ComponentPoolBase(ComponentPoolBase&&) = delete;
    auto operator=(ComponentPoolBase&&) -> ComponentPoolBase& = delete;
    virtual auto removeComponent(Entity entity) -> bool = 0;
    [[nodiscard]] virtual auto getTypeName() const -> String = 0;
    [[nodiscard]] virtual auto getComponentCount() const -> usize = 0;
  };

  template<typename T> struct TypedComponentPool final : ComponentPoolBase {
  private:
    ComponentPool<T> m_pool;

  public:
    auto getPool() -> ComponentPool<T>& { return m_pool; }
    auto getPool() const -> const ComponentPool<T>& { return m_pool; }

    auto removeComponent(Entity entity) -> bool override { return m_pool.removeComponent(entity); }

    [[nodiscard]] auto getTypeName() const -> String override { return typeid(T).name(); }

    [[nodiscard]] auto getComponentCount() const -> usize override
    {
      return m_pool.getAllComponents().size();
    }
  };

  std::unordered_map<std::type_index, std::unique_ptr<ComponentPoolBase>> m_component_pools;
  std::vector<std::unique_ptr<ISystem>> m_systems;

  template<typename T> auto getOrCreatePool() -> ComponentPool<T>&;
};


template<typename T, typename... Args> auto World::addComponent(Entity entity, Args&&... args) -> T*
{
  if (!isEntityValid(entity)) {
    EGE_ERROR("Cannot add component to invalid entity: {}", entity.toString());
    return nullptr;
  }

  const auto idx = entity.getId().get();
  if (idx >= m_entity_records.size()) {
    EGE_ERROR("Entity id {} out of range (size {})", idx, m_entity_records.size());
    return nullptr;
  }

  auto& pool = getOrCreatePool<T>();
  T component(std::forward<Args>(args)...);

  auto type_idx = std::type_index(typeid(T));
  auto& record = m_entity_records[idx];

  if (boost::range::find(record.component_types, type_idx) == record.component_types.end()) {
    record.component_types.push_back(type_idx);
  }

  return pool.addComponent(entity, std::move(component));
}

template<typename T> auto World::removeComponent(Entity entity) -> bool
{
  if (!isEntityValid(entity)) {
    return false;
  }

  const auto idx = entity.getId().get();
  if (idx >= m_entity_records.size()) {
    return false;
  }

  auto map_it = m_component_pools.find(std::type_index(typeid(T)));
  if (map_it == m_component_pools.end()) {
    return false;
  }

  bool removed = map_it->second->removeComponent(entity);
  if (removed) {
    auto& record = m_entity_records[idx];
    auto type_idx = std::type_index(typeid(T));
    auto comp_it = boost::range::find(record.component_types, type_idx);
    if (comp_it != record.component_types.end()) {
      record.component_types.erase(comp_it);
    }
  }
  return removed;
}

template<typename T> auto World::getComponent(Entity entity) -> T*
{
  if (!isEntityValid(entity)) {
    return nullptr;
  }
  const auto idx = entity.getId().get();
  if (idx >= m_entity_records.size()) {
    return nullptr;
  }
  auto map_it = m_component_pools.find(std::type_index(typeid(T)));
  if (map_it == m_component_pools.end()) {
    return nullptr;
  }

  auto* typed_pool = static_cast<TypedComponentPool<T>*>(map_it->second.get());
  return typed_pool->getPool().getComponent(entity);
}

template<typename T> auto World::hasComponent(Entity entity) const -> bool
{
  if (!isEntityValid(entity)) {
    return false;
  }
  const auto idx = entity.getId().get();
  if (idx >= m_entity_records.size()) {
    return false;
  }
  auto map_it = m_component_pools.find(std::type_index(typeid(T)));
  if (map_it == m_component_pools.end()) {
    return false;
  }

  auto* typed_pool = static_cast<const TypedComponentPool<T>*>(map_it->second.get());

  return typed_pool->getPool().hasComponent(entity);
}

template<typename T> auto World::getOrCreatePool() -> ComponentPool<T>&
{
  auto type_idx = std::type_index(typeid(T));
  auto map_it = m_component_pools.find(type_idx);

  if (map_it == m_component_pools.end()) {
    auto pool = std::make_unique<TypedComponentPool<T>>();
    auto* pool_ptr = pool.get();
    m_component_pools.emplace(type_idx, std::move(pool));
    return pool_ptr->getPool();
  }

  return static_cast<TypedComponentPool<T>*>(map_it->second.get())->getPool();
}

template<typename T, typename... Args> auto World::registerSystem(Args&&... args) -> T*
{
  auto system = std::make_unique<T>(std::forward<Args>(args)...);
  T* ptr = system.get();
  m_systems.push_back(std::move(system));

  EGE_INFO("Registered system: {}", ptr->getName());
  return ptr;
}

template<typename... Components> auto World::queryEntities() -> std::vector<Entity>
{
  std::vector<Entity> result;

  for (usize index = 0; index < m_entity_records.size(); ++index) {
    const auto& record = m_entity_records[index];
    if (!record.alive) {
      continue;
    }

    Entity entity(EntityId(static_cast<u32>(index)), EntityGeneration(record.generation));

    bool has_all = true;
    auto checkComponent = [&](auto type_tag) {
      using T = typename decltype(type_tag)::type;
      if (!has_all) {
        return;
      }

      auto map_it = m_component_pools.find(std::type_index(typeid(T)));
      if (map_it == m_component_pools.end()) {
        has_all = false;
        return;
      }

      auto* typed_pool = static_cast<const TypedComponentPool<T>*>(map_it->second.get());
      if (!typed_pool->getPool().hasComponent(entity)) {
        has_all = false;
      }
    };

    (checkComponent.template operator()<Components>(), ...);

    if (has_all) {
      result.push_back(entity);
    }
  }

  return result;
}
template<typename... Components, typename Func> void World::forEach(Func&& func)
{
  auto entities = queryEntities<Components...>();
  for (Entity entity : entities) {
    if (!isEntityValid(entity)) {
      continue;
    }

    std::tuple<Components*...> comps{getComponent<Components>(entity)...};

    bool all_valid = true;
    std::apply([&all_valid](auto*... ptrs) { ((all_valid &= (ptrs != nullptr)), ...); }, comps);

    if (all_valid) {
      std::apply([&](auto*... ptrs) { std::forward<Func>(func)(entity, *ptrs...); }, comps);
    } else {
      EGE_WARN("Entity {} skipped in forEach: missing one or more components", entity.toString());
    }
  }
}


inline auto World::createEntity() -> Entity
{
  EntityId entityId;

  if (!m_free_list.empty()) {
    entityId = m_free_list.back();
    m_free_list.pop_back();

    auto& record = m_entity_records[entityId.get()];
    record.generation += ONE;
    record.alive = true;
    record.component_types.clear();
  } else {
    entityId = m_next_entity_id;
    m_next_entity_id = EntityId(m_next_entity_id.get() + ONE);

    if (entityId.get() >= m_entity_records.size()) {
      m_entity_records.resize(entityId.get() + ONE);
    }

    auto& record = m_entity_records[entityId.get()];
    record.generation = INITIAL_ENTITY_GENERATION.get();
    record.alive = true;
    record.component_types.clear();
  }

  ++m_entity_count;
  return {entityId, EntityGeneration(m_entity_records[entityId.get()].generation)};
}

inline void World::destroyEntity(Entity entity)
{
  if (!isEntityValid(entity)) {
    return;
  }

  const auto idx = entity.getId().get();
  if (idx >= m_entity_records.size()) {
    return;
  }

  auto& record = m_entity_records[idx];
  for (const auto& type_idx : record.component_types) {
    auto iterator = m_component_pools.find(type_idx);
    if (iterator != m_component_pools.end()) {
      iterator->second->removeComponent(entity);
    }
  }

  record.component_types.clear();
  record.alive = false;
  m_free_list.push_back(entity.getId());
  --m_entity_count;
}

inline auto World::isEntityValid(Entity entity) const -> bool
{
  const auto idx = entity.getId().get();
  if (idx >= m_entity_records.size()) {
    return false;
  }
  const auto& record = m_entity_records[idx];
  return record.alive && record.generation == entity.getGeneration().get();
}


EGE_NAMESPACE_END
