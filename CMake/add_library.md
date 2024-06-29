使用指定的源文件将库添加到项目中。

## 普通库
```
add_library(<name> [STATIC | SHARED | MODULE]
            [EXCLUDE_FROM_ALL]
            [<source>...])
```
若没有指定 STATIC 还是 SHARED，且 `BUILD_SHARED_LIBS` 变量没有设置为 ON，将**默认生成一个静态库**。

 CMake 期望 `SHARED` 库在 Windows 上始终具有关联的导入库，所以 `SHARED` **默认导出任何符号**。
## 对象库
```
add_library(<name> OBJECT [<source>...])
```

创建一个 [Object Library](https://cmake-doc.readthedocs.io/zh-cn/latest/manual/cmake-buildsystem.7.html#object-libraries)。目标库编译源文件但不将它们的目标文件存档或链接到库中。

由 `add_library` 或 [`add_executable ()`](add_executable.md) 创建的其他目标可以使用 [`$<TARGET_OBJECTS:objlib>`]( https://cmake-doc.readthedocs.io/zh-cn/latest/manual/cmake-generator-expressions.7.html#genex:TARGET_OBJECTS "TARGET_OBJECTS") 形式的表达式作为源引用对象，其中 `` objlib`` 是对象库名称。例如：

```cmake
add_library(... $<TARGET_OBJECTS:objlib> ...)
add_executable(... $<TARGET_OBJECTS:objlib> ...)
```


将 objlib 的目标文件包含在一个库和一个可执行文件中，以及那些从他们自己的源代码编译的文件。对象库可能只包含编译源、头文件和其他不会影响普通库链接的文件（例如 `.txt`）。

或者，对象库可以链接到其他目标：
```cmake
add_library(archive OBJECT archive.cpp zip.cpp lzma.cpp)

add_library(archiveExtras STATIC extras.cpp)
target_link_libraries(archiveExtras PUBLIC archive)

add_executable(test_exe test.cpp)
target_link_libraries(test_exe archive)
```

## 接口库

```
add_library(<name> INTERFACE)
```

创建一个 [Interface Library](https://cmake-doc.readthedocs.io/zh-cn/latest/manual/cmake-buildsystem.7.html#interface-libraries)。 `INTERFACE` 库目标不会编译源代码，也不会在磁盘上生成库工件。但是，它可能设置了属性，并且可以安装和导出。通常，使用以下命令在接口目标上填充 `INTERFACE_*` 属性：

- [`set_property()`](https://cmake.org/cmake/help/latest/command/set_property.html#command:set_property),
- [`target_link_libraries(INTERFACE)`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries),
- [`target_link_options(INTERFACE)`](https://cmake.org/cmake/help/latest/command/target_link_options.html#command:target_link_options),
- [`target_include_directories(INTERFACE)`](https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories),
- [`target_compile_options(INTERFACE)`](https://cmake.org/cmake/help/latest/command/target_compile_options.html#command:target_compile_options),
- [`target_compile_definitions(INTERFACE)`](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions), and
- [`target_sources(INTERFACE)`](https://cmake.org/cmake/help/latest/command/target_sources.html#command:target_sources),

然后它像任何其他目标一样用作 [`target_link_libraries()`](https://cmake-doc.readthedocs.io/zh-cn/latest/command/target_link_libraries.html#command:target_link_libraries "target_link_libraries") 的参数。

使用上述签名创建的接口库本身没有源文件，也不会作为目标包含在生成的构建系统中。

## 导入库

```
add_library(<name> <type> IMPORTED [GLOBAL])
```

创建一个名为 `<name>` 的 [IMPORTED 库目标](https://cmake-doc.readthedocs.io/zh-cn/latest/manual/cmake-buildsystem.7.html#imported-targets)。没有生成规则来构建它，并且 [`IMPORTED`](https://cmake-doc.readthedocs.io/zh-cn/latest/prop_tgt/IMPORTED.html#prop_tgt:IMPORTED) 目标属性是 `True`。目标名称在其创建目录及以下目录中具有范围，但 `GLOBAL` 选项扩展了可见性。它可以像项目中构建的任何目标一样被引用。 `IMPORTED` 库可用于方便地从 [`target_link_libraries()`](https://cmake-doc.readthedocs.io/zh-cn/latest/command/target_link_libraries.html#command:target_link_libraries) 等命令中引用。

`<type>` 必须是以下之一：

- `STATIC`, `SHARED`, `MODULE`, `UNKNOWN`

  引用位于项目外部的库文件。 [`IMPORTED_LOCATION`](https://cmake-doc.readthedocs.io/zh-cn/latest/prop_tgt/IMPORTED_LOCATION.html#prop_tgt:IMPORTED_LOCATION) 目标属性（或其每个配置变体 [`IMPORTED_LOCATION_`](https://cmake-doc.readthedocs.io/zh-cn/latest/prop_tgt/IMPORTED_LOCATION_CONFIG.html#prop_tgt:IMPORTED_LOCATION_)）指定主库文件在磁盘上的位置：

  对于大多数非 Windows 平台上的“共享”库，主库文件是链接器和动态加载器使用的“. so”或“. dylib”文件。如果引用的库文件有一个 `SONAME`（或者在 macOS 上，有一个 ``LC_ID_DYLIB` 从 `@rpath/` 开始），该字段的值应该在 `IMPORTED_SONAME 中设置` 目标属性。如果引用的库文件没有 `SONAME`，但平台支持它，则应设置 [`IMPORTED_NO_SONAME`](https://cmake-doc.readthedocs.io/zh-cn/latest/prop_tgt/IMPORTED_NO_SONAME.html#prop_tgt:IMPORTED_NO_SONAME) 目标属性。

  对于 Windows 上的 `SHARED` 库， [`IMPORTED_IMPLIB`](https://cmake-doc.readthedocs.io/zh-cn/latest/prop_tgt/IMPORTED_IMPLIB.html#prop_tgt:IMPORTED_IMPLIB) 目标属性（或其每个配置变体 [`IMPORTED_IMPLIB_`](https://cmake.org/cmake/help/latest/prop_tgt/IMPORTED_IMPLIB_CONFIG.html#prop_tgt:IMPORTED_IMPLIB_)）指定 DLL 导入库文件 (`. lib` 或 `.dll.a`) 在磁盘上，而 `IMPORTED_LOCATION` 是 `.dll` 运行时库的位置（并且是可选的，但 `TARGET_RUNTIME_DLLS 需要` 生成器表达式）。


额外的使用要求可以在 `INTERFACE_*` 属性中指定。

`UNKNOWN` 库类型通常仅用于 [查找模块](https://cmake-doc.readthedocs.io/zh-cn/latest/manual/cmake-developer.7.html#find-modules) 的实现。它允许使用导入库的路径（通常使用 [`find_library()`](https://cmake-doc.readthedocs.io/zh-cn/latest/command/find_library.html#command:find_library) 命令找到），而不必知道它是什么类型的库。这在 Windows 上特别有用，其中静态库和 DLL 的导入库都具有相同的文件扩展名。

- `OBJECT`

  引用位于项目外部的一组目标文件。 [`IMPORTED_OBJECTS`](https://cmake-doc.readthedocs.io/zh-cn/latest/prop_tgt/IMPORTED_OBJECTS.html#prop_tgt:IMPORTED_OBJECTS) 目标属性（或其每个配置变体 [`IMPORTED_OBJECTS_`](https://cmake-doc.readthedocs.io/zh-cn/latest/prop_tgt/IMPORTED_OBJECTS_CONFIG.html#prop_tgt:IMPORTED_OBJECTS_)）指定目标文件在磁盘上的位置。额外的使用要求可以在 `INTERFACE_*` 属性中指定。

- `INTERFACE`

  不引用磁盘上的任何库或对象文件，但可以在 `INTERFACE_*` 属性中指定使用要求。

## 别名库

```
add_library(<name> ALIAS <target>)
```

创建一个 [Alias Target](https://cmake-doc.readthedocs.io/zh-cn/latest/manual/cmake-buildsystem.7.html#alias-targets)，这样 `<name>` 可用于在后续命令中引用 `<target>`。 `<name>` 不会作为生成目标出现在生成的构建系统中。 `<target>` 可能不是 `ALIAS`。


`ALIAS` 目标可以用作可链接的目标和从中读取属性的目标。也可以使用常规的 `if(TARGET)` 子命令来测试它们是否存在。 `<name>` 不得用于修改 `<target>` 的属性，也就是说，它不得用作 [`set_property()`](https://cmake-doc.readthedocs.io/zh-cn/latest/command/set_property.html#command:set_property)、 [`set_target_properties()`](https://cmake-doc.readthedocs.io/zh-cn/latest/command/set_target_properties.html#command:set_target_properties) 的操作数， [`target_link_libraries()`](https://cmake-doc.readthedocs.io/zh-cn/latest/command/target_link_libraries.html#command:target_link_libraries) 等 `ALIAS` 目标可能无法安装或导出。