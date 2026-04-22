cmake_minimum_required(VERSION 3.15)
project(TestCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/TestCpp)
file(GLOB SRC ${SRC_DIR}/*.cpp)

add_executable(TestCpp ${SRC})

# POSIX/macOS/Linux focused build flags
target_compile_options(TestCpp PRIVATE -Wall -Wextra)
find_package(Threads REQUIRED)
target_link_libraries(TestCpp PRIVATE Threads::Threads)

set_target_properties(TestCpp PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
