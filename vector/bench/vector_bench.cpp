#include <benchmark/benchmark.h>
#include "Vector.h"
#include <vector>
#include <string>

static void BM_DsVector_PushBack_Int(benchmark::State& state) {
    for (auto _ : state) {
        ds::Vector<int> vec;
        for (int i = 0; i < state.range(0); ++i) {
            vec.push_back(i);
        }
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_DsVector_PushBack_Int)->Range(8, 8 << 10);

static void BM_StdVector_PushBack_Int(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> vec;
        for (int i = 0; i < state.range(0); ++i) {
            vec.push_back(i);
        }
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_StdVector_PushBack_Int)->Range(8, 8 << 10);

static void BM_DsVector_PushBack_String(benchmark::State& state) {
    for (auto _ : state) {
        ds::Vector<std::string> vec;
        for (int i = 0; i < state.range(0); ++i) {
            vec.push_back("test string data");
        }
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_DsVector_PushBack_String)->Range(8, 8 << 10);

static void BM_StdVector_PushBack_String(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<std::string> vec;
        for (int i = 0; i < state.range(0); ++i) {
            vec.push_back("test string data");
        }
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_StdVector_PushBack_String)->Range(8, 8 << 10);

static void BM_DsVector_PushBack_Move(benchmark::State& state) {
    for (auto _ : state) {
        ds::Vector<std::string> vec;
        for (int i = 0; i < state.range(0); ++i) {
            std::string s = "test string data";
            vec.push_back(std::move(s));
        }
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_DsVector_PushBack_Move)->Range(8, 8 << 10);

static void BM_StdVector_PushBack_Move(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<std::string> vec;
        for (int i = 0; i < state.range(0); ++i) {
            std::string s = "test string data";
            vec.push_back(std::move(s));
        }
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_StdVector_PushBack_Move)->Range(8, 8 << 10);

static void BM_DsVector_Access(benchmark::State& state) {
    ds::Vector<int> vec;
    for (int i = 0; i < state.range(0); ++i) {
        vec.push_back(i);
    }

    for (auto _ : state) {
        int sum = 0;
        for (int i = 0; i < state.range(0); ++i) {
            sum += vec[i];
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_DsVector_Access)->Range(8, 8 << 10);

static void BM_StdVector_Access(benchmark::State& state) {
    std::vector<int> vec;
    for (int i = 0; i < state.range(0); ++i) {
        vec.push_back(i);
    }

    for (auto _ : state) {
        int sum = 0;
        for (int i = 0; i < state.range(0); ++i) {
            sum += vec[static_cast<size_t>(i)];
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_StdVector_Access)->Range(8, 8 << 10);

static void BM_DsVector_Iterate(benchmark::State& state) {
    ds::Vector<int> vec;
    for (int i = 0; i < state.range(0); ++i) {
        vec.push_back(i);
    }

    for (auto _ : state) {
        int sum = 0;
        for (const auto& val : vec) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_DsVector_Iterate)->Range(8, 8 << 10);

static void BM_StdVector_Iterate(benchmark::State& state) {
    std::vector<int> vec;
    for (int i = 0; i < state.range(0); ++i) {
        vec.push_back(i);
    }

    for (auto _ : state) {
        int sum = 0;
        for (const auto& val : vec) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_StdVector_Iterate)->Range(8, 8 << 10);

static void BM_DsVector_Copy(benchmark::State& state) {
    ds::Vector<int> src;
    for (int i = 0; i < state.range(0); ++i) {
        src.push_back(i);
    }

    for (auto _ : state) {
        ds::Vector<int> copy(src);
        benchmark::DoNotOptimize(copy);
    }
}
BENCHMARK(BM_DsVector_Copy)->Range(8, 8 << 10);

static void BM_StdVector_Copy(benchmark::State& state) {
    std::vector<int> src;
    for (int i = 0; i < state.range(0); ++i) {
        src.push_back(i);
    }

    for (auto _ : state) {
        std::vector<int> copy(src);
        benchmark::DoNotOptimize(copy);
    }
}
BENCHMARK(BM_StdVector_Copy)->Range(8, 8 << 10);
