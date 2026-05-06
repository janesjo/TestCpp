cmake_minimum_required(VERSION 3.20)

get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

set(CONFIGURE_PRESETS
  vscode-debug
  vscode-release
  vscode-debug-appleclang
  vscode-release-appleclang
  xcode
)

set(BUILD_PRESETS
  vscode-debug
  vscode-release
  vscode-debug-appleclang
  vscode-release-appleclang
  xcode-debug
  xcode-release
)

foreach(PRESET IN LISTS CONFIGURE_PRESETS)
  message(STATUS "Configuring ${PRESET}")
  execute_process(
    COMMAND "${CMAKE_COMMAND}" --preset "${PRESET}"
    WORKING_DIRECTORY "${PROJECT_ROOT}"
    COMMAND_ERROR_IS_FATAL ANY
  )
endforeach()

foreach(PRESET IN LISTS BUILD_PRESETS)
  message(STATUS "Building ${PRESET}")
  execute_process(
    COMMAND "${CMAKE_COMMAND}" --build --preset "${PRESET}"
    WORKING_DIRECTORY "${PROJECT_ROOT}"
    COMMAND_ERROR_IS_FATAL ANY
  )
endforeach()
