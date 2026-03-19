`std::declval` 作用是生成一个类型 T 的右值引用表达式，而无需实际创建该类型的对象。

```cpp
template< class T >
typename std::add_rvalue_reference<T>::type declval() noexcept; // C++11 起,直到 C++14,仅限未求值上下文
template< class T >
std::add_rvalue_reference_t<T> declval() noexcept; // C++14 起,仅限未求值上下文
```

`std::declval`此辅助模板用于编写出现在未求值上下文（通常是 `decltype` 的操作数）中的表达式。在未求值上下文，此辅助模板将任何类型T（可以是不完整类型）转换为该类型的一个表达式，从而可以在不需要通过构造函数的情况下使用T的成员函数。

`std::declval` 只能在未求值上下文中使用，并且不需要被定义；如果表达式中包含此函数被求值，则会出错。正式地说，如果此函数被ODR 使用，则程序格式错误。

`std::declval` 通常与 `decltype` 配合使用，用于获取表达式的类型，而无需实际调用构造函数或创建对象。

## 场景

处理无默认构造函数的类型:

```cpp
#include <iostream>
#include <utility>
 
struct Default
{
    int foo() const { return 1; }
};
 
struct NonDefault
{
    NonDefault() = delete;
    int foo() const { return 1; }
};
 
int main()
{
    using Type = decltype(Default().foo()); // type is int
    using Type = decltype(std::declval<Default>().foo()) n2 = 1; // same
 
	// decltype(NonDefault().foo()); // error: 无法实例化NoDefault对象来推导类型
    decltype(std::declval<NonDefault>().foo()); // type is int
}
```

decltype推导返回类型，如：

```cpp
// 推导两个类型相加的结果类型
template<typename T, typename U>
auto add(T t, U u) -> decltype(std::declval<T>() + std::declval<U>()) {
    return t + u;
}
```

SFINAE和类型萃取，如：

```cpp
// 检测类型是否有某个成员函数

template<typename T>
class has_size {
    // 第一个test：如果T有size()成员函数，这个版本匹配成功
    // 参数为int为了匹配类型优先级更高,test(...)能匹配任何类型
    template<typename U>
    static auto test(int) -> decltype(
        std::declval<U>().size(),  // 尝试调用size()
        std::true_type{}           // 如果成功，返回true_type
    );
    
    // 第二个test：fallback版本，总是匹配
    template<typename>
    static std::false_type test(...);
    
public:
    // 调用test<T>(0)，根据返回类型判断
    static constexpr bool value = decltype(test<T>(0))::value;
};

// 测试类型
struct WithSize {
    int size() const { return 42; }
};

struct WithoutSize {
    // 没有size()
};

int main() {
    std::cout << has_size<WithSize>::value;      // true
    std::cout << has_size<WithoutSize>::value;   // false
    std::cout << has_size<std::string>::value;   // true
}
```

