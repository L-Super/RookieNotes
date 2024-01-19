add_executable(shader shader.cpp)
target_link_libraries(shader PRIVATE glad::glad glfw)