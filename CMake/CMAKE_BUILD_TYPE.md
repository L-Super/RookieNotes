`CMAKE_BUILD_TYPE` 用于指定项目应该以哪种预设配置进行编译。

常见的可选值有：

- `Debug` 启用调试符号（debugging symbols），通常关闭优化，方便调试。

- `Release` 启用优化（optimization），关闭调试符号，追求最高运行速度。

- `RelWithDebInfo` 启用优化，但仍包含调试符号，兼顾性能和有限的调试能力。

- `MinSizeRel` 启用优化，目标是生成最小体积的可执行文件。



在命令行指定：

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

在 `CMakeLists.txt` 中设置：

```cmake
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode.")
endif()
```



> [CMAKE_BUILD_TYPE — CMake 4.2.1 Documentation](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html)
>
> [告别编译困扰：掌握 CMAKE_BUILD_TYPE 的用法和替代方法](https://runebook.dev/zh/docs/cmake/envvar/cmake_build_type)