rpath 全称是 `run-time search path`。Linux 下所有 elf 格式的文件都包含它，特别是可执行文件。它规定了可执行文件在寻找 `.so` 文件时的第一优先位置。  

另外，elf 文件中还规定了 runpath。它与 rpath 相同，只是优先级低一些。
# 搜索 `.so` 的优先级顺序

-   RPATH： 写在elf文件中
-   LD_LIBRARY_PATH： 环境变量
-   RUNPATH： 写在elf文件中
-   `ldconfig`的缓存： 配置`/etc/ld.conf*`可改变
-   默认的 `/lib`, `/usr/lib`
可以看到，RPATH 与 RUNPATH 中间隔着 LD_LIBRARY_PATH。为了让用户可以通过修改 LD_LIBRARY_PATH 来指定 `.so` 文件，大多数编译器都将输出的 RPATH 留空，并用 RUNPATH 代替 RPATH。

# 查看 RPATH

对于任意的elf文件，可以使用`$ readelf -d xxx'`来查看。  
![](../images/Pasted%20image%2020230201093741.png)

# 设置 RPATH
RPATH 中有个特殊的标识符 `$ORIGIN`。这个标识符代表 elf 文件自身所在的目录。当希望使用相对位置寻找 `.so` 文件，就需要利用 `$ORIGIN` 设置 RPATH。多个路径之间使用冒号 `:` 隔开。

在 gcc 中，设置 RPATH 的办法很简单，就是设置 linker 的 rpath 选项：
```
gcc -Wl,-rpath,/your/rpath/ test.cpp
```

如果需要设置 `$ORIGIN` ：
```
gcc -Wl,-rpath,'$ORIGIN/lib' test.cpp
```
  
注意，虽然选项里写着RPATH，但它设置的还是RUNPATH。

在 CMake 中，由于 CMake 需要包揽软件安装的事宜，因此 CMake 使用两个变量来控制 RPATH：`INSTALL_RPATH` 和 `BUILD_RPATH`。参见[设置链接库加载的路径](../CMake/设置链接库加载的路径.md)

# 拷贝依赖库到 lib 目录
拷贝 Pmair 应用程序所有的依赖到当前目录下的 lib 子目录下。  
```shell
ldd Pamir | awk '{print $3}' | xargs -i cp -L {} ./lib
```


