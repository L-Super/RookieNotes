add_executable(transform transform.cpp)
target_link_libraries(transform PRIVATE glad::glad glfw fmt::fmt-header-only)

add_executable(transform1 transform1.cpp)
target_link_libraries(transform1 PRIVATE glad::glad glfw)