cmake_minimum_required(VERSION 3.5)
project(trappable)

add_subdirectory(mathieu_lib)
add_subdirectory(gui)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_mathieu tests/test_mathieu.cpp)
target_include_directories(test_mathieu PRIVATE ${CMAKE_SOURCE_DIR}/mathieu_lib/include)
target_link_libraries(test_mathieu mathieu_lib ${GTEST_LIBRARIES} pthread)
add_test(NAME test_mathieu COMMAND test_mathieu)
