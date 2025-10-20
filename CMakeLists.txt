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


add_executable(tree src/tree.cpp)
target_include_directories(tree PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(std-set src/std-set.cpp)
target_include_directories(std-set PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Testing
enable_testing()
add_executable(google_test src/google_test.cpp)
target_link_libraries(google_test PRIVATE GTest::gtest_main)
gtest_discover_tests(google_test TEST_PREFIX gtest_)

add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS google_test
)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_custom_target(benchmark
    COMMAND ${CMAKE_BUILD_TYPE} $<TARGET_FILE:tree> $<TARGET_FILE:std-set>
    COMMAND Python3::Interpreter ${CMAKE_SOURCE_DIR}/run_benchmarks.py
    DEPENDS tree std-set
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running benchmarks and generating statistics/time.txt"
)
