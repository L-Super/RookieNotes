查看可执行文件运行所需的共享库

```
ldd [options] executable
```

选项：
+ `-v` – 详细模式，打印所有信息。
+ `-u` – 显示未使用的直接依赖项
+ `-d` – 执行数据重定位并显示丢失的 ELF 对象。
+ `-r` – 执行数据和函数重定位并显示丢失的 ELF 对象和函数。

例如：
```bash
# ldd nginx
        linux-vdso.so.1 (0x00007fff7e704000)
        libcrypt.so.1 => /lib/x86_64-linux-gnu/libcrypt.so.1 (0x00007fa88e464000)
        libpcre2-8.so.0 => /lib/x86_64-linux-gnu/libpcre2-8.so.0 (0x00007fa88e3ca000)
        libssl.so.3 => /lib/x86_64-linux-gnu/libssl.so.3 (0x00007fa88e320000)
        libcrypto.so.3 => /lib/x86_64-linux-gnu/libcrypto.so.3 (0x00007fa88de9e000)
        libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007fa88de7f000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa88dc9c000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fa88e6de000)
```
