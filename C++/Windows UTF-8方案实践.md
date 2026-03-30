在 C++ 20 中，增加了 `char8_t` 类型用于表示 UTF-8 数据。在 Windows 平台，采用 UTF-8 + Unicode 编码方案实践。

## 1. 项目配置 (CMakeLists. txt)

```cmake
if (MSVC)
    target_compile_options(your_target PRIVATE /utf-8)
    target_compile_definitions(your_target PRIVATE UNICODE _UNICODE)
endif ()
```

| 选项 | 作用 |
|------|------|
| `/utf-8` | 源文件和**执行字符集**都用 UTF-8 (`/source-charset:utf-8 /execution-charset:utf-8`) |
| `UNICODE _UNICODE` | Win32 API 宏默认展开为 `W` 版本 (如 `CreateFile` -> `CreateFileW`) |

二者**不冲突**：`/utf-8` 管的是 C++ 编译器内部字符串编码，`UNICODE` 管的是 Windows 头文件的 API 选择。

---

## 2. 核心原则：三种字符串各司其职

```
std::string  (char)        -- 存储 UTF-8 数据，用于业务逻辑和 I/O
std::wstring (wchar_t)     -- 仅在调用 Win32 W API 时临时转换
std::u8string (char8_t)    -- C++20 的 u8"..." 字面量类型
```

**日常开发只用 `std::string` 存 UTF-8，需要调 Windows API 时通过 `fs::path` 中转为 `wstring`。**

---

## 3. C++20 的 char8_t 陷阱与应对

C++20 起 `u8"你好"` 的类型是 `const char8_t[]`，**不能**隐式转为 `const char*`，也**不能**与 `std::string` 直接比较或输出到 `std::cout`。

### 方案 A：转换函数 (推荐)

```cpp
inline std::string u8_to_str(const std::u8string& s) {
    return {s.begin(), s.end()};
}

// 用法
std::string name = u8_to_str(path.u8string());
```

### 方案 B：直接用 char 字面量 + `/utf-8`

因为 `/utf-8` 已经把执行字符集设为 UTF-8，所以**普通字符串字面量本身就是 UTF-8**：

```cpp
// 在 /utf-8 下，这两者编码相同：
const char* a = "你好世界";     // OK, 已经是 UTF-8
const char8_t* b = u8"你好世界"; // C++20 类型安全，但不方便
```

**实践结论**：在已配置 `/utf-8` 的项目中，直接用 `"你好"` 即可，无需写 `u8"你好"`。`u8` 前缀主要用于需要跨平台保证编码安全的场景。

---

## 4. 文件系统操作

### 4.1 创建 path

```cpp
// 以下三种方式等价（/utf-8 环境下）
fs::path p1("测试文件.txt");          // char 字面量，/utf-8 下就是 UTF-8
fs::path p2(u8"测试文件.txt");        // char8_t 字面量，C++20 有专门的构造函数
fs::path p3(L"测试文件.txt");         // 宽字符字面量
```

### 4.2 读写文件

```cpp
fs::path p("中文文件.txt");

// 写
std::ofstream ofs(p, std::ios::binary);  // 直接传 path，底层走 W API
ofs << "UTF-8 内容";
ofs.close();

// 读
std::ifstream ifs(p, std::ios::binary);
std::string content((std::istreambuf_iterator<char>(ifs)),
                     std::istreambuf_iterator<char>());
```

### 4.3 目录遍历

```cpp
for (const auto& entry : fs::recursive_directory_iterator("测试目录")) {
    std::cout << entry.path().string() << "\n";  // /utf-8 下 .string() 返回 UTF-8
}
```

---

## 5. 调用 Win32 W API

关键：**用 `path::wstring()` 获取宽字符串传给 W API**。

```cpp
fs::path p("测试文件.txt");
std::wstring wp = p.wstring();  // UTF-8 -> UTF-16 转换由 path 内部完成

// CreateFileW
HANDLE h = CreateFileW(wp.c_str(), GENERIC_WRITE, 0, nullptr,
                       CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
```

从 W API 拿回来的 `wchar_t*` 结果，用 `fs::path` 包装回来：

```cpp
// Win32 返回的宽字符 -> 回到 UTF-8 世界
fs::path result_path(fd.cFileName);             // wchar_t* -> path
std::string utf8_name = result_path.string();   // path -> UTF-8 string (/utf-8 下)
```

---

## 6. 控制台输出

Windows 控制台默认不是 UTF-8，需要在 `main()` 入口设置：

```cpp
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
```

或者在 Windows Terminal 中默认支持 UTF-8。

## 7. 常见错误

```cpp
// ERROR: C++20 中 u8"..." 是 char8_t[]，不能直接输出
std::cout << u8"你好";                    // 编译错误

// ERROR: std::string 不能和 char8_t[] 比较
std::string s = "hello";
if (s == u8"hello") {}                    // 编译错误

// ERROR: 用 char* 接 u8 字面量
const char* p = u8"你好";                 // C++20 编译错误

// CORRECT: 在 /utf-8 下直接用普通字面量
std::cout << "你好";                      // OK
const char* p = "你好";                   // OK, 就是 UTF-8
```