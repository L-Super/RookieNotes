add_executable(transform transform.cpp)
target_link_libraries(transform PRIVATE glad::glad glfw fmt::fmt-header-only)