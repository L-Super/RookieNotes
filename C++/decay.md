`std::decay` 是 C++11 引入的类型特征（type trait）

将类型转换为"按值传递"时的等效类型，模拟函数参数的类型衰减（decay）规则。

```cpp
template <class T>
struct decay;

template <class T>
using decay_t = typename decay<T>::type;
```

可能的实现：
```cpp

template<class T>
struct decay
{
private:
    typedef typename std::remove_reference<T>::type U;
public:
    typedef typename std::conditional< 
        std::is_array<U>::value,
        typename std::add_pointer<typename std::remove_extent<U>::type>::type,
        typename std::conditional< 
            std::is_function<U>::value,
            typename std::add_pointer<U>::type,
            typename std::remove_cv<U>::type
        >::type
    >::type type;
};
```

类模板成员 typedef `type` 为在以下几个阶段中定义的“修改后类型“：

1. `U` 定义为 `remove_reference<T>::type`
2. 如果 `is_array<U>::value` 为 true，则修改后的类型 `type` 是 `remove_extent<U>::type *`。
3. 否则，如果 `is_function<U>::value` 为 true，则修改后的类型 `type` 是 `add_pointer<U>::type`。
- 否则，修改后的类型 `type` 是 `remove_cv<U>::type`。

```cpp
#include <type_traits>

// 1. 数组类型 -> 指针类型
int arr[10];
// std::decay_t<int[10]> => int*

// 2. 函数类型 -> 函数指针类型
void func(int);
// std::decay_t<void(int)> => void(*)(int)

// 3. 移除 cv 限定符（const/volatile）和引用
const int& x = 10;
// std::decay_t<const int&> => int

// 4. 其他类型保持不变
int y;
// std::decay_t<int> => int
```

## 使用示例

```cpp
#include <type_traits>

template<typename T, typename U>
constexpr bool is_decay_equal = std::is_same_v<std::decay_t<T>, U>;

int main() {
	bool r = is_decay_equal <int, int>; // true
	r = is_decay_equal <int&, int>; // true
	r = is_decay_equal <int&&, int>; // true
	r = is_decay_equal <const int&, int>; // true
	r = is_decay_equal <int[2], int*>; // true
	r = is_decay_equal <int[2], int>; // false
	r = is_decay_equal <void(int), void(*)(int)> // true
}
```


## 应用场景

1. **泛型编程中统一类型**
2. **存储转发参数的副本**
3. **类型比较时忽略引用和cv限定**

`std::decay` 是处理模板参数类型的重要工具，特别适合需要"值语义"的场景。

> [conditional](conditional.md)