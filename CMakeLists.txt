cmake_minimum_required(VERSION 3.20)

set(CMAKE_SUPPRESS_REGENERATION ON)

project(TestCpp LANGUAGES CXX)

set(CMAKE_XCODE_GENERATE_SCHEME ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(main
  src/main.cpp
  src/network.cpp
  src/network.h
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES
  src/main.cpp
  src/network.cpp
  src/network.h
)

target_include_directories(main PRIVATE
  src
)

target_compile_options(main PRIVATE
  -Wall
  -Wextra
  $<$<CONFIG:Debug>:-O0>
  $<$<CONFIG:Debug>:-g>
  $<$<CONFIG:Release>:-O3>
)

target_compile_definitions(main PRIVATE
  $<$<CONFIG:Release>:NDEBUG>
)

option(ENABLE_ASAN "Enable AddressSanitizer for debug builds" ON)

if(ENABLE_ASAN)
  target_compile_options(main PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address>
  )
  target_link_options(main PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address>
  )
endif()

set_target_properties(main PROPERTIES
  XCODE_GENERATE_SCHEME ON
  XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(APPLE)
  set_target_properties(main PROPERTIES
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Debug] "dwarf-with-dsym"
  )
endif()

if(APPLE AND NOT CMAKE_CONFIGURATION_TYPES AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  find_program(DSYMUTIL_EXECUTABLE dsymutil)

  if(DSYMUTIL_EXECUTABLE)
    add_custom_command(TARGET main POST_BUILD
      COMMAND "${DSYMUTIL_EXECUTABLE}" "$<TARGET_FILE:main>" -o "$<TARGET_FILE:main>.dSYM"
      VERBATIM
    )
  endif()
endif()
