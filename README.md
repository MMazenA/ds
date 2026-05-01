# ds

## vcpkg setup

```bash
  git submodule update --init --recursive
  external\vcpkg\bootstrap-vcpkg.bat 
```

## Build

```bash
cmake --preset dev
cmake --build build
```

## Test

```bash
ctest --preset dev
```

