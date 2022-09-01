size_t是标准C库中定义的，在64位系统中为long unsigned int，非64位系统中为unsigned int。size_t的引入增强了程序在不同平台上的可移植性。

```c++
typedef unsigned __int64 size_t;
```

一个基本的无符号整数的C / C + +类型，它是sizeof操作符返回的结果类型，该类型的大小可选择。因此，它可以存储在理论上是可能的任何类型的数组的最大大小。换句话说，一个指针可以被安全地放进为size_t类型.

通常用sizeof(XX)操作，这个操作所得到的结果就是size_t类型。