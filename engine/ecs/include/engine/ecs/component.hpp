#pragma once

/**
 * @file    engine/ecs/component.hpp
 * @brief   Base component interface for ECS
 * @author  alex-1-tech
 * @date    2026
 */

#include <fmt/format.h>

#include <memory>
#include <typeindex>
#include <typeinfo>

#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN

class IComponent
{
public:
  IComponent() = default;
  virtual ~IComponent() = default;

  IComponent(const IComponent&) = default;
  IComponent(IComponent&&) = default;
  auto operator=(const IComponent&) -> IComponent& = default;
  auto operator=(IComponent&&) -> IComponent& = default;

  [[nodiscard]] virtual auto getTypeIndex() const -> std::type_index = 0;
  [[nodiscard]] virtual auto getTypeName() const -> const char* = 0;

  [[nodiscard]] virtual auto toString() const -> String = 0;
  [[nodiscard]] virtual auto explain() const -> String = 0;

  [[nodiscard]] virtual auto clone() const -> Unique<IComponent> = 0;
};

template<typename T> class Component : public IComponent
{
public:
  [[nodiscard]] auto getTypeIndex() const -> std::type_index override { return typeid(T); }
  [[nodiscard]] auto getTypeName() const -> const char* override { return typeid(T).name(); }

  [[nodiscard]] auto clone() const -> Unique<IComponent> override
  {
    return std::make_unique<T>(static_cast<const T&>(*this));
  }

  [[nodiscard]] auto toString() const -> String override
  {
    return fmt::format("Component[{}]", getTypeName());
  }

  [[nodiscard]] auto explain() const -> String override
  {
    return fmt::format("Component Type: {}\n"
                       "Size: {} bytes\n"
                       "Alignment: {} bytes",
                       getTypeName(), sizeof(T), alignof(T));
  }
};
EGE_NAMESPACE_END