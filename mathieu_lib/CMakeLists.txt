add_library(mathieu_lib
    src/mathieu.cpp
)

target_include_directories(mathieu_lib PUBLIC include)

# Suppress AUTOMOC warning since mathieu_lib does not use Qt features
set_target_properties(mathieu_lib PROPERTIES AUTOMOC OFF)
