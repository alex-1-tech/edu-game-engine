#pragma once

/**
 * @file    engine/ecs/world/world_fwd.hpp
 * @brief   Forward declarations for ECS world
 * @author  alex-1-tech
 * @date    2026
 */

#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN

using ComponentMask = u64;
static constexpr u32 MAX_COMPONENT_TYPES = 64;
static constexpr u32 FIRST_ENTITY_ID = 1;
inline constexpr uint64_t BIT_MASK_0 = 1ULL;
static constexpr u32 INVALID_COMPONENT_TYPE = static_cast<u32>(-1);

class ISystem;
class World;

EGE_NAMESPACE_END