Win32 API 中 A 和 W 的区别

在 Win32 API 中，函数名后缀的 **“A”** 和 **“W”** 分别代表 ANSI 和 Unicode 版本。这两种版本的主要区别在于字符编码的处理方式。
### A 后缀 (ANSI 版本)

**A 后缀 (ANSI 版本)** 的函数使用 **多字节字符集 (MBCS)**，具体编码取决于系统当前的代码页（Code Page）。单个字符可能占 1~2 字节（DBCS）甚至更多。例如 `CreateWindowA` 接收 `const char*` 类型的 ANSI 字符串。
例如：
- 简体中文系统默认代码页为 **CP936 (GBK)**
- 日文系统默认代码页为 **CP932 (Shift-JIS)**
- 西欧系统默认代码页为 **CP1252**

**W 后缀 (Unicode 版本)** 的函数使用 **双字节字符集（DBCS）**，即 Unicode 编码（在 Windows 中通常是 UTF-16），每个编码单元固定 2 字节（`wchar_t`）。BMP 之外的字符使用代理对（surrogate pair），占 4 字节。Unicode 支持更广泛的字符集，适用于多语言环境。例如 `CreateWindowW` 接收 `const wchar_t*`（即 `LPCWSTR`）类型的 Unicode 字符串。

**Ex 后缀**的函数表示拓展版本，提供更多参数或更强功能。Ex 后缀同样存在 A/W 两个版本

### 无后缀函数的行为
如果调用没有后缀的函数（如 `CreateWindow`），其具体版本取决于编译时是否定义了 `UNICODE` / `_UNICODE` 宏。

在 Windows SDK 头文件中，典型实现如下：

```c
// winuser.h
#ifdef UNICODE
#define CreateWindow  CreateWindowW
#define CreateWindowEx  CreateWindowExW
#else
#define CreateWindow  CreateWindowA
#define CreateWindowEx  CreateWindowExA
#endif
```

- **`UNICODE`**：控制 Windows SDK 头文件（`<windows.h>` 等）中的 API 宏选择
- **`_UNICODE`**：控制 C 运行时库头文件（`<tchar.h>` 等）中的函数和宏选择

两者通常需要**同时定义或同时不定义**，否则可能出现不一致的问题。


- 定义了宏时，函数会默认使用 Unicode 版本（W 后缀的函数）。
- 未定义宏时，函数会默认使用 ANSI 版本（A 后缀的函数）。

**一般来说，ANSI 版本的实现会调用 Unicode 版本，并对 ANSI 参数和结构字段与 Unicode 之间进行必要的转换。**

在现代 Windows 系统中，**推荐使用 Unicode 版本（W 后缀）**，因为它支持更广泛的字符集，适合多语言开发，并且在性能和兼容性上更优。