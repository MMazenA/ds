cmake_minimum_required(VERSION 3.25)
project(vector LANGUAGES CXX)

add_executable(vector_main src/main.cpp)
target_include_directories(vector_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(vector_main PUBLIC cxx_std_23)

if(BUILD_TESTING)
  add_executable(vector_test test/vector_test.cpp)
  target_include_directories(vector_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_link_libraries(vector_test PRIVATE GTest::gtest_main)
  gtest_discover_tests(vector_test)
endif()
