cmake_minimum_required(VERSION 3.25)
project(anymap LANGUAGES CXX)

if(BUILD_TESTING)
  add_executable(anymap_test test/any_map_test.cpp)
  target_include_directories(anymap_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_link_libraries(anymap_test PRIVATE GTest::gtest_main)
  gtest_discover_tests(anymap_test)
endif()
