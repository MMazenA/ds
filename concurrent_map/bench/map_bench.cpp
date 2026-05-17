/*
 * google-benchmark reference file.
 *
 * A benchmark is a free function taking benchmark::State&. The library calls it
 * many times to collect statistics. The body of `for (auto _ : state) { ... }` is
 * the timed region; anything outside it, or anything between state.PauseTiming()
 * and state.ResumeTiming(), is excluded from the measurement. The library
 * chooses the iteration count automatically by adjusting until the wall-time is
 * statistically stable -- you do not pick it yourself.
 *
 * Registration uses BENCHMARK(fn), or BENCHMARK_TEMPLATE(fn, T) when fn is a
 * template. Chained methods on the registration configure runtime behavior:
 *
 *   ->Arg(n)             passes an integer readable inside the body via
 *                        state.range(0); use it to sweep input sizes, ratios,
 *                        or any other parameter.
 *   ->ThreadRange(a, b)  runs the same benchmark with that many threads
 *                        concurrently invoking the body, in powers of two.
 *   ->UseRealTime()      required whenever you use threads, because the
 *                        default timer sums CPU time across threads, which is
 *                        meaningless for concurrency comparisons.
 *
 * Inside the body, state.thread_index() returns the id of the current thread
 * and state.threads() the total count. Code guarded by
 * `if (state.thread_index() == 0)` runs only on the first thread -- the
 * idiomatic place to do one-time shared setup before the timed loop.
 *
 * benchmark::DoNotOptimize(x) is a compiler barrier that forces a value to be
 * computed and stored, preventing the optimizer from deleting the work you are
 * trying to measure. state.SetItemsProcessed(n) makes the library print
 * throughput (items/sec) in the output.
 *
 * Linking benchmark::benchmark_main (see CMakeLists.txt) provides main() for
 * free. That main parses CLI flags including --benchmark_filter=<regex>,
 * --benchmark_out=<file>, and --benchmark_out_format=json|csv|console.
 *
 * Snippet A is single-threaded and sweeps an input size. Snippet B is
 * templated over the map implementation and sweeps both the read/write ratio
 * and the thread count, which is the shape you will reach for when comparing
 * the lock-free and blocking maps against each other.
 */

#include <benchmark/benchmark.h>
#include <random>

#include "blocking_map.h"
#include "lockfree_map.h"

static void BM_Insert(benchmark::State& state) {
  const int n = state.range(0);
  for (auto _ : state) {
    state.PauseTiming();
    BlockingMap<int, int> m;
    state.ResumeTiming();

    for (int i = 0; i < n; ++i) m.insert(i, i);

    benchmark::DoNotOptimize(m);
  }
  state.SetItemsProcessed(int64_t(state.iterations()) * n);
}
BENCHMARK(BM_Insert)->Arg(1'000)->Arg(10'000)->Arg(100'000);

template <class Map>
static void BM_MixedReadWrite(benchmark::State& state) {
  static Map m;
  if (state.thread_index() == 0) {
    for (int i = 0; i < 10'000; ++i) m.insert(i, i);
  }

  const int read_pct = state.range(0);
  std::mt19937 rng(state.thread_index());
  std::uniform_int_distribution<int> key(0, 9'999);
  std::uniform_int_distribution<int> coin(0, 99);

  for (auto _ : state) {
    int k = key(rng);
    if (coin(rng) < read_pct) {
      auto v = m.find(k);
      benchmark::DoNotOptimize(v);
    } else {
      m.insert(k, k);
    }
  }
}
BENCHMARK_TEMPLATE(BM_MixedReadWrite, BlockingMap<int, int>)
    ->Arg(95)->Arg(50)->Arg(5)
    ->ThreadRange(1, 16)
    ->UseRealTime();
BENCHMARK_TEMPLATE(BM_MixedReadWrite, LockFreeMap<int, int>)
    ->Arg(95)->Arg(50)->Arg(5)
    ->ThreadRange(1, 16)
    ->UseRealTime();
