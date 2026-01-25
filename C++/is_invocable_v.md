`std::is_invocable_v` 是 C++17 中引入的一个模板变量，用于在编译期间判断一个函数对象是否可以使用指定的参数调用。
```cpp
template< class Fn, class... ArgTypes >  
struct is_invocable;
```

示例：
```cpp
#include <type_traits>

void func(int, double) {}

int main() {
   // 检查 func 函数签名是为为 (int, double) 
    static_assert(std::is_invocable_v<decltype(func), int, double>); // true
   
   // (double, bool) 与func函数签名不匹配
    static_assert(std::is_invocable_v<decltype(func), double, bool>); // false
}
```

其他几个相关的模板变量：
- `std::is_invocable_r_v`：检查函数对象是否可以使用指定的参数调用，并且返回类型是否可以转换为指定的类型。
- `std::is_nothrow_invocable_v`：检查函数对象是否可以使用指定的参数调用，并且调用不会抛出异常。
- `std::is_nothrow_invocable_r_v`：检查函数对象是否可以使用指定的参数调用，返回类型是否可以转换为指定的类型，并且调用不会抛出异常。

> [std::is_invocable, std::is_invocable_r, std::is_nothrow_invocable, std::is_nothrow_invocable_r - cppreference.com](https://en.cppreference.com/w/cpp/types/is_invocable.html)
> [is_invocable、is_invocable_r、is_nothrow_invocable、is_nothrow_invocable_r 类 | Microsoft Learn](https://learn.microsoft.com/zh-cn/cpp/standard-library/is-invocable-classes?view=msvc-170)
> 