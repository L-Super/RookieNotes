在内部，Windows 应用程序使用 Unicode 的 UTF-16 实现。在 UTF-16 中，大多数字符由双字节代码标识。

许多 Windows API 函数具有“A” (ANSI) 和“W” (宽的 Unicode) 版本。 “A”版本处理基于 Windows 代码页的文本，而“W”版本处理 Unicode 文本。

## char、wchar_t

**`char`** 类型是 C 和 C++ 中的原始字符类型。 **`char`** 类型可存储 ASCII 字符集或任何 ISO-8859 字符集中的字符，以及多字节字符的单个字节，例如 Shift-JIS 或 Unicode 字符集的 UTF-8 编码。 

**`wchar_t`** 类型是实现定义的宽字符类型。 在 Microsoft 编译器中，它表示一个 16 位宽字符，用于存储编码为 UTF-16LE 的 Unicode（Windows 操作系统上的本机字符类型）。

在 C++ 标准库中，[`basic_string`](https://learn.microsoft.com/zh-cn/cpp/standard-library/basic-string-class) 类型专用于窄字符串和宽字符串。 字符的类型为 **`char`** 时，使用 `std::string`；字符的类型为 **`char8_t`** 时，使用 `std::u8string`；字符的类型为 **`char16_t`** 时，使用 `std::u16string`；字符的类型为 **`char32_t`** 时，使用 `std::u32string`；而字符的类型为 **`wchar_t`** 时，使用 `std::wstring`。

## MultiByteToWideChar 函数
将字符串映射到 UTF-16 (宽字符) 字符串。 字符串不一定来自多字节字符集。
```cpp
int MultiByteToWideChar(
  [in]            UINT                              CodePage,
  [in]            DWORD                             dwFlags,
  [in]            _In_NLS_string_(cbMultiByte)LPCCH lpMultiByteStr,
  [in]            int                               cbMultiByte,
  [out, optional] LPWSTR                            lpWideCharStr,
  [in]            int                               cchWideChar
);
```
+ [in] CodePage：用于执行转换的代码页。

| 值                | 含义                                                         |
| :---------------- | :----------------------------------------------------------- |
| **CP_ACP**        | 系统默认的 Windows ANSI 代码页。**注意** 此值在不同计算机上可能不同，即使在同一网络上也是如此。 可以在同一台计算机上对其进行更改，导致存储的数据无法恢复地损坏。 此值仅用于临时使用，永久存储应尽可能使用 UTF-16 或 UTF-8。 |
| **CP_MACCP**      | 当前系统 Macintosh 代码页。**注意** 此值在不同计算机上可能不同，即使在同一网络上也是如此。 可以在同一台计算机上对其进行更改，导致存储的数据无法恢复地损坏。 此值仅用于临时使用，永久存储应尽可能使用 UTF-16 或 UTF-8。 **注意** 此值主要在旧代码中使用，通常不需要此值，因为新式 Macintosh 计算机使用 Unicode 进行编码。 |
| **CP_OEMCP**      | 当前系统 OEM 代码页。**注意** 此值在不同计算机上可能不同，即使在同一网络上也是如此。 可以在同一台计算机上对其进行更改，导致存储的数据无法恢复地损坏。 此值仅用于临时使用，永久存储应尽可能使用 UTF-16 或 UTF-8。 |
| **CP_SYMBOL**     | 符号代码页 (42) 。                                           |
| **CP_THREAD_ACP** | 当前线程的 Windows ANSI 代码页。**注意** 此值在不同计算机上可能不同，即使在同一网络上也是如此。 可以在同一台计算机上对其进行更改，导致存储的数据无法恢复地损坏。 此值仅用于临时使用，永久存储应尽可能使用 UTF-16 或 UTF-8。 |
| **CP_UTF7**       | UTF-7。 仅当受 7 位传输机制强制使用此值。 最好使用 UTF-8。   |
| **CP_UTF8**       | UTF-8。                                                      |

+ [in] dwFlags：指示转换类型的标志。 应用程序可以指定以下值的组合，MB_PRECOMPOSED为默认值。 MB_PRECOMPOSED和MB_COMPOSITE是互斥的。 无论其他标志的状态如何，都可以设置MB_USEGLYPHCHARS和MB_ERR_INVALID_CHARS。

| 值                       | 含义                                                         |
| :----------------------- | :----------------------------------------------------------- |
| **MB_COMPOSITE**         | 始终使用分解字符，即基字符和一个或多个非节奏字符各具有不同码位值的字符。 例如，Ä 由 A + ー：拉丁文大写字母 A (U+0041) + 组合 DIAERESIS (U+0308) 表示。 请注意，此标志不能与 MB_PRECOMPOSED 一起使用。 |
| **MB_ERR_INVALID_CHARS** | 如果遇到无效的输入字符，则失败。  从 Windows Vista 开始，如果应用程序未设置此标志，函数不会删除非法代码点，而是使用 U+FFFD 替换非法序列， (根据指定的代码页) 编码。  **Windows 2000 SP4 及更高版本、Windows XP：** 如果未设置此标志，函数会无提示删除非法码位。 调用 [GetLastError](https://learn.microsoft.com/zh-cn/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror) 将返回ERROR_NO_UNICODE_TRANSLATION。 |

> 对于下面列出的代码页， *dwFlags* 必须设置为 `0`。 否则，函数将失败并 **ERROR_INVALID_FLAGS**。
>
> - 50220
> - 50221
> - 50222
> - 50225
> - 50227
> - 50229
> - 57002 到 57011
> - 65000 (UTF-7)
> - 42 (符号)
>
>  对于 UTF-8 或代码页 54936 (GB18030，从 Windows Vista) 开始， *dwFlags* 必须设置为 `0` 或 **MB_ERR_INVALID_CHARS**。否则，函数将失败并 **ERROR_INVALID_FLAGS**。


+ [in] lpMultiByteStr：指向要转换的字符串的指针。

+ [in] cbMultiByte：lpMultiByteStr 参数指示的字符串的大小（以字节为单位）。 或者，如果字符串以 null 结尾，则可以将此参数设置为 -1。 请注意，如果 cbMultiByte 为 0，则函数将失败。

  如果此参数为 -1，则该函数将处理整个输入字符串，包括终止 null 字符。 因此，生成的 Unicode 字符串具有终止 null 字符，并且 函数返回的长度包括此字符。

  如果此参数设置为正整数，则函数将完全处理指定的字节数。 如果提供的大小不包含终止 null 字符，则生成的 Unicode 字符串不会以 null 结尾，并且返回的长度不包括此字符。

+ [out, optional] lpWideCharStr：指向接收转换字符串的缓冲区的指针。

+ [in] cchWideChar：lpWideCharStr 指示的缓冲区的大小（以字符为单位）。 如果此值为 0，则该函数返回所需的缓冲区大小（以字符为单位），包括任何终止 null 字符，并且不使用 lpWideCharStr 缓冲区。
+ 返回值：如果成功，则返回写入 *到 lpWideCharStr* 指示的缓冲区的字符数。 如果函数成功且 *cchWideChar* 为 `0`，则返回值是 *lpWideCharStr* 指示的缓冲区所需的大小（以字符为单位）。如果函数不成功， `0` 则返回 。





## 示例

在Windows上读取文件时，编码可能是GBK本地编码，使用`char*`直接读取将是乱码，故需要转为宽字符类型以正确解码。

`char*` 转 `wchar` / `wstring`：

```cpp
#include <windows.h>

std::wstring char_to_wchar(const char* str)  
{  
    int len = MultiByteToWideChar(CP_ACP, 0, str, (int) strlen(str), NULL, 0);  
    wchar_t* wc = new wchar_t[len + 1];  
    MultiByteToWideChar(CP_ACP, 0, str, (int) strlen(str), wc, len);  
    wc[len] = '\0';  
    std::wstring wstr = wc;  
    delete[] wc;  
    return wstr;  
}
```

C++ 17 之后（`std::string` 17 之前是 `const CharT* data()`, 之后为 `CharT* data()`）：
```cpp
std::wstring ansi_to_wstring(const std::string &s) {
    // ACP = ANSI Code Page，告诉他字符串里的是当前区域设置指定的编码（在中国区，ANSI 就是 GBK 了）
    int len = MultiByteToWideChar(CP_ACP, 0,
                                  s.c_str(), s.size(),
                                  nullptr, 0);
    std::wstring ws(len, 0);
    MultiByteToWideChar(CP_ACP, 0,
                        s.c_str(), s.size(), 
                        ws.data(), ws.size());
    return ws;
}

std::string wstring_to_ansi(const std::wstring &ws) {
    int len = WideCharToMultiByte(CP_ACP, 0,
                                  ws.c_str(), ws.size(),
                                  nullptr, 0,
                                  nullptr, nullptr);
    std::string s(len, 0);
    WideCharToMultiByte(CP_ACP, 0,
                        ws.c_str(), ws.size(),
                        s.data(), s.size(),
                        nullptr, nullptr);
    return s;
}

std::wstring utf8_to_wstring(const std::string &s) {
    int len = MultiByteToWideChar(CP_UTF8, 0,
                                  s.c_str(), s.size(),
                                  nullptr, 0);
    std::wstring ws(len, 0);
    MultiByteToWideChar(CP_UTF8, 0,
                        s.c_str(), s.size(), 
                        ws.data(), ws.size());
    return ws;
}

std::string wstring_to_utf8(const std::wstring &ws) {
    int len = WideCharToMultiByte(CP_UTF8, 0,
                                  ws.c_str(), ws.size(),
                                  nullptr, 0,
                                  nullptr, nullptr);
    std::string s(len, 0);
    WideCharToMultiByte(CP_UTF8, 0,
                        ws.c_str(), ws.size(),
                        s.data(), s.size(),
                        nullptr, nullptr);
    return s;
}
```




> [MultiByteToWideChar 函数 (stringapiset.h) - Win32 apps | Microsoft Learn](https://learn.microsoft.com/zh-cn/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar)
>
> [wcstombs_s、_wcstombs_s_l | Microsoft Learn](https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/reference/wcstombs-s-wcstombs-s-l?view=msvc-170)