## CMake 设置版本号

### 方法一

直接在 `project()` 指令里设置版本号：

```cmake
project(Tutorial VERSION 1.0.0)
```

当 `project()` 命令被调用时，CMake 会定义 `Tutorial_VERSION_MAJOR` 、 `Tutorial_VERSION_MINOR`、`Tutorial_VERSION_PATCH`。与 CMake 的 `PROJECT_VERSION` 的参数一致。

比如：
```cmake
project(Demo VERSION 1.0.5)

set(CMAKE_CXX_STANDARD 17)

configure_file(version.h.in version.h)

message(STATUS "Version: ${PROJECT_VERSION} ${PROJECT_VERSION_MAJOR} ${PROJECT_VERSION_MINOR}")
message(STATUS "Version: ${Demo_VERSION_MAJOR}.${Demo_VERSION_MINOR}.${Demo_VERSION_PATCH}")
```
输出：
```
-- Version: 1.0.5 1 0
-- Version: 1.0.5
```
然后创建 `version.h.in` 文件：
```
// method 1
#define VERSION_MAJOR @PROJECT_VERSION_MAJOR@  
#define VERSION_MINOR @PROJECT_VERSION_MINOR@  
#define VERSION_PATCH @PROJECT_VERSION_PATCH@
#define VERSION "@PROJECT_VERSION@"
// method 2  
#define VERSION_MAJOR @Demo_VERSION_MAJOR@  
#define VERSION_MINOR @Demo_VERSION_MINOR@  
#define VERSION_PATCH @Demo_VERSION_PATCH@
```
两种方式生成版本宏均可。第一种可以更方便生成版本号字符串。
### 方法二

在CMakeLists.txt中

```cmake
set(VERSION_MAJOR 1) 
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

configure_file(version.h.in version.h @ONLY)
```

然后创建version.h.in文件

```
#define VERSION_MAJOR @VERSION_MAJOR@
#define VERSION_MINOR @VERSION_MINOR@  
#define VERSION_PATCH @VERSION_PATCH@
#define VERSION @VERSION@
```

`configure_file` 命令会使用 CMake 中的变量替换 `version.h.in` 中的变量，生成 `version.h` 头文件。

version. h 默认生成在可执行文件路径下，也可指定生成的路径，如生成到 `${CMAKE_SOURCE_DIR}` 路径下：

```cmake
configure_file(version.h.in ${CMAKE_SOURCE_DIR}/version.h)
```

## Windows 版本信息

使用此方法对Windows可执行文件写入版本信息，需要注意版本号格式。一个文件版本是逗号分隔的数字，一个产品版本是点分隔的字符串。

```cmake
set(VERSION "${VERSION_MAJOR},${VERSION_MINOR},${VERSION_PATCH}")
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
```

```
//version.h.in
#define VERSION @VERSION@
#define VERSION_STR "@VERSION_STR@"
```

## Linux 查看版本信息

在Linux下:

- 使用file命令查看文件信息:

```shell
file your_executable
```

- 如果版本信息存在,会显示在输出中,例如:

```
your_executable: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=..., stripped
```

这里可以看到版本号是"version 1"。

- 也可以使用objdump命令查看版本信息:

```shell
objdump -p your_executable | grep -i version
```

输出类似:

```
version 1 (current)
```