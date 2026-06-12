cmake_minimum_required(VERSION 3.25)
project(unique_ptr LANGUAGES CXX)

if(BUILD_TESTING)
  add_executable(unique_ptr_test test/unique_ptr_test.cpp)
  target_include_directories(unique_ptr_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_link_libraries(unique_ptr_test PRIVATE GTest::gtest_main)
  gtest_discover_tests(unique_ptr_test)
endif()
