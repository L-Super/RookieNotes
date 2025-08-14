POD是C++中**Plain Old Data的缩写**，指的是**可以通过简单内存复制进行复制和传输的数据类型**。这表示该类型与用于 C 程序语言的类型兼容，即它能直接以二进制形式与 C 库交互。

POD 类型的特点包括：

1. **标量类型**
2. **没有虚函数**：POD类型的类或结构体不包含虚函数，即它们不使用C++的多态特性。
3. **没有虚继承**：POD类型的类或结构体不涉及虚继承，这意味着它们的继承关系是直接和明确的。
4. **与C兼容**：POD类型的对象可以直接与C语言代码交互，因为它们遵循C语言的结构体布局规则。
5. **平凡性**：POD类型需要满足平凡性，即它们的默认构造函数、拷贝构造函数、析构函数以及赋值运算符都是平凡的（没有额外的行为）。
6. **标准布局**：POD 类型还需要满足标准布局的要求，这意味着类型的非静态成员按照它们在类声明中的顺序排列，并且没有类成员之间的空隙。

检查类型是否为 POD 类型：
```cpp
#include <type_traits>
 
struct A { int m; };
static_assert(std::is_pod_v<A> == true);
 
class B: public A { int m; };
static_assert(std::is_pod_v<B> == false);
 
struct C { virtual void foo(); };
static_assert(std::is_pod_v<C> == false);
```
注：此类型要求已在 C++ 标准中被弃用。它的所有使用都已被替换成更为斟酌过的类型要求，例如[平凡类型](https://zh.cppreference.com/w/cpp/named_req/TrivialType "cpp/named req/TrivialType")。(C++20 起)

> [C++ 具名要求：简旧数据类型 (PODType) - cppreference.com](https://zh.cppreference.com/w/cpp/named_req/PODType)
> [平凡类类型](https://zh.cppreference.com/w/cpp/language/classes#.E5.B9.B3.E5.87.A1.E7.B1.BB "cpp/language/classes")
> [POD 类 - cppreference.com](https://zh.cppreference.com/w/cpp/language/classes#POD_.E7.B1.BB)
> [is_pod]( https://zh.cppreference.com/w/cpp/types/is_pod "cpp/types/is pod")
> [普通、标准布局、POD 和文本类型](https://learn.microsoft.com/zh-cn/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170)