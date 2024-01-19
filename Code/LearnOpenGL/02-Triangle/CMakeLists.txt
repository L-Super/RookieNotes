add_executable(triangle triangle.cpp)
target_link_libraries(triangle PRIVATE glad::glad glfw)

add_executable(triangle1 triangle_1.cpp)
target_link_libraries(triangle1 PRIVATE glad::glad glfw)

add_executable(triangle_p1 triangle_practice1.cpp)
target_link_libraries(triangle_p1 PRIVATE glad::glad glfw)

add_executable(triangle_p2 triangle_practice2.cpp)
target_link_libraries(triangle_p2 PRIVATE glad::glad glfw)

add_executable(triangle_p3 triangle_practice3.cpp)
target_link_libraries(triangle_p3 PRIVATE glad::glad glfw)