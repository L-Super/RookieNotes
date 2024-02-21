`std::cout`标准流不支持输出`enum class`类型，可以通过重载`operator<<`操作符，再使用SFINAE泛型支持任何枚举类，而不限定于某特定的枚举类：

```cpp
template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}
```

例子：

```cpp
#include <iostream>
#include <type_traits>

// Scoped enum
enum class Color
{
    Red,
    Green,
    Blue
};

// Unscoped enum
enum Orientation
{
    Horizontal,
    Vertical
};

template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

int main()
{
    std::cout << Color::Blue << "\n";
    std::cout << Vertical << "\n";
    return 0;
}
```

