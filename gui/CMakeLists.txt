cmake_minimum_required(VERSION 3.5)
project(trappable_gui)

find_package(Qt5 COMPONENTS Quick QuickControls2 REQUIRED)

qt5_add_resources(QML_RESOURCES qml.qrc)

add_executable(gui
    main.cpp
    ${QML_RESOURCES}
)

target_link_libraries(gui PRIVATE Qt5::Quick Qt5::QuickControls2 mathieu_lib)
target_include_directories(gui PRIVATE ../mathieu_lib/include)
