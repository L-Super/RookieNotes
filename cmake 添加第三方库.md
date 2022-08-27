使用FetchContent的步骤:

```bash
 1. 在cmake文件写入  include(FetchContent) ，具体看完整实例.
 2. 使用FetchContent_Declare(三方库) 获取项目。可以是一个URL也可以是一个Git仓库。
 3. 使用FetchContent_MakeAvailable(三方库) 获取我们需要库,然后引入项目。
 4. 使用 target_link_libraries(项目名PRIVATE 三方库::三方库)
```

```cmake
include(FetchContent)  
FetchContent_Declare(fmt  
        URL https://github.com/fmtlib/fmt/archive/refs/tags/9.0.0.tar.gz)  
FetchContent_MakeAvailable(fmt)

target_link_libraries(project PRIVATE fmt)
```