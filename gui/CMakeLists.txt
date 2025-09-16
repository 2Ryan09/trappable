cmake_minimum_required(VERSION 3.5)
project(trappable_gui)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(GUI_SOURCES
    main.cpp
    MathieuBackend.cpp
    MathieuWindow.cpp
)

add_executable(gui ${GUI_SOURCES})
target_link_libraries(gui PRIVATE Qt6::Widgets mathieu_lib)
target_include_directories(gui PRIVATE ../mathieu_lib/include)
