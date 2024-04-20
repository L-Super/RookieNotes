`target_sources()` 函数用于向 CMake 目标添加源文件。目标可以是可执行文件、库或自定义目标。源文件可以是 C/C++ 源代码、头文件、资源文件等。

```
target_sources(<target>
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

- `<target>`: 要添加源文件的目标名称。
- `<scope>`: 源文件的可见范围，可以是以下之一：
    - `PRIVATE`: 源文件仅对目标本身可见。
    - `PUBLIC`: 源文件对目标本身及其依赖项可见。
    - `INTERFACE`: 源文件仅对目标的依赖项可见。
- `<item>`: 要添加的源文件，可以是单个文件或文件列表。

示例：

```cmake
add_executable(my_app main.c)

target_sources(my_app PRIVATE utils.c)
target_sources(my_app PUBLIC api.h)
```

`main.c` 是 `my_app` 可执行文件的源文件。`utils.c` 是私有源文件，仅对 `my_app` 可执行文件本身可见。`api.h` 是公共头文件，对 `my_app` 可执行文件及其依赖项可见。

**注释**

- `target_sources()` 函数可以多次调用，以向目标添加多个源文件。
- 源文件的路径可以是相对路径或绝对路径。
- 相对路径是相对于当前源目录的。
- 可以使用生成器表达式来指定源文件。

**错误**

- 如果 `<target>` 不是有效的目标，则会发生错误。
- 如果 `<scope>` 不是有效的可见范围，则会发生错误。
- 如果 `<item>` 不是有效的文件，则会发生错误。



> [target_sources() (runebook.dev)](https://runebook.dev/zh/docs/cmake/command/target_sources#command:target_sources)
> [target_sources — CMake 3.26.4](https://cmake-doc.readthedocs.io/zh-cn/latest/command/target_sources.html#command:target_sources)