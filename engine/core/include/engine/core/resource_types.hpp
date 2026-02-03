#pragma once

/**
 * @file    resource_types.hpp
 * @brief   Basic types and interfaces
 * @author  alex-1-tech
 * @date    2026
 */

#include <future>
#include <memory>

#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN
template<typename T> class ResourceHandle
{
public:
  ResourceHandle() = default;

  explicit ResourceHandle(u64 identificator)
      : m_id(identificator) {};


  [[nodiscard]] auto isValid() const -> bool { return m_id != 0; }
  [[nodiscard]] auto getId() const -> u64 { return m_id; }

  auto operator==(const ResourceHandle& other) const -> bool { return m_id == other.m_id; }
  auto operator!=(const ResourceHandle& other) const -> bool { return m_id != other.m_id; }

private:
  u64 m_id = 0;
};

enum class LoadPriority : u8 {
  CRITICAL = 0, // Needed for the next frame
  HIGH = 1,     // Needed soon
  NORMAL = 2,   // Background loading
  LOW = 3       // Idle loading
};

template<typename T> using ResourceResult = Result<T, String>;

/// Futures for asynchronous loading
template<typename T> using ResourceFuture = std::future<ResourceResult<std::shared_ptr<T>>>;
EGE_NAMESPACE_END