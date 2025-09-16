cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets PrintSupport REQUIRED)


# Build each h/cpp pair as its own static library
add_subdirectory(plot/QCustomPlot)


add_library(stabilityregionplotter STATIC plot/StabilityRegionPlotter.cpp plot/StabilityRegionPlotter.h)
target_include_directories(stabilityregionplotter PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/plot
	${CMAKE_CURRENT_SOURCE_DIR}/plot/QCustomPlot
	${CMAKE_CURRENT_SOURCE_DIR}/stability
	${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(stabilityregionplotter PRIVATE qcustomplot Qt6::Widgets Qt6::PrintSupport)


add_library(mathieubackend STATIC MathieuBackend.cpp MathieuBackend.h)
target_include_directories(mathieubackend PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/plot
	${CMAKE_CURRENT_SOURCE_DIR}/plot/QCustomPlot
	${CMAKE_SOURCE_DIR}/mathieu_lib/include
)
target_link_libraries(mathieubackend PRIVATE Qt6::Widgets Qt6::PrintSupport)



# Stability module
add_library(stability STATIC 
	stability/StabilityCalculator.cpp stability/StabilityCalculator.h
	stability/StabilityOutputs.cpp stability/StabilityOutputs.h
)
target_include_directories(stability PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/stability
)
target_link_libraries(stability PRIVATE Qt6::Widgets Qt6::PrintSupport)

add_library(mathieuwindow STATIC MathieuWindow.cpp MathieuWindow.h Inputs.cpp Inputs.h Outputs.cpp Outputs.h)
target_include_directories(mathieuwindow PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/plot
	${CMAKE_CURRENT_SOURCE_DIR}/plot/QCustomPlot
	${CMAKE_CURRENT_SOURCE_DIR}/stability
)
target_link_libraries(mathieuwindow PRIVATE stabilityregionplotter mathieubackend stability Qt6::Widgets Qt6::PrintSupport)


add_executable(gui main.cpp)
target_link_libraries(gui PRIVATE mathieuwindow stabilityregionplotter mathieubackend stability Qt6::Widgets Qt6::PrintSupport mathieu_lib qcustomplot)
target_include_directories(gui PRIVATE
	${CMAKE_SOURCE_DIR}/mathieu_lib/include
	${CMAKE_CURRENT_SOURCE_DIR}/plot
	${CMAKE_CURRENT_SOURCE_DIR}/plot/QCustomPlot
	${CMAKE_CURRENT_SOURCE_DIR}/stability
)

# Install rules
install(TARGETS gui mathieuwindow stabilityregionplotter mathieubackend stability
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)
