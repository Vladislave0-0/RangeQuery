cmake_minimum_required(VERSION 3.11)
project("RangeQuery")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

# Option for sanitizer
option(USE_SANITIZER "Enable address sanitizer" OFF)
if(USE_SANITIZER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

include_directories(include/)

set(RANGE_QUERY_SOURCES
    src/main.cpp
)

add_executable(tree ${RANGE_QUERY_SOURCES})

# Include directories for the target
target_include_directories(tree PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Testing
enable_testing()
add_executable(google_test src/google_test.cpp)
target_link_libraries(google_test PRIVATE GTest::gtest_main)

gtest_discover_tests(google_test TEST_PREFIX gtest_)

# add_test(
#     NAME end2end_tests
#     COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/end2end/run_e2e.sh 
#         -b $<TARGET_FILE:tree>
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/end2end
# )

add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS google_test
)
