将链接目录添加到目标。
```cmake
target_link_directories(<target> [BEFORE]
<INTERFACE|PUBLIC|PRIVATE> [items1...]
[<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```
指定链接器在链接给定目标时应在其中搜索 libraries 的路径。每个项目可以是绝对路径或相对路径，后者被解释为相对于当前源目录。这些项目将被添加到链接命令中。

命名的 `<target>` 必须是由 `add_executable()` 或 `add_library()` 等命令创建的，并且不能是 `ALIAS target`。

需要 `INTERFACE` 、 `PUBLIC` 和 `PRIVATE` 关键字来指定其后面的项目的 [scope](https://runebook.dev/cn/docs/cmake/manual/cmake-buildsystem.7#target-usage-requirements) 。 `PRIVATE` 和 `PUBLIC` 项目将填充 `<target>` 的 [`LINK_DIRECTORIES`](https://runebook.dev/cn/docs/cmake/prop_tgt/link_directories#prop_tgt:LINK_DIRECTORIES) 属性。 `PUBLIC` 和 `INTERFACE` 项目将填充 `<target>` 的 [`INTERFACE_LINK_DIRECTORIES`](https://runebook.dev/cn/docs/cmake/prop_tgt/interface_link_directories#prop_tgt:INTERFACE_LINK_DIRECTORIES) 属性（ [IMPORTED targets](https://runebook.dev/cn/docs/cmake/manual/cmake-buildsystem.7#imported-targets) 仅支持 `INTERFACE` 项目）。每个项目指定一个链接目录，如有必要，在将其添加到相关属性之前将转换为绝对路径。重复调用相同的 `<target>` 按调用顺序附加项目。

如果指定了 `BEFORE` ，则内容将被添加到相关属性的前面，而不是附加到后面。

`target_link_directories` 的参数可以使用语法为 `$<...>` 的生成器表达式。

>  `target_link_directories` 与 `link_directories`
`link_directories` 会对当前 `CMakeLists.txt` 文件的目标文件生效，`target_link_directories` 则对指定的目标文件生效