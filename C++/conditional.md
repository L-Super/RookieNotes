`std::conditional` 根据指定的条件，从两种类型中选择一种。

```cpp
template< bool B, class T, class F >
struct conditional;

template< bool B, class T, class F >
using conditional_t = typename conditional<B,T,F>::type; // C++ 14
```

如果在编译时 `B` 为 true，则定义为 `T` ，否则定义为 `F` 

实现原理：

```cpp
template <bool B, class T, class F>
struct conditional {
using type = T; // If B is true
};

template <class T, class F>
struct conditional<false, T, F> {
using type = F; // If B is false
};
```

## 应用场景 

有时我们需要根据一个在编译时已知的条件，在两种类型之间做出选择。

```cpp
template<typename T>
struct B {
    using Type = std::conditional_t<std::is_integral_v<T>, int, double>;
    Type value;
};

int main() {
    static_assert(std::is_same_v<B<int>::Type, int>);    // 通过
    static_assert(std::is_same_v<B<float>::Type, double>); // 通过
}
```