添加可执行文件
```
add_executable(<name> <options>... <sources>...)
```

添加一个名为 `name` 的可执行目标，以从命令调用中列出的源文件构建。 `<name>` 对应于逻辑目标名称，并且在项目中必须是全局唯一的。

option：
- **WIN32**：指定可执行文件为 Windows 平台的 PE 格式，参阅 WIN32_EXECUTABLE[^WIN32_EXECUTABLE]
- **MACOSX_BUNDLE**：指定可执行文件为 macOS 平台的应用程序包，参阅 MACOSX_BUNDLE [^MACOSX_BUNDLE]
- **POSITION_INDEPENDENT_CODE**：指定可执行文件为位置无关代码
- **LINK_LIBRARIES**：指定可执行文件的依赖项库

sources：
构建可执行目标文件所需要的源文件。也可通过 `target_sources()` 继续为可执行目标文件添加源文件，要求是在调用 `target_sources` 之前，可执行目标文件必须已经通过 ` add_executable()` 或 `add_library()` 定义。

示例：
```cmake
cmake_minimum_required(VERSION 3.20)
project(test)

add_executable(test main.cpp)
# 继续添加源文件
target_sources(test test.cpp)
```



[^WIN32_EXECUTABLE]: 当此属性设置为 true 时，这使它成为一个 GUI 可执行文件而不是控制台应用程序。

[^MACOSX_BUNDLE]: 当此属性设置为“TRUE”时，在 macOS 或 iOS 上构建的可执行文件将创建为应用程序包。这使它成为可以从 Finder 启动的 GUI 可执行文件。



> [添加可执行文件 — CMake 3.26.4](https://cmake-doc.readthedocs.io/zh-cn/latest/command/add_executable.html)