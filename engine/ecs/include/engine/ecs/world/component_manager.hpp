#pragma once

/**
 * @file    engine/ecs/world/component_manager.hpp
 * @brief   Component manager for ECS world
 * @author  alex-1-tech
 * @date    2026
 */

#include <atomic>
#include <memory>
#include <mutex>
#include <typeindex>
#include <unordered_map>

#include "engine/core/logging.hpp"
#include "engine/ecs/component_pool.hpp"
#include "engine/ecs/world/world_fwd.hpp"

EGE_NAMESPACE_BEGIN

class World;


class ComponentPoolBase
{
public:
  ComponentPoolBase() = default;
  virtual ~ComponentPoolBase() = default;

  ComponentPoolBase(const ComponentPoolBase&) = default;
  ComponentPoolBase(ComponentPoolBase&&) = default;
  auto operator=(const ComponentPoolBase&) -> ComponentPoolBase& = default;
  auto operator=(ComponentPoolBase&&) -> ComponentPoolBase& = default;
  [[nodiscard]] virtual auto hasComponent(Entity entity) const -> bool = 0;
  virtual auto removeComponent(Entity entity) -> bool = 0;
  [[nodiscard]] virtual auto getTypeName() const -> String = 0;
  [[nodiscard]] virtual auto getComponentCount() const -> usize = 0;
};

template<typename T> class TypedComponentPool final : public ComponentPoolBase
{
private:
  ComponentPool<T> m_pool;

public:
  auto getPool() -> ComponentPool<T>& { return m_pool; }
  auto getPool() const -> const ComponentPool<T>& { return m_pool; }

  [[nodiscard]] auto hasComponent(Entity entity) const -> bool override { return m_pool.hasComponent(entity); }

  auto removeComponent(Entity entity) -> bool override { return m_pool.removeComponent(entity); }

  [[nodiscard]] auto getTypeName() const -> String override { return typeid(T).name(); }

  [[nodiscard]] auto getComponentCount() const -> usize override { return m_pool.getAllComponents().size(); }
};

class ComponentManager
{
public:
  ComponentManager() = default;

  auto getTypeToBitMap() const -> const std::unordered_map<std::type_index, u32>& { return m_type_to_bit; }

  auto getBitToTypeMap() const -> const std::unordered_map<u32, std::type_index>& { return m_bit_to_type; }

  auto getComponentPools() const -> const std::unordered_map<std::type_index, std::unique_ptr<ComponentPoolBase>>&
  {
    return m_component_pools;
  }

  template<typename T, typename... Args> auto addComponent(Entity entity, Args&&... args) -> T*
  {
    if (!entity.isValid()) {
      EGE_ERROR("Cannot add component to invalid entity");
      return nullptr;
    }

    auto& pool = getOrCreatePool<T>();
    T component(std::forward<Args>(args)...);

    const u32 componentId = getComponentTypeId<T>();
    return pool.addComponent(entity, std::move(component));
  }

  template<typename T> auto removeComponent(Entity entity) -> bool
  {
    if (!entity.isValid()) {
      return false;
    }

    auto map_it = m_component_pools.find(std::type_index(typeid(T)));
    if (map_it == m_component_pools.end()) {
      return false;
    }

    return map_it->second->removeComponent(entity);
  }

  template<typename T> auto getComponent(Entity entity) -> T*
  {
    if (!entity.isValid()) {
      return nullptr;
    }

    auto map_it = m_component_pools.find(std::type_index(typeid(T)));
    if (map_it == m_component_pools.end()) {
      return nullptr;
    }

    auto* typed_pool = static_cast<TypedComponentPool<T>*>(map_it->second.get());
    return typed_pool->getPool().getComponent(entity);
  }

  template<typename T> auto hasComponent(Entity entity) const -> bool
  {
    if (!entity.isValid()) {
      return false;
    }

    auto map_it = m_component_pools.find(std::type_index(typeid(T)));
    if (map_it == m_component_pools.end()) {
      return false;
    }

    auto* typed_pool = static_cast<const TypedComponentPool<T>*>(map_it->second.get());
    return typed_pool->getPool().hasComponent(entity);
  }

  void removeAllComponents(Entity entity, ComponentMask mask)
  {
    u32 bitPosition = 0;

    while (mask != 0) {
      if ((mask & BIT_MASK_0) != 0U) {
        auto bitIt = m_bit_to_type.find(bitPosition);
        if (bitIt != m_bit_to_type.end()) {
          auto poolIt = m_component_pools.find(bitIt->second);
          if (poolIt != m_component_pools.end()) {
            poolIt->second->removeComponent(entity);
          }
        }
      }
      mask >>= ONE;
      ++bitPosition;
    }
  }

  template<typename T> auto getComponentTypeId() -> u32;

private:
  std::unordered_map<std::type_index, std::unique_ptr<ComponentPoolBase>> m_component_pools;
  std::unordered_map<std::type_index, u32> m_type_to_bit;
  std::unordered_map<u32, std::type_index> m_bit_to_type;

  template<typename T> auto getOrCreatePool() -> ComponentPool<T>&;
  template<typename T> auto registerComponentType() -> u32;
};

template<typename T> auto ComponentManager::getOrCreatePool() -> ComponentPool<T>&
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

template<typename T> auto ComponentManager::registerComponentType() -> u32
{
  static std::atomic<u32> s_next_type_id{0};
  static std::mutex s_registration_mutex;

  std::lock_guard<std::mutex> lock(s_registration_mutex);

  auto type_idx = std::type_index(typeid(T));
  auto iterator = m_type_to_bit.find(type_idx);

  if (iterator != m_type_to_bit.end()) {
    return iterator->second;
  }

  const u32 new_id = s_next_type_id.fetch_add(1, std::memory_order_relaxed);

  if (new_id >= MAX_COMPONENT_TYPES) {
    EGE_ERROR("Too many component types! Maximum is {}", MAX_COMPONENT_TYPES);
    return INVALID_COMPONENT_TYPE;
  }

  m_type_to_bit.emplace(type_idx, new_id);
  m_bit_to_type.emplace(new_id, type_idx);

  return new_id;
}

template<typename T> auto ComponentManager::getComponentTypeId() -> u32
{
  static const u32 typeId = registerComponentType<T>();
  return typeId;
}

EGE_NAMESPACE_END