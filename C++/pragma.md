在所有的预处理指令中，`#pragma` 指令可能是最复杂的了，它的作用是设定编译器的状态或者是指示编译器完成一些特定的动作。 `#pragma` 指令对每个编译器给出了一个方法，在保持与C和C++语言完全兼容的情况下，给出主机或操作系统专有的特征。依据定义，编译指示是机器或操作系统专有的，且对于每个编译器都是不同的。

**一般格式**
其格式一般为: \#pragma Para。其中Para 为参数，下面来看一些常用的参数

## `#pragma once`

只要在头文件的最开始加入这条指令就能够保证头文件被编译一次，这条指令实际上在VC6中就已经有了，但是考虑到兼容性并没有太多的使用它。

\#pragma once是编译相关，就是说这个编译系统上能用，但在其他编译系统不一定可以，也就是说**移植性差**，不过基本上已经是每个编译器都有这个定义了。

\#ifndef，\#define，\#endif这个是C++语言相关，这是C++语言中的宏定义，通过宏定义避免文件多次编译。所以在所有支持C++语言的编译器上都是有效的，如果写的程序要**跨平台，最好使用这种方式**。

## `#pragma comment`
该指令将一个注释记录放入一个对象文件或可执行文件中。

` #pragma comment(comment-type,["commentstring"])`

comment-type是一个预定义的标识符，指定注释的类型，应该是compiler，exestr，lib，linker之一。

comment string是一个提供为comment-type提供附加信息的字符串。

注释类型：
1、compiler：
放置编译器的版本或者名字到一个对象文件，该选项是被linker忽略的。
2、exestr：
在以后的版本将被取消。
3、lib：
放置一个库搜索记录到对象文件中，这个类型应该是和comment string（指定你要Linker搜索的lib的名称和路径）这个库的名字放在Object文件的默认库搜索记录的后面，linker搜索这个这个库就像你在命令行输入这个命令一样。你可以在一个源文件中设置多个库记录，它们在object文件中的顺序和在源文件中的顺序一样。如果默认库和附加库的次序是需要区别的，使用Z编译开关是防止默认库放到object模块。
4、linker：
指定一个连接选项，这样就不用在命令行输入或者在开发环境中设置了。

常用的lib关键字，可以帮我们连入一个库文件。

下面的编译指示导致连接程序在连接时搜索EMAPI.LIB库。连接程序首先在当前工作目录然后在LIB环境变量指定的路径中搜索。

```
#pragma comment( lib, "emapi" )
```

> https://learn.microsoft.com/zh-cn/cpp/preprocessor/pragma-directives-and-the-pragma-keyword?view=msvc-170
> https://gcc.gnu.org/onlinedocs/gcc/Pragmas.html
> https://zh.cppreference.com/w/cpp/preprocessor/impl

