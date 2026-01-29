
## C++ 20 新增
### `wait()` / `notify_one()`
C++20为 `std::atomic` 引入了类似条件变量的同步机制：
```cpp
// 等待操作
void wait(T old, std::memory_order order = std::memory_order::seq_cst) const noexcept;

// 通知操作
void notify_one() noexcept;
void notify_all() noexcept;
```
- **wait()**: 阻塞当前线程，直到原子值发生变化（不再等于old）
- **notify_one()**: 唤醒一个等待线程
- **notify_all ()**: 唤醒所有等待线程

示例：
```cpp
#include <atomic>  
#include <thread>  
#include <chrono>  
  
using namespace std::chrono_literals;

std::atomic<int> data{0};  
  
// 生产者  
void producer() {  
    std::println("{}  {}", __func__, std::chrono::system_clock::now());  
    std::this_thread::sleep_for(1s);  
  
    data.store(42, std::memory_order_release);  
    data.notify_one();  // 通知等待者  
    
    std::println("notify one {}", std::chrono::system_clock::now());  
}  
  
// 消费者  
void consumer() {  
    std::println("{} {}", __func__, std::chrono::system_clock::now());  
    
    int expected = 0;  
    data.wait(expected, std::memory_order_acquire);  // 等待值改变  
    // 此时data != 0  
    std::println("{} {}", expected, std::chrono::system_clock::now());  
}  
  
  
int main() {   
    std::jthread p(producer);  
    std::jthread c(consumer);  
}
```
输出：
```
consumer 2026-01-29 02:59:56.1390277
producer  2026-01-29 02:59:56.1390281
notify one 2026-01-29 02:59:57.1542759
0 2026-01-29 02:59:57.1542941
```

### atomic_ref
在C++20之前，原子操作必须通过`std::atomic<T>`类型进行。`std::atomic_ref`允许您对**非原子类型**的数据进行原子操作。
```cpp
int x = 0;
std::atomic_ref<int> ref(x);  // 对普通变量的原子引用
ref.fetch_add(1);
```

### atomic_flag 增强

```cpp
#include <atomic>

std::atomic_flag flag = ATOMIC_FLAG_INIT;

// 读取但不修改
bool set = flag.test(std::memory_order_acquire);

// 等待到值发生改变（true/false）
flag.wait(set, std::memory_order_acquire);

// 修改并通知
flag.clear(std::memory_order_release);
flag.notify_all();
```

### 浮点数原子操作

```cpp
std::atomic<float> f{1.0f};
f.fetch_add(0.5f);   // C++20支持
f.fetch_sub(0.2f);
```
### 智能指针原子化
引入了 `std::atomic<std::shared_ptr<T>>` 和 `std::atomic<std::weak_ptr<T>>` 的特化版本。

> [atomic 赋值操作是否为原子操作](atomic%20赋值操作是否为原子操作.md)