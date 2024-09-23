#include "utils/thread_pool.h"

#include <utility>

#include "utils/status/status_macros.h"

namespace utils {

ThreadPool::ThreadPool(size_t thread_num) : idle_num_(thread_num) {
  workers_.reserve(thread_num);
  for (size_t ii = 0; ii < thread_num; ++ii) {
    workers_.emplace_back([this]() {
      for (;;) {
        Job cur_job;
        {
          ghost::unique_lock<ghost::mutex> lck(mutex_);
          new_job_cv_.wait(lck, [this]() GHOST_REQUIRES(mutex_) {
            return !jobs_.empty() || stopped_;
          });
          if (stopped_) {
            return;
          }
          cur_job = std::move(jobs_.front());
          jobs_.pop();
          --idle_num_;
        }
        cur_job();

        ghost::lock_guard<ghost::mutex> lck(mutex_);
        ++idle_num_;
        job_completion_cv_.notify_one();
      }
    });
  }
}

void ThreadPool::Wait() {
  ghost::unique_lock<ghost::mutex> lck(mutex_);
  job_completion_cv_.wait(lck, [this]() GHOST_REQUIRES(mutex_) {
    if (stopped_) {
      return true;
    }
    if (idle_num_ == workers_.size() && jobs_.empty()) {
      return true;
    }
    return false;
  });
}

ThreadPool::~ThreadPool() noexcept {
  {
    ghost::lock_guard<ghost::mutex> lck(mutex_);
    stopped_ = true;
    new_job_cv_.notify_one();
  }
  for (auto& cur_worker : workers_) {
    GHOST_CHECK(INTERNAL, cur_worker.joinable());
    cur_worker.join();
  }
}

}  // namespace utils
