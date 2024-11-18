添加自定义命令
`add_custom_command` 有两个主要签名。
## 生成文件
用于添加自定义命令以生成输出
```
add_custom_command(OUTPUT output1 [output2 ...]
                   COMMAND command1 [ARGS] [args1...]
                   [COMMAND command2 [ARGS] [args2...] ...]
                   [MAIN_DEPENDENCY depend]
                   [DEPENDS [depends...]]
                   [BYPRODUCTS [files...]]
                   [IMPLICIT_DEPENDS <lang1> depend1
                                    [<lang2> depend2] ...]
                   [WORKING_DIRECTORY dir]
                   [COMMENT comment]
                   [DEPFILE depfile]
                   [JOB_POOL job_pool]
                   [VERBATIM] [APPEND] [USES_TERMINAL]
                   [COMMAND_EXPAND_LISTS])
```
生成指定 `OUTPUT` 文件的命令。

选项：
-  `APPEND`：将 `COMMAND` 和 `DEPENDS` 选项值附加到指定第一个输出的自定义命令。之前必须已经调用过此命令并具有相同的输出。如果前一个调用通过生成器表达式指定了输出，则当前调用指定的输出必须在计算生成器表达式后至少在一个配置中匹配。在这种情况下，附加的命令和依赖项适用于所有配置。

-  `BYPRODUCTS`：指定命令预期生成的文件，但其修改时间可能比依赖项更新，也可能不更新。如果副产品名称是相对路径，它将相对于当前源目录对应的构建树目录进行解释。每个副产品文件将自动标记有 [`GENERATED`](https://runebook.dev/cn/docs/cmake/prop_sf/generated#prop_sf:GENERATED) 源文件属性。

-  `COMMAND`：指定在构建时执行的命令行。如果指定了多个 `COMMAND` ，它们将按顺序执行，但不一定组成有状态 shell 或批处理脚本。


-  `COMMENT`：在构建时执行命令之前显示给定的消息。

-  `DEPENDS`：指定命令所依赖的文件。每个参数都转换为依赖项，如下所示：
   1. 如果参数是目标的名称（由 [`add_custom_target()`](https://runebook.dev/cn/docs/cmake/command/add_custom_target#command:add_custom_target) 、 [`add_executable()`](https://runebook.dev/cn/docs/cmake/command/add_executable#command:add_executable) 或 [`add_library()`](https://runebook.dev/cn/docs/cmake/command/add_library#command:add_library) 命令创建），则会创建目标级依赖项以确保在使用此自定义命令的任何目标之前构建该目标。此外，如果目标是可执行文件或库，则会创建文件级依赖项，以便在重新编译目标时重新运行自定义命令。
   1. 如果参数是绝对路径，则会在该路径上创建文件级依赖项。
   1. 如果参数是已添加到目标或已设置源文件属性的源文件的名称，则会在该源文件上创建文件级依赖项。
   1. 如果参数是相对路径并且存在于当前源目录中，则会在当前源目录中的该文件上创建文件级依赖关系。
   1. 否则，将在相对于当前二进制目录的该路径上创建文件级依赖关系。


如果任何依赖项是同一目录（ `CMakeLists.txt` 文件）中另一个自定义命令的 `OUTPUT` ，则 CMake 会自动将另一个自定义命令带入构建此命令的目标中。

如果未指定 `DEPENDS` ，则只要 `OUTPUT` 丢失，该命令就会运行；如果该命令实际上并未创建 `OUTPUT` ，则该规则将始终运行。

-  `COMMAND_EXPAND_LISTS`：`COMMAND` 参数中的列表将被扩展，包括使用 [`generator expressions`]( https://runebook.dev/cn/docs/cmake/manual/cmake-generator-expressions.7#manual:cmake-generator-expressions (7)) 创建的列表，从而允许正确扩展 `COMMAND` 参数（例如 `${CC} "-I$<JOIN:$<TARGET_PROPERTY:foo,INCLUDE_DIRECTORIES>,;-I>" foo.cc` ）。

-  `IMPLICIT_DEPENDS`：请求扫描输入文件的隐式依赖关系。给定的语言指定应使用其相应依赖关系扫描器的编程语言。目前仅支持 `C` 和 `CXX` 语言扫描仪。必须为 `IMPLICIT_DEPENDS` 列表中的每个文件指定语言。通过扫描发现的依赖项会在构建时添加到自定义命令的依赖项中。请注意， `IMPLICIT_DEPENDS` 选项当前仅支持 Makefile 生成器，其他生成器将忽略该选项。

Note
该选项不能与 `DEPFILE` 选项同时指定。

-  `JOB_POOL`：为 [`Ninja`](https://cmake.org/cmake/help/v3.26/generator/Ninja.html#generator:Ninja) 发电机指定 [`pool`](https://runebook.dev/cn/docs/cmake/prop_gbl/job_pools#prop_gbl:JOB_POOLS) 。与 `USES_TERMINAL` 不兼容，这意味着 `console` 池。使用 [`JOB_POOLS`](https://runebook.dev/cn/docs/cmake/prop_gbl/job_pools#prop_gbl:JOB_POOLS) 未定义的池会导致 ninja 在构建时出错。

-  `MAIN_DEPENDENCY`：指定命令的主要输入源文件。这就像为 `DEPENDS` 选项赋予的任何值一样处理，但也向 Visual Studio 生成器建议在何处挂起自定义命令。每个源文件最多可以有一个命令将其指定为其主要依赖项。编译命令（即 library 或可执行文件）被视为隐式主要依赖项，它会被自定义命令规范默默地覆盖。

- `OUTPUT`：指定命令预期生成的输出文件。每个输出文件将自动标记有 [`GENERATED`](https://runebook.dev/cn/docs/cmake/prop_sf/generated#prop_sf:GENERATED) 源文件属性。如果自定义命令的输出实际上并未作为磁盘上的文件创建，则应使用 [`SYMBOLIC`](https://runebook.dev/cn/docs/cmake/prop_sf/symbolic#prop_sf:SYMBOLIC) 源文件属性对其进行标记。


-  `USES_TERMINAL`：如果可能的话，该命令将直接访问终端。对于 [`Ninja`](https://cmake.org/cmake/help/v3.26/generator/Ninja.html#generator:Ninja) 生成器，这会将命令放入 `console` [`pool`](https://runebook.dev/cn/docs/cmake/prop_gbl/job_pools#prop_gbl:JOB_POOLS) 中。

-  `VERBATIM`：命令的所有参数都将为构建工具正确转义，以便调用的命令接收每个参数不变。请注意，在 `add_custom_command` 甚至看到参数之前， CMake 语言处理器仍然使用一级转义。建议使用 ` VERBATIM ` ，因为它可以实现正确的行为。当未给出 ` VERBATIM ` 时，该行为是特定于平台的，因为没有对特定于工具的特殊字符的保护。

- `WORKING_DIRECTORY`：使用给定的当前工作目录执行命令。如果它是相对路径，它将相对于当前源目录对应的构建树目录进行解释。

-  `DEPFILE`：指定一个包含自定义命令依赖项的 depfile。

### 示例
自定义命令可用于生成源文件。

添加自定义命令来运行 `someTool` 以生成 `out.c` ，然后将生成的源编译为库的一部分。每当 `in.txt` 发生更改时，生成规则都会重新运行。
```cmake
add_custom_command(
  OUTPUT out.c
  COMMAND someTool -i ${CMAKE_CURRENT_SOURCE_DIR}/in.txt
                   -o out.c
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/in.txt
  VERBATIM)
add_library(myLib out.c)

```
可以使用生成器表达式来指定每个配置的输出。

添加自定义命令来运行 `someTool` 以生成 `out-<config>.c` ，其中 `<config>` 是构建配置，然后将生成的源编译为库的一部分。
```cmake
add_custom_command(
  OUTPUT "out-$<CONFIG>.c"
  COMMAND someTool -i ${CMAKE_CURRENT_SOURCE_DIR}/in.txt
                   -o "out-$<CONFIG>.c"
                   -c "$<CONFIG>"
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/in.txt
  VERBATIM)
add_library(myLib "out-$<CONFIG>.c")
```
## 建立事件
将自定义命令添加到目标，例如库或可执行文件。这对于在构建目标之前或之后执行操作很有用。
```
add_custom_command(TARGET <target>
                   PRE_BUILD | PRE_LINK | POST_BUILD
                   COMMAND command1 [ARGS] [args1...]
                   [COMMAND command2 [ARGS] [args2...] ...]
                   [BYPRODUCTS [files...]]
                   [WORKING_DIRECTORY dir]
                   [COMMENT comment]
                   [VERBATIM] [USES_TERMINAL]
                   [COMMAND_EXPAND_LISTS])
```
- PRE_BUILD：在目标内执行任何其他规则之前运行 (仅 Visual Studio 生成器; 对于其他的生成器，在 `PRE_LINK` 命令之前运行。)。
- PRE_LINK：在编译源代码之后但在链接二进制文件或运行静态库的库管理器或归档工具之前运行。
- POST_BUILD：在执行完目标中的所有其他规则后运​​行。

> 出于向后兼容性的原因，如果未给出此类关键字，则假定为 `POST_BUILD`

`POST_BUILD` 事件可用于在链接后对二进制文件进行后处理。
将运行 `someHasher` ，在链接后在可执行文件旁边生成一个 `.hash` 文件：
```cmake
add_executable(myExe myExe.c)
add_custom_command(
  TARGET myExe POST_BUILD
  COMMAND someHasher -i "$<TARGET_FILE:myExe>"
                     -o "$<TARGET_FILE:myExe>.hash"
  VERBATIM)
```
可以使用生成器表达式来指定每个配置的副产品。
在链接 `myPlugin` 之后运行 `someHasher` ，例如生成一个 `.c` 文件，其中包含检查 `myPlugin` 哈希值的代码， `myExe` 可执行文件可以使用该代码在加载之前对其进行验证。
```cmake
add_library(myPlugin MODULE myPlugin.c)
add_custom_command(
  TARGET myPlugin POST_BUILD
  COMMAND someHasher -i "$<TARGET_FILE:myPlugin>"
                     --as-code "myPlugin-hash-$<CONFIG>.c"
  BYPRODUCTS "myPlugin-hash-$<CONFIG>.c"
  VERBATIM)
add_executable(myExe myExe.c "myPlugin-hash-$<CONFIG>.c")
```



> https://cmake-doc.readthedocs.io/zh-cn/latest/command/add_custom_command.html