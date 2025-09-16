cmake_minimum_required(VERSION 3.5)
project(trappable_gui)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets PrintSupport REQUIRED)


# Build each h/cpp pair as its own static library
add_subdirectory(plot/QCustomPlot)

add_library(stabilityregionplotter STATIC plot/StabilityRegionPlotter.cpp plot/StabilityRegionPlotter.h)
target_include_directories(stabilityregionplotter PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/plot
	${CMAKE_CURRENT_SOURCE_DIR}/plot/QCustomPlot
)
target_link_libraries(stabilityregionplotter PUBLIC qcustomplot Qt6::Widgets Qt6::PrintSupport)

add_library(mathieubackend STATIC MathieuBackend.cpp MathieuBackend.h)
target_include_directories(mathieubackend PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/plot
	${CMAKE_CURRENT_SOURCE_DIR}/plot/QCustomPlot
	${CMAKE_SOURCE_DIR}/mathieu_lib/include
)
target_link_libraries(mathieubackend PUBLIC Qt6::Widgets Qt6::PrintSupport)

add_library(mathieuwindow STATIC MathieuWindow.cpp MathieuWindow.h)
target_include_directories(mathieuwindow PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/plot
	${CMAKE_CURRENT_SOURCE_DIR}/plot/QCustomPlot
)
target_link_libraries(mathieuwindow PUBLIC stabilityregionplotter mathieubackend Qt6::Widgets Qt6::PrintSupport)

add_executable(gui main.cpp)
target_link_libraries(gui PRIVATE mathieuwindow stabilityregionplotter mathieubackend Qt6::Widgets Qt6::PrintSupport mathieu_lib qcustomplot)
target_include_directories(gui PRIVATE
	${CMAKE_SOURCE_DIR}/mathieu_lib/include
	${CMAKE_CURRENT_SOURCE_DIR}/plot
	${CMAKE_CURRENT_SOURCE_DIR}/plot/QCustomPlot
)
