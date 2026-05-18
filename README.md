# ds

C++ data structures.

## Setup

```bash
git submodule update --init --recursive
./external/vcpkg/bootstrap-vcpkg.sh
```

Deps from `vcpkg.json` resolve automatically on first cmake configure.

## Dev (tests)

```bash
cmake --preset dev
cmake --build build
ctest --preset dev
```

## Release (benchmarks)

```bash
cmake --preset release
cmake --build build-release
./build-release/concurrent_map/map_bench
```
