#pragma once

#include <string>

#include "engine/ecs/entity.hpp"
#include "engine/ecs/world.hpp"

EGE_NAMESPACE_BEGIN


class ISystem
{
public:
  ISystem() = default;
  virtual ~ISystem() = default;

  ISystem(const ISystem&) = delete;
  auto operator=(const ISystem&) -> ISystem& = delete;
  ISystem(ISystem&&) = delete;
  auto operator=(ISystem&&) -> ISystem& = delete;

  virtual void update(World& world, float deltaTime) = 0;
  [[nodiscard]] virtual auto getName() const -> std::string = 0;

  [[nodiscard]] virtual auto explain() const -> std::string
  {
    return getName() + " - processes entities with specific components";
  }

  virtual void onEnabled() {}
  virtual void onDisabled() {}

  [[nodiscard]] auto isEnabled() const -> bool { return m_enabled; }

  void setEnabled(bool enabled)
  {
    if (m_enabled != enabled) {
      m_enabled = enabled;
      enabled ? onEnabled() : onDisabled();
    }
  }

private:
  bool m_enabled = true;
};

template<typename... RequiredComponents> class System : public ISystem
{
public:
  void update(World& world, float deltaTime) override
  {
    if (!isEnabled()) {
      return;
    }

    world.forEach<RequiredComponents...>(
        [this, &world, deltaTime](Entity entity, RequiredComponents&... components) {
          process(entity, world, components..., deltaTime);
        });
  }

protected:
  virtual void process(Entity entity, World& world, RequiredComponents&... components, float deltaTime) = 0;
};

EGE_NAMESPACE_END
