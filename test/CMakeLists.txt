set(MY_TEST_NAME "${MY_PROJECT_NAME}_test")

message("-- Fetch Packages")

# fetch latest GoogleTest
message(" >> Fetch GoogleTest")
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG        main
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    include_directories(${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)
endif()
FetchContent_MakeAvailable(googletest)
message(" >> Fetch GoogleTest - done")

message("-- Fetch Packages - done")

add_executable(${MY_TEST_NAME}
    test.cpp
    ${PROJECT_SOURCES}
)

target_link_libraries(${MY_TEST_NAME}
    PRIVATE CLI11::CLI11
    gtest
)
