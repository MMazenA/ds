# ds
#### Projects to explore data structures in modern cpp
#### This is not good code, all exploratory

---
Repo contains:
#### Typed Container
- Based on [Using Modern C++ to Eliminate Virtual Functions - Jonathan Gopel - CppCon 2022](https://www.youtube.com/watch?v=gTNJXVmuRRA)
- Explores multi-type storage without v-table overhead
- Uses `std::concepts` and `std::variant` to emulate owning polymorphic types
- Generic Programming Practice

#### Vector
- Custom allocation
- Custom vector with bidirectional_iterator concept abiding iter
- Python-like vector slicing

####

---

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


---
