add_executable(query_vertex_attribute query_vertex_attribute.cpp)
target_link_libraries(query_vertex_attribute PRIVATE glad::glad glfw)

add_executable(shader shader_1.cpp)
target_link_libraries(shader PRIVATE glad::glad glfw)

add_executable(shader_uniform shader_uniform.cpp)
target_link_libraries(shader_uniform PRIVATE glad::glad glfw)

add_executable(shaders_interpolation shaders_interpolation.cpp)
target_link_libraries(shaders_interpolation PRIVATE glad::glad glfw)

add_executable(shaders_class shaders_class.cpp)
target_link_libraries(shaders_class PRIVATE glad::glad glfw)

add_executable(shader_p1 shader_practice1.cpp)
target_link_libraries(shader_p1 PRIVATE glad::glad glfw)

add_executable(shader_p2 shader_practice2.cpp)
target_link_libraries(shader_p2 PRIVATE glad::glad glfw)

add_executable(shader_p3 shader_practice3.cpp)
target_link_libraries(shader_p3 PRIVATE glad::glad glfw)
