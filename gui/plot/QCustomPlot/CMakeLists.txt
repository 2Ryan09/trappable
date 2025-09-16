# Build QCustomPlot as a static library for fast incremental builds
cmake_minimum_required(VERSION 3.5)
project(qcustomplot)

set(CMAKE_AUTOMOC ON)

add_library(qcustomplot STATIC qcustomplot.cpp)
target_include_directories(qcustomplot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Qt6 COMPONENTS Widgets PrintSupport REQUIRED)
target_link_libraries(qcustomplot PUBLIC Qt6::Widgets Qt6::PrintSupport)
