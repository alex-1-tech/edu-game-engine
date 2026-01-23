#include "engine/ecs/world.hpp"

#include "engine/core/base.hpp"
#include "engine/ecs/system.hpp"


EGE_NAMESPACE_BEGIN


void World::updateSystems(float deltaTime)
{
  for (auto& system : m_systems) {
    system->update(*this, deltaTime);
  }
}
EGE_NAMESPACE_END