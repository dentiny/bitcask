// Benchmark between leveldb and bitcask.

#include <benchmark/benchmark.h>

#include <array>
#include <string_view>

#include "absl/strings/str_format.h"
#include "bitcask/db_impl.h"
#include "utils/scoped_directory.h"
#include "utils/status/status_macros.h"

namespace bitcask {

namespace {
constexpr int kMaxVal = 1000000;
std::array<int, 10000> keys_to_query;

// Prefix string for large string.
constexpr std::string_view kLargeStrPrefix = "hello world prefix";
}  // namespace

void BM_bitcask_single_thread_small_value(benchmark::State& state) {
  for (auto _ : state) {
    LOG(INFO) << "Start bitcask benchmark";
    // Put key-value pairs.
    utils::ScopedDirectory dir;
    const auto db_folder = dir.get_directory();
    GHOST_ASSIGN_OR_THROW(auto db, DbImpl::New(db_folder, Config{}));
    for (int ii = 0; ii < kMaxVal; ++ii) {
      const std::string key = absl::StrFormat("%d", ii);
      const std::string val = absl::StrFormat("%d", ii);
      GHOST_THROW_IF_ERROR(db->Put(std::move(key), std::move(val)));
    }
    LOG(INFO) << "Key value pairs write completed";

    // Get key-value pairs.
    for (int ii = 0; ii < 10000; ++ii) {
      const std::string key = absl::StrFormat("%d", ii);
      const std::string expected_val = absl::StrFormat("%d", ii);
      GHOST_ASSIGN_OR_THROW(auto val, db->Get(key));
      GHOST_CHECK_EQ(INTERNAL, val, expected_val);
    }
  }
}

void BM_bitcask_single_thread_large_value(benchmark::State& state) {
  for (auto _ : state) {
    LOG(INFO) << "Start bitcask benchmark";
    // Put key-value pairs.
    utils::ScopedDirectory dir;
    const auto db_folder = dir.get_directory();
    GHOST_ASSIGN_OR_THROW(auto db, DbImpl::New(db_folder, Config{}));
    for (int ii = 0; ii < kMaxVal; ++ii) {
      const std::string key = absl::StrFormat("%s%d", kLargeStrPrefix, ii);
      const std::string val = absl::StrFormat("%s%d", kLargeStrPrefix, ii);
      GHOST_THROW_IF_ERROR(db->Put(std::move(key), std::move(val)));
    }
    LOG(INFO) << "Key value pairs write completed";

    // Get key-value pairs.
    for (int ii = 0; ii < 10000; ++ii) {
      const std::string key = absl::StrFormat("%s%d", kLargeStrPrefix, ii);
      const std::string expected_val =
          absl::StrFormat("%s%d", kLargeStrPrefix, ii);
      GHOST_ASSIGN_OR_THROW(auto val, db->Get(key));
      GHOST_CHECK_EQ(INTERNAL, val, expected_val);
    }
  }
}

BENCHMARK(BM_bitcask_single_thread_small_value)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(1)
    ->MeasureProcessCPUTime()
    ->UseRealTime();
BENCHMARK(BM_bitcask_single_thread_large_value)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(1)
    ->MeasureProcessCPUTime()
    ->UseRealTime();

}  // namespace bitcask

int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);
  // Initialize keys to query.
  for (int ii = 0; ii < 10000; ++ii) {
    bitcask::keys_to_query[ii] = rand() % bitcask::kMaxVal;
  }
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}
