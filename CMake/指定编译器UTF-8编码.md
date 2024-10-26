```cmake
add_compile_options(/utf-8)

target_compile_options(target PUBLIC /utf-8)
```
两个都可，区别在于作用范围：
1. `add_compile_options (/utf-8)`：
- 这个命令是全局的，设置后会将 `/utf-8` 编译选项应用到当前 CMake 目录及其子目录中定义的所有目标。
- 如果有多个目标且您希望所有目标都使用 `/utf-8` 编译选项，这种方式会更方便，但可能会影响一些不需要此选项的目标。
2. `target_compile_options(target PUBLIC /utf-8)`：
- 将编译选项 /utf-8 应用于特定目标。
- 选项的作用范围由 PUBLIC 指定，这意味着这个选项不仅会应用于构建 target 本身，还会传递给依赖 target 的其他目标（即使用 target_link_libraries 关联的目标）。
- 使用这种方式可以确保选项仅限于特定的目标，其他目标不会受到影响。这种方式更精确，适合在大型项目中管理编译选项。

