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