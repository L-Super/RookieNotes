在 macOS 平台，现在有 X86 和 ARM 两个架构，而 universal 架构就是同时包含 Intel（x86_64）和 Apple Silicon（arm64）架构的二进制文件，使得应用程序可以在不同架构上运行而无需重新编译。

在 CMake 中，使用 `CMAKE_OSX_ARCHITECTURES` 选项即可指定对应架构，或者同时指定：
```cmake
-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64
```
这样，编译后，会生成一个 universal 库（自动合并 X86、ARM 架构），使用 `file` 命令查看：
```bash
$ file libSimZip.a 
libSimZip.a: Mach-O universal binary with 2 architectures: [x86_64:current ar archive random library] [arm64:current ar archive random library]
libSimZip.a (for architecture x86_64):  current ar archive random library
libSimZip.a (for architecture arm64):   current ar archive random library
```

[lipo](../计算机操作系统/macOS/lipo.md) 命令查看：
```bash
lipo -info libSimZip.a
Architectures in the fat file: libSimZip.a are: x86_64 arm64
```
还可使用 [otool](../计算机操作系统/macOS/otool.md) 命令查看。

>[!warning] 
>未在 macOS X86 架构上测试，是否可以同时指定两个架构