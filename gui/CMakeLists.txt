cmake_minimum_required(VERSION 3.5)
project(trappable_gui)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(gui main.cpp)

target_link_libraries(gui PRIVATE Qt6::Widgets mathieu_lib)
target_include_directories(gui PRIVATE ../mathieu_lib/include)
