cmake_minimum_required(VERSION 3.5)
project(trappable_gui)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets PrintSupport REQUIRED)

set(GUI_SOURCES
    main.cpp
    MathieuBackend.cpp
    MathieuWindow.cpp
    plot/MathieuStabilityPlot.cpp
    plot/QCustomPlot/qcustomplot.cpp
)

add_executable(gui ${GUI_SOURCES})
target_link_libraries(gui PRIVATE Qt6::Widgets Qt6::PrintSupport mathieu_lib)
target_include_directories(gui PRIVATE ../mathieu_lib/include ./plot)
