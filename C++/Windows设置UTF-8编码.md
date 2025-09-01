
在 Windows 上，使用 UTF-8 编码时，会有乱码问题，将所有环境统一为 UTF-8 可解决。

比如：使用 CLion 时，新环境上遇到编译控制台、输出控制台乱码的问题。

1. 首先设置编译选项： `/utf-8`
```cmake
if (MSVC) 
	target_compile_options(target PRIVATE /utf-8)
endif()
```
2. 设置本地化和输入输出控制台编码
```cpp
int main() {
#if _WIN32 // 是否是 Windows 系统
    setlocale(LC_ALL, ".utf-8");  // 设置标准库调用系统 API 所用的编码，用于 fopen，ifstream 等函数
    SetConsoleOutputCP(CP_UTF8); // 设置控制台输出编码，或者 system("chcp 65001") ，这里 CP_UTF8 = 65001
    SetConsoleCP(CP_UTF8); // 设置控制台输入编码，用于 std::cin
#endif
    
    
    std::cout << "你好，世界\n";   // OK
    std::ifstream fin("你好.txt"); // OK
    
    return 0;
}
```