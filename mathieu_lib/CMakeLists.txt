
add_library(mathieu_lib STATIC src/mathieu.cpp)
target_include_directories(mathieu_lib PUBLIC include ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(mathieu_lib PROPERTIES AUTOMOC OFF)
target_compile_features(mathieu_lib PUBLIC cxx_std_17)

# Install rules
install(TARGETS mathieu_lib
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
