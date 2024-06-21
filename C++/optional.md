`std::optional` 管理一个_可选_ ﻿的所含值，即既可以存在也可以不存在的值。

```cpp
std::optional<T>::optional
```

| 观察器                          |                             |
| ---------------------------- | --------------------------- |
| operator-><br />operator*    | 访问所含值, 若 `*this` 不含值则行为未定义。 |
| operator bool<br />has_value | 检查对象是否含值                    |
| value                        | 返回所含值                       |
| value_or                     | 在所含值可用时返回它，否则返回另一个值         |

示例：

```cpp
#include <iostream>
#include <optional>
#include <string>
 
// optional 可用作可能失败的工厂的返回类型
std::optional<std::string> create(bool b)
{
    if (b)
        return "Godzilla";
    return {};
}
 
// 能用 std::nullopt 创建任何（空的）std::optional
auto create2(bool b)
{
    return b ? std::optional<std::string>{"Godzilla"} : std::nullopt;
}
 
int main()
{
    std::cout << "create(false) 返回 "
              << create(false).value_or("empty") << '\n';
 
    // 返回 optional 的工厂函数可用作 while 和 if 的条件
    if (auto str = create2(true))
        std::cout << "create2(true) 返回 " << str.value() << '\n';
}
```

`std::optional<T>::operator->, std::optional<T>::operator*` 示例：
```cpp
std::optional<int> opt1 = 1;
std::cout<< "opt1: "  << *opt1 << '\n';

// 修改int值
*opt1 = 2;
std::cout<< "opt1: "  << *opt1 << '\n';

std::optional<std::string> opt2 = "abc";
// 可调用string size()
std::cout << "opt2: " << *opt2 << ", size: " << opt2->size() << '\n';
```
注意：解引用运算符 [operator*()]( https://zh.cppreference.com/w/cpp/utility/optional/operator* "cpp/utility/optional/operator*") 不检查此 `optional` 是否含值。

`std::optional<T>::operator bool, std::optional<T>::has_value` 示例：
```cpp
int main()
{
    // 输出bool值
    std::cout << std::boolalpha;
 
    std::optional<int> opt;
    std::cout << opt.has_value() << '\n';
 
    opt = 43;
    if (opt)
        std::cout << "设置值为 " << opt.value() << '\n';
    else
        std::cout << "未设置值\n";
 
    opt.reset();
    if (opt.has_value())
        std::cout << "值仍被设为 " << opt.value() << '\n';
    else
        std::cout << "不再设置值\n";
}
```
输出：
```
false
设置值为 43
不再设置值
```
## make_optional

```cpp
auto op1 = std::make_optional<std::vector<char>>({'a','b','c'});
std::cout << "op1: ";
for (char c : op1.value())
	std::cout << c << ',';
auto op2 = std::make_optional<std::vector<int>>(5, 2);
std::cout << "\nop2: ";
for (int i : *op2)
	std::cout << i << ',';
std::string str{"hello world"};
auto op3 = std::make_optional<std::string>(std::move(str));
std::cout << "\nop3: " << quoted(op3.value_or("empty value")) << '\n';
std::cout << "str: " << std::quoted(str) << '\n';
```

> https://zh.cppreference.com/w/cpp/utility/optional