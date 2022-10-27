add_executable(${MY_PROJECT_NAME} main.cpp ${PROJECT_SOURCES})

target_link_libraries(${MY_PROJECT_NAME}
  PRIVATE CLI11::CLI11
)
