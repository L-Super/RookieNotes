`std::optional` 管理一个_可选_ ﻿的所含值，即既可以存在也可以不存在的值。

```cpp
std::optional<T>::optional
```

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
        std::cout << "create2(true) 返回 " << *str << '\n';
}
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