#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "engine/assets/asset_loader.hpp"
#include "engine/core/resource_types.hpp"

EGE_NAMESPACE_BEGIN
struct ResourceEntry {
  Ref<void> resource;
  std::type_index type;
  std::chrono::steady_clock::time_point lastAccess;
  u32 accessCount = 0;
  bool persistent = false;
};

struct LoadTask {
  Path path;
  LoadPriority priority;
  std::promise<ResourceResult<Ref<void>>> promise;
  std::type_index resourceType;
};

inline constexpr auto DEFAULT_RESOURCE_UNLOAD_AGE = std::chrono::seconds(30);

class AssetManager
{
public:
  AssetManager();
  ~AssetManager();

  AssetManager(const AssetManager&) = delete;
  auto operator=(const AssetManager&) -> AssetManager& = delete;
  AssetManager(AssetManager&&) = delete;
  auto operator=(AssetManager&&) -> AssetManager& = delete;

  auto initialize() -> bool;
  void shutdown();

  template<typename T> auto load(const Path& path) -> ResourceResult<Ref<T>>;
  template<typename T> auto loadAsync(const Path& path, LoadPriority priority = LoadPriority::NORMAL) -> ResourceFuture<T>;
  template<typename T> auto get(const Path& path) -> Ref<T>;
  template<typename T> void unload(const Path& path);
  template<typename T> void setPersistent(const Path& path, bool persistent = true);
  void unloadUnused(std::chrono::seconds minAge = DEFAULT_RESOURCE_UNLOAD_AGE);
  void checkForChanges();

  struct Statistics {
    usize loadedResources = 0;
    usize memoryUsage = 0; // Bytes
    usize pendingLoads = 0;
  };

  [[nodiscard]] auto getStatistics() const -> Statistics;

private:
  void workerThread();
  void processLoadQueue();
  auto getOrCreateEntry(const Path& path, const std::type_index& type) -> ResourceEntry&;
  [[nodiscard]] auto getPathFromId(u64 identificator) const -> Path;

  Unique<AssetLoaderRegistry> m_loaderRegistry;

  std::thread m_workerThread;
  std::atomic<bool> m_running = false;

  std::mutex m_queueMutex;
  std::condition_variable m_queueCV;


  struct LoadTaskComparator {
    auto operator()(const LoadTask& firstTask, const LoadTask& secondTask) const -> bool
    {
      return static_cast<u8>(firstTask.priority) > static_cast<u8>(secondTask.priority);
    }
  };

  std::priority_queue<LoadTask, std::vector<LoadTask>, LoadTaskComparator> m_loadQueue;

  mutable std::mutex m_resourcesMutex;
  std::unordered_map<Path, ResourceEntry> m_resources;
  std::unordered_map<u64, Path> m_idToPath;

#ifdef EGE_PLATFORM_WINDOWS
  void* m_fileWatcher = nullptr;
#else
  int m_inotifyFd = IVALID_INDEX;
  std::unordered_map<int, Path> m_watchDescriptors;
#endif

  std::atomic<u64> m_nextId = ONE;
};

EGE_NAMESPACE_END