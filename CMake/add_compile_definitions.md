将预处理器定义添加到源文件的编译中。
```cmake
add_compile_definitions(<definition> ...)
```
将预处理器定义添加到编译器命令行。

使用语法 `VAR` 或 `VAR=value` 指定定义。不支持函数样式定义。

> 不推荐使用 `add_definitions()` ，已以下命令替代：
> - 使用 `add_compile_definitions()` 添加预处理器定义。
> - 使用 `include_directories()` 添加包含目录。
> - 使用 `add_compile_options()` 添加其他选项。

**示例：**
定义平台宏：
```cmake
if(WIN32)
    add_compile_definitions(PLATFORM_WINDOWS)
elseif(APPLE)
    add_compile_definitions(PLATFORM_MAC)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(PLATFORM_LINUX)
endif()
```
定义变量：
```cmake
add_compile_definitions(DEFAULT_LOGLEVEL=1)
```