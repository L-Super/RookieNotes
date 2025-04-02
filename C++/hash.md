`std::hash` 返回哈希值

用法：
```cpp
std::string str = "Meet the new boss...";
std::size_t str_hash = std::hash<std::string>{}(str);
```

自定义散列函数用法：
```cpp
struct S
{
    std::string first_name;
    std::string last_name;
    bool operator==(const S&) const = default; // C++20 起
};
 
// C++20 前
// bool operator==(const S& lhs, const S& rhs)
// {
//     return lhs.first_name == rhs.first_name && lhs.last_name == rhs.last_name;
// }
 
// 自定义散列函数可以是独立函数对象：
struct MyHash
{
    std::size_t operator()(S const& s) const 
    {
        std::size_t h1 = std::hash<std::string>{}(s.first_name);
        std::size_t h2 = std::hash<std::string>{}(s.last_name);
        return h1 ^ (h2 << 1); 
    }
};
 
// std::hash 的自定义特化能注入命名空间 std 中
template<>
struct std::hash<S>
{
    std::size_t operator()(const S& s) const noexcept
    {
        std::size_t h1 = std::hash<std::string>{}(s.first_name);
        std::size_t h2 = std::hash<std::string>{}(s.last_name);
        return h1 ^ (h2 << 1); // 或者使用 boost::hash_combine
    }
};
 
int main()
{
	S obj = {"Hubert", "Farnsworth"};
	// 使用独立的函数对象
	std::cout << "hash(" << obj.first_name << ", "
	          << obj.last_name << ") = "
	          << MyHash{}(obj) << "（使用 MyHash）\n" << std::setw(31) << "或 "
	          << std::hash<S>{}(obj) << "（使用注入的特化）\n";
	
	// 自定义散列函数令在无序容器中使用自定义类型可行。
	// 此示例将使用注入的 std::hash 特化，
	// 如果要使用 MyHash 替代，那么将它作为第二模板参数传递。
	std::unordered_set<S> names = {obj, {"Bender", "Rodriguez"}, {"Turanga", "Leela"}};
	for (auto const& s: names)
	    std::cout << s.first_name << ' '
	              << s.last_name << '\n';
}
```