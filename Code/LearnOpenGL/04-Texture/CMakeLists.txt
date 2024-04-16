add_executable(texture texture.cpp)
target_link_libraries(texture PRIVATE glad::glad glfw)

add_executable(texture1 texture1.cpp)
target_link_libraries(texture1 PRIVATE glad::glad glfw)