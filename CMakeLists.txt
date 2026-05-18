cmake_minimum_required(VERSION 3.25)
project(ds VERSION 0.1.0 LANGUAGES CXX)

if(APPLE)
  add_link_options("-Wl,-no_warn_duplicate_libraries")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

if(BUILD_TESTING)
  find_package(GTest CONFIG REQUIRED)
  include(GoogleTest)
endif()

add_subdirectory(vector)
add_subdirectory(type_erased_map)
add_subdirectory(concurrent_map)
