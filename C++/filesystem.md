`std::filesystem` 是 C++17 引入的标准文件系统库，它提供的核心能力包括：

- 路径表示：`path`
- 文件/目录判断：`exists`, `is_regular_file`, `is_directory`
- 路径拼接、拆分、规范化
- 创建/删除目录
- 文件拷贝、重命名、删除
- 目录遍历：`directory_iterator`, `recursive_directory_iterator`
- 获取文件大小、时间戳、权限

## `fs::path`

创建路径：

```cpp
#include <filesystem>
namespace fs = std::filesystem;

fs::path p1 = "dir/subdir";
fs::path p2 = "file.txt";

// 拼接路径
fs::path full = p1 / p2;


// 路径字符串形式
fs::path p = "dir/sub/file.txt";

p.string();          // 普通字符串表示
p.generic_string();  // 用 / 作为分隔符
p.native();          // 平台原生表示
```

路径拆分：

```cpp
fs::path p = "/home/user/docs/report.txt"; // or p = "C:/home/user/docs/report.txt"

p.root_name();       // Linux "/" | Windows "C:"
p.root_directory();  // "/"
p.root_path();       // "/" | "C:/"
p.relative_path();   // "home/user/docs/report.txt"
p.parent_path();     // "/home/user/docs" | "C:/home/user/docs"
p.filename();        // "report.txt"
p.stem();            // "report"
p.extension();       // ".txt"

//
fs::path p = "/tmp/test.tar.gz";

std::cout << p.filename() << '\n';   // test.tar.gz
std::cout << p.stem() << '\n';       // test.tar
std::cout << p.extension() << '\n';  // .gz
```

注意：`stem()` 只去掉最后一个扩展名。

## 非成员函数

### 路径是否存在判断，文件类型

```cpp
fs::path p = "test.txt";

if (fs::exists(p)) {
    std::cout << "exists\n";
}

if (fs::is_regular_file(p)) {
    std::cout << "regular file\n";
}

if (fs::is_directory(p)) {
    std::cout << "directory\n";
}

// error_code
std::error_code ec;
if (fs::exists(p, ec)) {
    std::cout << "exists\n";
}
if (ec) {
    std::cout << "error: " << ec.message() << '\n';
}
```

当前工作目录：

```cpp
fs::path cwd = fs::current_path();
std::cout << cwd << '\n';
```
注意：**当前工作目录是与进程关联的目录**，用作相对路径路径名解析的起始位置。在库中调用时，路径不准确。
设置当前目录：

```cpp
fs::current_path("/tmp");
```

绝对路径：

```cpp
fs::path p = "data/a.txt";
fs::path abs = fs::absolute(p); // F:\Code\filesystem\build\data\report.txt
```

注意：

- `absolute()` 不要求路径存在
- 它只是基于当前目录补全

> 对于基于 POSIX 的操作系统，`std::filesystem::absolute(p)` 与 `std::filesystem::current_path() / p` 等效，除非 `p` 是空路径。
>
> 在 Windows 上，`std::filesystem::absolute` 可能通过调用 `GetFullPathNameW` 来实现。

### 规范化路径

```cpp
fs::path p = "a/./b/../c";
// 词法规范化
std::cout << p.lexically_normal() << '\n';  // a/c


// 真实规范路径
fs::path real = fs::canonical(p);
```

区别：

- `lexically_normal()`：不访问文件系统
- `canonical()`：访问文件系统，要求路径存在，解析符号链接

更宽松的版本：

```cpp
fs::path real = fs::weakly_canonical(p);
```

### 创建和删除目录

创建单层目录：

```cpp
fs::create_directory("output");
```

递归创建目录：

```cpp
fs::create_directories("a/b/c");
```

### 删除

```cpp
fs::remove("a.txt");     // 删除文件或空目录
fs::remove_all("temp");  // 递归删除

// 安全尝试删除
std::error_code ec;
auto count = fs::remove_all("temp", ec);
if (ec) {
    std::cerr << "remove_all failed: " << ec.message() << '\n';
}
```

### 重命名、复制、文件大小

重命名

```cpp
fs::rename("old.txt", "new.txt");
```

也可以用于移动文件：

```cpp
fs::rename("a.txt", "backup/a.txt");
```

### 拷贝文件

```cpp
fs::copy_file("a.txt", "b.txt");
```

带选项：

```cpp
fs::copy_file("a.txt", "b.txt", fs::copy_options::overwrite_existing);
```

拷贝目录树：

```cpp
fs::copy("src_dir", "dst_dir", fs::copy_options::recursive);
```

### 文件大小

```cpp
auto n = fs::file_size("a.bin");
std::cout << n << '\n';
```


### 8. 目录遍历

非递归遍历

```cpp
for (const auto& entry : fs::directory_iterator(".")) {
    std::cout << entry.path() << '\n';
}
```

递归遍历

```cpp
for (const auto& entry : fs::recursive_directory_iterator(".")) {
    std::cout << entry.path() << '\n';
}
```


常见过滤

```cpp
for (const auto& entry : fs::directory_iterator(".")) {
    if (entry.is_regular_file() && entry.path().extension() == ".txt") {
        std::cout << entry.path() << '\n';
    }
}
```

##  C++20 的变化

需注意，C++20 新增 `char8_t` 作为 UTF-8 字符串表示

```cpp
// C++17 及以前
auto s = u8"测试";   // const char*

// C++20 起
auto s = u8"测试";   // const char8_t*
```


## Windows 配置建议

对于 MSVC：

```cmake
target_compile_options(my_target PRIVATE /utf-8)
target_compile_definitions(my_target PRIVATE UNICODE _UNICODE)
```

原因：

- Win32 API 默认映射到 `W` 版本
- 避免误用 ANSI API

## 综合示例

```cpp
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::path dir = "data";
    fs::path file = u8"测试.txt";
    fs::path full = dir / file;

    std::error_code ec;
    fs::create_directories(dir, ec);
    if (ec) {
        std::cerr << "create_directories failed: " << ec.message() << '\n';
        return 1;
    }

    {
        std::ofstream ofs(full, std::ios::binary);
        if (!ofs) {
            std::cerr << "open for write failed\n";
            return 1;
        }
        ofs << "hello\n";
    }

    {
        std::ifstream ifs(full, std::ios::binary);
        if (!ifs) {
            std::cerr << "open for read failed\n";
            return 1;
        }
        std::cout << "file exists and can be opened\n";
    }

    std::cout << "parent: " << full.parent_path() << '\n';
    std::cout << "filename: " << full.filename() << '\n';
    std::cout << "stem: " << full.stem() << '\n';
    std::cout << "extension: " << full.extension() << '\n';
}
```

