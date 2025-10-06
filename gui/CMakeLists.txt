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
	${CMAKE_SOURCE_DIR}/mathieu_lib/include
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
	${CMAKE_SOURCE_DIR}/mathieu_lib/include
)
target_link_libraries(stability PRIVATE Qt6::Widgets Qt6::PrintSupport)

add_library(minicalculator STATIC MiniCalculator.cpp MiniCalculator.h)
target_include_directories(minicalculator PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/mathieu_lib/include
)
target_link_libraries(minicalculator PRIVATE Qt6::Widgets Qt6::PrintSupport)

add_library(mathieuwindow STATIC MathieuWindow.cpp MathieuWindow.h Inputs.cpp Inputs.h Outputs.cpp Outputs.h)
target_include_directories(mathieuwindow PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/plot
	${CMAKE_CURRENT_SOURCE_DIR}/plot/QCustomPlot
	${CMAKE_CURRENT_SOURCE_DIR}/stability
	${CMAKE_SOURCE_DIR}/mathieu_lib/include
)
target_link_libraries(mathieuwindow PRIVATE minicalculator stabilityregionplotter mathieubackend stability Qt6::Widgets Qt6::PrintSupport)

qt_add_resources(GUI_RESOURCES icons.qrc)
add_executable(gui WIN32 main.cpp ${APP_ICON_RESOURCE_WINDOWS} ${GUI_RESOURCES})
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

target_link_libraries(gui PRIVATE mathieuwindow stabilityregionplotter mathieubackend stability Qt6::Widgets Qt6::PrintSupport mathieu_lib qcustomplot)

# Runtime library paths - Qt DLLs will be in same directory as executable
target_include_directories(gui PRIVATE
	${CMAKE_SOURCE_DIR}/mathieu_lib/include
	${CMAKE_CURRENT_SOURCE_DIR}/plot
	${CMAKE_CURRENT_SOURCE_DIR}/plot/QCustomPlot
	${CMAKE_CURRENT_SOURCE_DIR}/stability
)

# Qt Deployment - Always bundle Qt
# Create the deployment directory
file(MAKE_DIRECTORY ${QT_DEPLOY_DIR})

# Run windeployqt after building the GUI executable
add_custom_command(TARGET gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E env QTFRAMEWORK_BYPASS_LICENSE_CHECK=1 ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gui> ${QT_DEPLOY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E env QTFRAMEWORK_BYPASS_LICENSE_CHECK=1 ${QT_DEPLOY_TOOL} --verbose 2 --no-translations --no-system-d3d-compiler --no-virtualkeyboard --no-opengl-sw ${QT_DEPLOY_DIR}/gui.exe
    COMMENT "Deploying Qt libraries for gui executable"
)

# Copy mathieu_lib to deployment directory
add_custom_command(TARGET gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:mathieu_lib> ${QT_DEPLOY_DIR}/
    COMMENT "Copying mathieu_lib to deployment directory"
)

# Install rules
install(TARGETS gui mathieuwindow stabilityregionplotter mathieubackend stability
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)

# Install Qt deployment - always include bundled Qt
install(DIRECTORY ${QT_DEPLOY_DIR}/ DESTINATION bin
    USE_SOURCE_PERMISSIONS
    PATTERN "*.pdb" EXCLUDE
    PATTERN "*.ilk" EXCLUDE
)
