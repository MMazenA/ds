project(vector LANGUAGES CXX)

add_executable(vector_main src/main.cpp)
target_include_directories(vector_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(BUILD_TESTING)
  add_executable(vector_test test/vector_test.cpp)
  target_include_directories(vector_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_link_libraries(vector_test PRIVATE GTest::gtest_main)
  gtest_discover_tests(vector_test)
endif()

if(BUILD_BENCHMARKS)
  add_executable(vector_bench bench/vector_bench.cpp)
  target_include_directories(vector_bench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_link_libraries(vector_bench PRIVATE benchmark::benchmark_main)
endif()
