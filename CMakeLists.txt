

cmake_minimum_required(VERSION 3.5)
project(trappable)

set(CMAKE_AUTOMOC ON)

set(CMAKE_SKIP_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

add_subdirectory(mathieu_lib)
add_subdirectory(gui)

option(BUILD_TESTS "Build test executables" ON)

if(BUILD_TESTS)
	enable_testing()
	find_package(GTest REQUIRED)
	include_directories(${GTEST_INCLUDE_DIRS})

	add_executable(test_mathieu tests/test_mathieu.cpp)
	target_include_directories(test_mathieu PRIVATE ${CMAKE_SOURCE_DIR}/mathieu_lib/include)
	target_link_libraries(test_mathieu mathieu_lib gtest gtest_main pthread)
	add_test(NAME test_mathieu COMMAND test_mathieu)

	# GUI E2E test
	find_package(Qt6 COMPONENTS Widgets PrintSupport Test REQUIRED)
	add_executable(test_mathieu_e2e tests/test_mathieu_e2e.cpp gui/MathieuWindow.cpp gui/plot/MathieuStabilityPlot.cpp gui/plot/QCustomPlot/qcustomplot.cpp)
	target_include_directories(test_mathieu_e2e PRIVATE ${PROJECT_SOURCE_DIR}/gui ${PROJECT_SOURCE_DIR}/gui/plot ${PROJECT_SOURCE_DIR}/mathieu_lib/include)
	target_link_libraries(test_mathieu_e2e PRIVATE Qt6::Widgets Qt6::PrintSupport Qt6::Test mathieu_lib)
	add_test(NAME test_mathieu_e2e COMMAND test_mathieu_e2e)
endif()
