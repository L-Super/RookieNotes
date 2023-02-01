rpath 全称是 `run-time search path`。Linux 下所有 elf 格式的文件都包含它，特别是可执行文件。它规定了可执行文件在寻找 `.so` 文件时的第一优先位置。  

另外，elf 文件中还规定了 runpath。它与 rpath 相同，只是优先级低一些。
# 搜索 `.so` 的优先级顺序

-   RPATH： 写在elf文件中
-   LD_LIBRARY_PATH： 环境变量
-   RUNPATH： 写在elf文件中
-   `ldconfig`的缓存： 配置`/etc/ld.conf*`可改变
-   默认的 `/lib`, `/usr/lib`
# 查看 RPATH

对于任意的elf文件，可以使用`$ readelf -d xxx'`来查看。  
![](../images/Pasted%20image%2020230201093741.png)

RPATH中有个特殊的标识符`$ORIGIN`。这个标识符代表elf文件自身所在的目录。当希望使用相对位置寻找`.so`文件，就需要利用`$ORIGIN`设置RPATH。多个路径之间使用冒号`:`隔开。
