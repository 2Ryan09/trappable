
# Use ccache for faster builds if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

cmake_minimum_required(VERSION 3.16)
project(trappable LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)



add_subdirectory(mathieu_lib)
add_subdirectory(gui)

option(BUILD_TESTS "Build test executables" ON)


if(BUILD_TESTS)
	enable_testing()
	include(FetchContent)
	FetchContent_Declare(
		googletest
		URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
		DOWNLOAD_EXTRACT_TIMESTAMP TRUE
	)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	# Disable GoogleMock build
	set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

	FetchContent_MakeAvailable(googletest)
	# Disable AUTOMOC for GoogleTest targets to avoid AUTOGEN Qt warnings
	set_target_properties(gtest PROPERTIES AUTOMOC OFF)
	set_target_properties(gtest_main PROPERTIES AUTOMOC OFF)

	add_executable(test_mathieu tests/test_mathieu.cpp)
	target_include_directories(test_mathieu PRIVATE ${CMAKE_SOURCE_DIR}/mathieu_lib/include ${CMAKE_SOURCE_DIR}/gui ${CMAKE_SOURCE_DIR}/gui/plot)
	target_link_libraries(test_mathieu PRIVATE mathieu_lib gtest gtest_main)
	set_target_properties(test_mathieu PROPERTIES
		INSTALL_RPATH "$ORIGIN:/usr/lib:/usr/lib/x86_64-linux-gnu:/usr/local/lib"
		BUILD_WITH_INSTALL_RPATH ON
	)

	add_executable(test_stabilitycalculator tests/test_stabilitycalculator.cpp gui/stability/StabilityCalculator.cpp)
	target_include_directories(test_stabilitycalculator PRIVATE ${CMAKE_SOURCE_DIR}/gui ${CMAKE_SOURCE_DIR}/gui/stability ${CMAKE_SOURCE_DIR}/mathieu_lib/include ${Qt6Gui_INCLUDE_DIRS})
	target_link_libraries(test_stabilitycalculator PRIVATE Qt6::Gui gtest gtest_main)
	set_target_properties(test_stabilitycalculator PROPERTIES
		INSTALL_RPATH "$ORIGIN:/usr/lib:/usr/lib/x86_64-linux-gnu:/usr/local/lib"
		BUILD_WITH_INSTALL_RPATH ON
	)
	add_test(NAME test_stabilitycalculator COMMAND test_stabilitycalculator)
	add_test(NAME test_mathieu COMMAND test_mathieu)

	# StabilityOutputs GUI/output logic tests
	add_executable(test_stabilityoutputs tests/test_stabilityoutputs.cpp gui/stability/StabilityOutputs.cpp)
	target_include_directories(test_stabilityoutputs PRIVATE ${CMAKE_SOURCE_DIR}/gui ${CMAKE_SOURCE_DIR}/gui/stability)
	target_link_libraries(test_stabilityoutputs PRIVATE Qt6::Widgets gtest gtest_main)
	set_target_properties(test_stabilityoutputs PROPERTIES
		INSTALL_RPATH "$ORIGIN:/usr/lib:/usr/lib/x86_64-linux-gnu:/usr/local/lib"
		BUILD_WITH_INSTALL_RPATH ON
	)
	add_test(NAME test_stabilityoutputs COMMAND test_stabilityoutputs)
		set_tests_properties(test_stabilityoutputs PROPERTIES LABELS "gui")
		if(DEFINED ENV{CI})
			set_tests_properties(test_stabilityoutputs PROPERTIES DISABLED TRUE)
		endif()

	# Vectorized mathieu_lib tests
	add_executable(test_mathieu_vector tests/test_mathieu_vector.cpp)
	target_include_directories(test_mathieu_vector PRIVATE ${CMAKE_SOURCE_DIR}/mathieu_lib/include ${CMAKE_SOURCE_DIR}/gui ${CMAKE_SOURCE_DIR}/gui/plot)
	target_link_libraries(test_mathieu_vector PRIVATE mathieu_lib gtest gtest_main)
	set_target_properties(test_mathieu_vector PROPERTIES
		INSTALL_RPATH "$ORIGIN:/usr/lib:/usr/lib/x86_64-linux-gnu:/usr/local/lib"
		BUILD_WITH_INSTALL_RPATH ON
	)
	add_test(NAME test_mathieu_vector COMMAND test_mathieu_vector)

	# GUI E2E test
	find_package(Qt6 COMPONENTS Widgets PrintSupport Test REQUIRED)
	add_executable(test_mathieu_e2e tests/test_mathieu_e2e.cpp gui/MathieuWindow.cpp gui/stability/StabilityOutputs.cpp gui/Inputs.cpp gui/Inputs.h gui/Outputs.cpp gui/Outputs.h gui/plot/StabilityRegionPlotter.cpp)
	target_include_directories(test_mathieu_e2e PRIVATE ${CMAKE_SOURCE_DIR}/gui ${CMAKE_SOURCE_DIR}/gui/plot ${CMAKE_SOURCE_DIR}/gui/plot/QCustomPlot ${CMAKE_SOURCE_DIR}/mathieu_lib/include)
	target_link_libraries(test_mathieu_e2e PRIVATE minicalculator Qt6::Widgets Qt6::PrintSupport Qt6::Test mathieu_lib qcustomplot stability)
	add_test(NAME test_mathieu_e2e COMMAND test_mathieu_e2e)
endif()

# Install rules
install(TARGETS mathieu_lib
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION bin)
install(DIRECTORY mathieu_lib/include/ DESTINATION include)
