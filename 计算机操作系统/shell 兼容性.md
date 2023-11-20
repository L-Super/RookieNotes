## Shell 脚本

[Shell 脚本](https://zh.wikipedia.org/wiki/Shell_script) 是指包含有下面格式的可执行的文本文件。

```sh
#!/bin/sh
 ... command lines
```

第一行指明了读取并执行这个文件的 shell 解释器。

## POSIX shell 兼容性

**shell 程序列表**

| 软件包 | POSIX shell | 说明                                                         |
| ------ | ----------- | ------------------------------------------------------------ |
| bash   | 是          | Bash：GNU Bourne Again SHell (事实上的标准)                  |
| dash   | 是          | Debian [Almquist Shell](https://zh.wikipedia.org/wiki/Almquist_shell), 擅长 shell 脚本 |
| zsh    | 是          | [Z shell](https://zh.wikipedia.org/wiki/Z_shell):有许多增强的标准 shell |
| tcsh   | 否          | [TENEX C Shell](https://zh.wikipedia.org/wiki/Tcsh): 一个 [Berkeley csh](https://zh.wikipedia.org/wiki/C_shell) 的增强版本 |
| ...    |             |                                                              |


系统中的许多脚本都可以通过任意 [POSIX](https://zh.wikipedia.org/wiki/POSIX) shell 来执行。

- 默认的非交互 POSIX shell "`/bin/sh`" 是一个指向到 `/usr/bin/dash` 的符号链接，并被许多系统程序使用。
- 默认的交互式 POSIX shell 是 `/usr/bin/bash`。

避免编写具有 **bashisms**（bash 化）或者 **zshisms**（zsh 化）语法的 shell 脚本，确保脚本在所有 POSIX shell 之间具有可移植性。你可以使用 `checkbashisms` 对其进行检查。

**典型 bashism 语法列表**

| 好的：POSIX                       | 应该避免的：bashism                |
| --------------------------------- | ---------------------------------- |
| `if [ "$foo" = "$bar" ] ; then …` | `if [ "$foo" == "$bar" ] ; then …` |
| `diff -u file.c.orig file.c`      | `diff -u file.c{.orig,}`           |
| `mkdir /foobar /foobaz`           | `mkdir /foo{bar,baz}`              |
| `funcname() { … }`                | `function funcname() { … }`        |
| 八进制格式："`\377`"              | 十六进制格式："`\xff`"             |

使用 "`echo`" 命令的时候需要注意以下几个方面，因为根据内置 shell 和外部命令的不同，它的实现也有差别。

+ 避免使用除“`-n`”以外的任何命令行选项。
+ 避免在字符串中使用转义序列，因为根据 shell 不同，计算后的结果也不一样。





> https://www.debian.org/doc/manuals/debian-reference/ch01.zh-cn.html#_the_login_shell
>
> https://www.debian.org/doc/manuals/debian-reference/ch12.zh-cn.html#_posix_shell_compatibility