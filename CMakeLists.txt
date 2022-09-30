cmake_minimum_required(VERSION 3.14)

set(MY_PROJECT_NAME "cli11_template")

project(${MY_PROJECT_NAME})

set(CMAKE_BUILD_TYPE Debug) # Debug or Release

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/source
)

#link_directories()

file(GLOB PROJECT_SOURCES
  include/*.hpp
  source/*.cpp
  source/*.hpp
)

add_executable(${MY_PROJECT_NAME} ${PROJECT_SOURCES})

message("-- Fetch Packages")

# fetch latest cli11
message(" >> Fetch CLI11")
include(FetchContent)
FetchContent_Declare(cli11
  GIT_REPOSITORY  https://github.com/CLIUtils/CLI11
  GIT_TAG         main  # please keep the tag latest, "main" or "v2.2.0"
)
FetchContent_GetProperties(cli11)
if(NOT cli11_POPULATED)
  FetchContent_Populate(cli11)
  add_subdirectory(${cli11_SOURCE_DIR} ${cli11_BINARY_DIR})
endif()
FetchContent_MakeAvailable(cli11)
message(" >> Fetch CLI11 - done")

message("-- Fetch Packages - done")

target_link_libraries(${MY_PROJECT_NAME}
  PRIVATE CLI11::CLI11
)
