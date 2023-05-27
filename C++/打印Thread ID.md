打印线程 ID 的两种办法
1. cout 输出
```cpp
cout << "1. thread id" << std::this_thread::get_id() << endl;
```
2. string 输出
```cpp
ostringstream oss;
oss << std::this_thread::get_id();
cout << "2. thread id: " << oss.str() << endl;
```
两种方式的差异在于，能不能将 ID 用其他如 `qDebug()`、`spdlog` 等打印出来。

创建vector线程
```cpp
std::vector<std::thread> v;
for (size_t i = 0; i < 5; i++)
{
	v.push_back(std::thread([]() {
		//打印线程id两种方式
		cout << "1. thread id" << std::this_thread::get_id() << endl;

		ostringstream oss;
		oss << std::this_thread::get_id();
		cout << "2. thread id: " << oss.str() << endl;
		}));
}
for (auto& it : v)
{
	it.join();
}
```