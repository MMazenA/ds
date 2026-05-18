cmake_minimum_required(VERSION 3.25)
project(concurrent_map LANGUAGES CXX)

add_library(concurrent_map INTERFACE)
target_include_directories(concurrent_map INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(concurrent_map INTERFACE cxx_std_23)

option(BUILD_BENCHMARKS "Build microbenchmarks" OFF)
if(BUILD_BENCHMARKS)
  find_package(benchmark CONFIG REQUIRED)
  add_executable(map_bench bench/map_bench.cpp)
  target_link_libraries(map_bench PRIVATE concurrent_map benchmark::benchmark_main)
endif()
