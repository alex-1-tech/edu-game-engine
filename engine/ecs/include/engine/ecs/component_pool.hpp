#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

#include "engine/ecs/component.hpp"
#include "engine/ecs/entity.hpp"

EGE_NAMESPACE_BEGIN

template<typename T> class ComponentPool
{
  static_assert(std::is_base_of_v<Component<T>, T>, "T must inherit Component<T>");

public:
  ComponentPool()
  {
    m_components.reserve(INITIAL_CAPACITY);
    m_entity_to_index.reserve(INITIAL_CAPACITY);
  }

  auto addComponent(Entity entity, T component) -> T*
  {
    if (hasComponent(entity)) {
      return getComponent(entity);
    }

    m_components.push_back(std::move(component));
    const usize index = m_components.size() - 1;
    m_entity_to_index[entity] = index;
    m_index_to_entity[index] = entity;

    return &m_components.back();
  }

  auto removeComponent(Entity entity) -> bool
  {
    auto iterator = m_entity_to_index.find(entity);
    if (iterator == m_entity_to_index.end()) {
      return false;
    }

    const usize indexToRemove = iterator->second;
    const usize lastIndex = m_components.size() - 1;

    if (indexToRemove != lastIndex) {
      m_components[indexToRemove] = std::move(m_components[lastIndex]);

      Entity lastEntity = m_index_to_entity[lastIndex];
      m_entity_to_index[lastEntity] = indexToRemove;
      m_index_to_entity[indexToRemove] = lastEntity;
    }

    m_components.pop_back();
    m_entity_to_index.erase(entity);
    m_index_to_entity.erase(lastIndex);

    return true;
  }

  auto getComponent(Entity entity) -> T*
  {
    auto iterator = m_entity_to_index.find(entity);
    if (iterator == m_entity_to_index.end()) {
      return nullptr;
    }
    return &m_components[iterator->second];
  }

  auto getComponent(Entity entity) const -> const T*
  {
    return const_cast<T*>(std::as_const(*this).getComponent(entity));
  }

  auto hasComponent(Entity entity) const -> bool
  {
    return m_entity_to_index.find(entity) != m_entity_to_index.end();
  }

  auto getAllComponents() -> std::vector<T>& { return m_components; }
  auto getAllComponents() const -> const std::vector<T>& { return m_components; }

  auto visualizeMemoryLayout() const -> String
  {
    return fmt::format("ComponentPool[{}]:\n"
                       "  Entities: {}\n"
                       "  Components: {}\n"
                       "  Memory: {} bytes\n"
                       "  Load Factor: {:.1f}%",
                       typeid(T).name(), m_entity_to_index.size(), m_components.size(),
                       m_components.capacity() * sizeof(T),
                       (m_components.size() * PERCENTAGE_MULTIPLIER) / m_components.capacity());
  }

  auto getAllEntities() const -> std::vector<Entity>
  {
    std::vector<Entity> entities;
    entities.reserve(m_entity_to_index.size());
    for (const auto& [entity, idx] : m_entity_to_index) {
      entities.push_back(entity);
    }
    return entities;
  }

private:
  std::vector<T> m_components;
  std::unordered_map<Entity, usize, EntityHash> m_entity_to_index;
  std::unordered_map<usize, Entity> m_index_to_entity;
};

EGE_NAMESPACE_END