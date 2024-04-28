add_executable (example "example.cpp")
set_property(TARGET example PROPERTY CXX_STANDARD 20)
target_link_libraries(example PUBLIC lunaldtk)

add_custom_command(
    TARGET example
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets/ $<TARGET_FILE_DIR:example>/assets/
)