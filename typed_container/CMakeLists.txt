project(typed_container LANGUAGES CXX)

add_executable(typed_container_main src/main.cpp)
target_include_directories(typed_container_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(BUILD_TESTING)
  add_executable(typed_container_test test/typed_container_test.cpp)
  target_include_directories(typed_container_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_link_libraries(typed_container_test PRIVATE GTest::gtest_main)
  gtest_discover_tests(typed_container_test)
endif()
