提供一个布尔值选项，用户可以选择该选项。
```cmake
option(<variable> "<help_text>" [value])
```

如果没有提供初始的 `<value>`，则 `OFF` 是默认值。如果 `<variable>` 已经设置为普通变量或缓存变量，则该命令不执行任何操作（请参阅策略:policy:CMP0077）。


在 CMake 项目模式下，使用选项值创建布尔缓存变量。在 CMake 脚本模式下，布尔变量设置有选项值。