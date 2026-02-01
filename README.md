# ds

## Build

```bash
cmake --preset dev
cmake --build build
```

## Test

```bash
ctest --preset dev
```

## Benchmark

```bash
cmake --preset release
cmake --build build-release

./build-release/vector/vector_bench --benchmark_out=build-release/baseline.json --benchmark_out_format=json

# after changes
./build-release/vector/vector_bench --benchmark_out=build-release/current.json --benchmark_out_format=json

# compare
uv run --with scipy --with numpy python external/vcpkg/buildtrees/benchmark/src/v1.9.5-*/tools/compare.py benchmarks build-release/baseline.json build-release/current.json
```
