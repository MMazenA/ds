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

# run and compare ds::Vector vs std::vector
./build-release/vector/vector_bench --benchmark_out=build-release/results.json --benchmark_out_format=json
python3 scripts/compare_vectors.py build-release/results.json
```
