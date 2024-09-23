#pragma once

#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "ghost/mutex/mutex.h"

namespace utils {

class ThreadPool {
 public:
  explicit ThreadPool(size_t thread_num);

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  ~ThreadPool() noexcept;

  // @return future for synchronization.
  template <typename Fn, typename... Args>
  auto Push(Fn&& fn, Args&&... args)
    -> std::future<typename std::result_of_t<Fn(Args...)>>;

  // Block until the threadpool is dead, or all enqueued tasks finish.
  void Wait();

 private:
  using Job = std::function<void(void)>;

  size_t idle_num_ GHOST_GUARDED_BY(mutex_) = 0;
  bool stopped_ GHOST_GUARDED_BY(mutex_) = false;
  ghost::mutex mutex_;
  std::condition_variable new_job_cv_;
  std::condition_variable job_completion_cv_;
  std::queue<Job> GHOST_GUARDED_BY(mutex_) jobs_;
  std::vector<std::thread> GHOST_GUARDED_BY(mutex_) workers_;
};

template <typename Fn, typename... Args>
auto ThreadPool::Push(Fn&& fn, Args&&... args)
  -> std::future<typename std::result_of_t<Fn(Args...)>> {
  using Ret = typename std::result_of_t<Fn(Args...)>;

  auto job = std::make_shared<std::packaged_task<Ret()>>(
      std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...));
  std::future<Ret> result = job->get_future();
  {
    ghost::lock_guard<ghost::mutex> lck(mutex_);
    jobs_.emplace([job = std::move(job)]() mutable { (*job)(); });
    new_job_cv_.notify_one();
  }
  return result;
}

}  // namespace utils
