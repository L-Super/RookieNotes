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

注：尚有缺陷，无法打印命名空间内的 enum class，如 `pp::enum_class`

更通用的方法：
```cpp
#pragma once  
  
#include <string>  
  
namespace sn  
{  
namespace details  
{  
template <class T, T N> const char *enum_name_static()  
{  
#if defined(_MSC_VER)  
    return __FUNCSIG__;  
#else  
    return __PRETTY_FUNCTION__;  
#endif  
}  
  
template <bool Cond> struct my_enable_if  
{  
};  
  
template <> struct my_enable_if<true>  
{  
    typedef void type;  
};  
  
template <int Beg, int End, class F> typename my_enable_if<Beg == End>::type static_for(F const &func)  
{  
}  
  
template <int Beg, int End, class F> typename my_enable_if<Beg != End>::type static_for(F const &func)  
{  
    func.template call<Beg>();  
    static_for<Beg + 1, End>(func);  
}  
  
template <class T> struct enum_name_functor  
{  
    int n;  
    std::string &s;  
  
    enum_name_functor(int n, std::string &s) : n(n), s(s)  
    {  
    }  
    template <int I> void call() const  
    {  
        if (n == I)  
            s = details::enum_name_static<T, (T)I>();  
    }  
};  
  
} // namespace details  
  
template <class T, T Beg, T End> std::string enum_name(T n)  
{  
    std::string s;  
    details::static_for<(int)Beg, (int)End + 1>(details::enum_name_functor<T>((int)n, s));  
    if (s.empty())  
        return "";  
#if defined(_MSC_VER)  
    size_t pos = s.find(',');  
    pos += 1;    size_t pos2 = s.find('>', pos);#else  
    size_t pos = s.find("N = ");  
    pos += 4;  
    size_t pos2 = s.find_first_of(";]", pos);  
#endif  
    s = s.substr(pos, pos2 - pos);  
    size_t pos3 = s.find("::");  
    if (pos3 != s.npos)  
        s = s.substr(pos3 + 2);  
    return s;  
}  
  
template <class T> std::string enum_name(T n)  
{  
    return enum_name<T, (T)0, (T)256>(n);  
}  
  
template <class T, T Beg, T End> T enum_from_name(std::string const &s)  
{  
    for (int i = (int)Beg; i < (int)End; i++)  
    {  
        if (s == enum_name((T)i))  
        {  
            return (T)i;  
        }  
    }  
    throw;  
}  
  
template <class T> T enum_from_name(std::string const &s)  
{  
    return enum_from_name<T, (T)0, (T)256>(s);  
}  
} // namespace sn
```

C++ 17 中，可以使用开源库 [magic_enum](https://github.com/Neargye/magic_enum) 通过反射获得枚举变量的名字。