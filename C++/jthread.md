C++20 引入的 `std::jthread` 是对原 `std::thread` 的一次升级。

它的出现主要为了解决 `std::thread` 的两个痛点：
1.  **没有 RAII**：`std::thread` 在析构时如果仍然是 `joinable` 的（即没调用 `join()` 或 `detach()`），会直接导致程序崩溃（调用 `std::terminate`）。
2.  **缺乏线程停止机制**：在 C++20 之前，要让一个线程中途停止，通常需要自己手动实现。

## `std::jthread` 的核心特性

`std::jthread` 遵循 RAII 原则。当 `std::jthread` 对象离开作用域时，其析构函数会自动做两件事：

1.  调用 `request_stop()`（请求停止线程）。
2.  调用 `join()`（等待线程结束）。

`std::jthread` 支持向线程函数传递一个 `std::stop_token`，线程函数内部可以通过这个 token 检查是否收到了停止请求。

## 示例

```cpp
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

// 线程函数接收 stop_token 作为第一个参数
void worker(std::stop_token st, int id) {
    while (!st.stop_requested()) { // 检查是否有人要求停止
        std::cout << "Worker " << id << " is running...\n";
        std::this_thread::sleep_for(1s);
    }
    std::cout << "Worker " << id << " received stop signal, exiting.\n";
}

int main() {
    std::jthread t(worker, 1);

    std::this_thread::sleep_for(3s);
  
    // 显式请求停止（可选，因为析构函数也会自动调用）
    // t.request_stop(); 

    std::cout << "Main thread leaving scope...\n";
    // t 离开作用域：
    // 1. t.request_stop() 被调用
    // 2. t.join() 被调用，主线程在此等待 worker 结束
}
```

使用自己的 `stop_source`：

```cpp
void worker(std::stop_token st, int id) {
    while (!st.stop_requested()) {
        std::cout << "Worker " << id << " is running...\n";
        std::this_thread::sleep_for(200ms);
    }
    std::cout << "Worker " << id << " received stop signal, exiting.\n";
}

int main() {
    // 1. 创建一个独立的停止源
    std::stop_source my_source;

    // 2. 获取 token
    std::stop_token my_token = my_source.get_token();
    // 3. 启动线程，传入自己的 token
    // 注意：这里没有使用 jthread 自动生成的 token，而是传了自己的
    std::jthread t(worker, my_token, 1);

    std::this_thread::sleep_for(1s);
    std::cout << "Main: Stopping via external source...\n";
    // 4. 通过外部源停止线程
    my_source.request_stop();
}
```

> [!NOTE]
>
> 示例一使用的是 `jthread` 自身内部的 `stop_source`，所以调用 `jthread.request_stop()` 停止，而示例二是采用的外部传入的 `stop_source`，调用 `jthread.request_stop()` 是无法成功停止的，需要使用 `stop_source.request_stop()`

取消多个线程：

```cpp
void worker(std::stop_token st, int id) {
    while (!st.stop_requested()) {
        std::cout << "Worker " << id << " is running...\n";
        std::this_thread::sleep_for(200ms);
    }
    std::cout << "Worker " << id << " received stop signal, exiting.\n";
}

int main() {
    std::stop_source my_source;
    std::stop_token my_token = my_source.get_token();

    std::jthread t1(worker, my_token, 1);
    std::jthread t2(worker, my_token, 2);

    std::this_thread::sleep_for(1s);
    std::cout << "Main: Stopping via external source...\n";
    // 停止所有线程
    my_source.request_stop();
}
```

`stop_callback` 使用：

```cpp
int main() {  
    std::jthread t([](std::stop_token st) {  
        // 注册一个回调：一旦有人请求停止，会立即在发出停止信号的线程中执行回调  
        std::stop_callback cb(st, []{  
            std::cout << "Callback: Stop requested!\n";  
        });  
  
        // 模拟一个耗时操作  
        for (int i = 0; i < 10; ++i) {  
            if (st.stop_requested()) break;  
  
            std::cout << "Thread: Processing " << i << "...\n";  
            std::this_thread::sleep_for(200ms);  
        }  
    });  
    std::this_thread::sleep_for(600ms);  
    std::cout << "Main: Requesting stop!\n";  
    t.request_stop(); // 此时会触发子线程内部注册的回调  
  
    std::cout << "Main thread: " << std::this_thread::get_id() << "\n";  
    // 在构造函数注册之前已经请求了停止，则回调函数将在构造的当前线程中被立即调用。  
    std::stop_callback callback_after_stop(t.get_stop_token(), [] {  
        std::cout << "Stop callback executed by thread: "  
            << std::this_thread::get_id() << '\n';  
    });  
}
```

## 底层原理

`std::jthread` 的停止机制基于一套**共享状态（Shared State）**模型实现的。这套机制由三个核心组件构成：

1.  **`std::stop_source`**：停止信号的**发出者**（遥控器）。
2.  **`std::stop_token`**：停止信号的**接收者/观察者**（信号灯）。
3.  **`std::stop_callback`**：停止信号发出时的**回调注册者**。

4. `std::stop_source` 

*   **作用**：用于发起停止请求。
*   **原理**：它持有一个指向共享状态的引用。当调用 `request_stop()` 时，它会触发所有注册的回调函数。
*   **在 `jthread` 中**：`std::jthread` 内部包含一个 `std::stop_source` 成员变量。

2. `std::stop_token` 

*   **作用**：用于查询停止状态。
*   **原理**：它也持有指向同一个共享状态的引用，但它只有**读权限**。
*   **API**：
    *   `stop_requested()`: 检查共享状态中的值是否为 `true`。
    *   `stop_possible()`: 检查是否还有关联的 `stop_source` 存在（如果源头都销毁了，就不可能停止了）。
*   **在 `jthread` 中**：当 `jthread` 启动线程时，它会从内部的 `stop_source` 生成一个 `stop_token` (`source.get_token()`) 并传递给线程函数。

3. `std::stop_callback` 

*   **作用**：收到停止信号时执行的回调。
*   **原理**：这是一个 RAII 对象。它为关联的 `std::stop_token` 对象注册一个回调函数，当 `std::stop_token` 的关联 `std::stop_source` 请求停止时，将调用该回调函数。

### 架构图解

可以将它们的关系想象成 `std::shared_ptr`。它们都指向堆上的同一个**控制块（Control Block）**。

```text
[ std::jthread / std::stop_source ]      [ std::stop_token ]
           |                                     |
           | (拥有)                              | (观察)
           v                                     v
    +-------------------------------------------------------+
    |                 Shared Stop State (堆内存)             |
    |-------------------------------------------------------|
    | 1. Atomic Bool: stop_requested (是否已请求停止)          |
    | 2. Thread ID:   requester_id   (谁请求的)               |
    | 3. Callback List: 注册的 stop_callback 链表             |
    +-------------------------------------------------------+
```

> [!NOTE] 
> 仅供理解，实际有差异。

###  `std::jthread` 内部实现逻辑

 `std::jthread` 的简化版源码：

```cpp
class jthread {
private:
    thread _thread;        // 底层的 thread
    stop_source _ssource;  // 控制停止的stop_source

public:
     // 构造函数
     template <class _Fn, class... _Args>
        requires (!is_same_v<remove_cvref_t<_Fn>, jthread>)
     explicit jthread(_Fn&& func, _Args&&... args) {
        // 1. 检查 func 是否接受 stop_token 作为第一个参数调用
        if constexpr (is_invocable_v<decay_t<_Fn>, stop_token, decay_t<_Args>...>) {
            // 2. 接受，从 source 获取 token 并传递进去
            _thread._Start(_STD forward<_Fn>(func), _ssource.get_token(), _STD forward<_Args>(args)...);
        } else {
            // 3. 不接受，以普通 thread 启动
            _thread._Start(_STD forward<_Fn>(func), _STD forward<_Args>(args)...);
        }
    }

    // 获取内部的 stop_source
    std::stop_source get_stop_source() noexcept {
        return _ssource;
    }

    // 获取对应的 token
    std::stop_token get_stop_token() noexcept {
        return _ssource.get_token();
    }

    // 请求停止
    bool request_stop() noexcept {
        return _ssource.request_stop();
    }

    ~jthread() {
        if (_thread.joinable()) {
            request_stop(); // A. 先请求停止
            _thread.join(); // B. 再等待结束
        }
    }
  
    // ... 移动构造、赋值等 ...
};
```

需要特别说明的是：

```cpp
if constexpr (is_invocable_v<decay_t<_Fn>, stop_token, decay_t<_Args>...>)
```

是在**判断传进来的函数 `_Fn`，是否可以使用指定的参数调用。**

即：

```cpp
void worker(std::stop_token st, int x) { ... }
// 判断worker()签名是否为(std::stop_token, int)
bool r = std::is_invocable_v<decltype(worker), std::stop_token, int>; // true

// 没有 stop_token的签名
void worker(int x) { ... }
bool r = std::is_invocable_v<decltype(worker), std::stop_token, int>; // false
```

注意 `decay_t<_Args>...>`，如果自己传入了 `stop_token`，那么展开后为：

```cpp
std::is_invocable_v<decltype(worker), std::stop_token, std::stop_token, int>;
```

与 `worker(std::stop_token, int)` 签名不匹配，所以进入 else 分支：

```cpp
_Impl._Start(_STD forward<_Fn>(func), _STD forward<_Args>(args)...);
```