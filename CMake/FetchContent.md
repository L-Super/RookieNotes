
该模块允许在配置时通过 ExternalProject 模块支持的任何方法填充内容。

## include (FetchContent) 
使用 include(FetchContent) 将模块添加到项目中
## FetchContent_Declare
配置依赖项。告知 FetchContent 依赖项的位 置，以及应该使用哪个版本。

```
FetchContent_Declare(
  <name>
  <contentOptions>...
  [SYSTEM]
  [OVERRIDE_FIND_PACKAGE |
   FIND_PACKAGE_ARGS args...]
)
```
`FetchContent_Declare()` 函数记录描述如何填充指定内容的选项。
+  `<name>` 可以是任何不带空格的字符串，但最好只使用字母、数字和下划线。该名称将不区分大小写处理
+ `<contentOptions>` 可以是 [`ExternalProject_Add()`](https://cmake-doc.readthedocs.io/zh-cn/latest/module/ExternalProject.html#command:externalproject_add "externalproject_add") 命令可以理解的任何下载、更新或补丁选项。配置、构建、安装和测试步骤被明确禁用，因此与它们相关的选项将被忽略。在大多数情况下，`<contentOptions>` 只是几个定义下载方法和特定于方法的详细信息

即使用 `FetchContent_Declare(MyName)` 从 URL、Git 仓库等地方获取软件包或数据。
## FetchContent_MakeAvailable
完成依赖项设置。这将下载、构建、安装， 并将列表文件添加到主项目以供解析。

```
FetchContent_MakeAvailable(<name1> [<name2>...])
```
此命令确保每个命名的依赖项在返回时都可供项目使用。

## 示例
声明某些依赖项的内容详细信息然后确保使用单独的调用填充它们的示例：
```cmake
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        605a34765aa5d5ecbf476b4598a862ada971b0cc # v3.0.1
)
FetchContent_Declare(
  myCompanyIcons
  URL      https://intranet.mycompany.com/assets/iconset_1.12.tar.gz
  URL_HASH MD5=5588a7b18261c20068beabfb4f530b87
)

# 在以下调用之后，由 googletest 和 Catch2将可用于构建的其余部分
FetchContent_MakeAvailable(googletest Catch2 myCompanyIcons)
```
[`FetchContent_MakeAvailable ()`]( https://cmake-doc.readthedocs.io/zh-cn/latest/module/FetchContent.html#command:fetchcontent_makeavailable "fetchcontent_makeavailable") 命令确保指定的依赖项已被填充，无论是通过较早的调用还是通过自己填充它们。执行填充时，如果可能的话，它还会将它们添加到主构建中，以便主构建可以使用填充项目的目标等。

有时，我们需要在 `CMakeLists.txt` 里关闭库的选项开关：
```cmake
cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0077 NEW)

set(LEVELDB_BUILD_TESTS OFF CACHE BOOL "Don't build leveldb tests")  
set(LEVELDB_BUILD_BENCHMARKS OFF CACHE BOOL "Don't build leveldb benchmarks")  
set(LEVELDB_INSTALL OFF CACHE BOOL "Don't install leveldb")

include(FetchContent)
FetchContent_Declare(
  leveldb
  GIT_REPOSITORY https://github.com/google/leveldb.git
  GIT_TAG main
)

FetchContent_MakeAvailable(leveldb)
```
注意：这些变量需要以 **CACHE BOOL** 形式设置，否则 leveldb 项目里的 option(...) 会覆盖掉你设置的值（尤其在没有 CMP0077 NEW 的时候）