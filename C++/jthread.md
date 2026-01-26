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
> 示例一使用的是 `jthread` 自身内部的 `stop_source`，所以调用 `jthread.request_stop()` 停止，而示例二是采用的外部传入的 `stop_source`，调用 `jthread.request_stop()` 是无法成功停止的，因为不是同一个 `stop_token` ，需要使用 `stop_source.request_stop()`

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

### stop_source
`stop_source` 管理一个共享的停止状态（`_Stop_state`），通过引用计数实现资源共享和生命周期管理。

```cpp
class stop_source {  
public:  
    stop_source() : _State{new _Stop_state} {}  
    explicit stop_source(nostopstate_t) noexcept : _State{} {}  
    stop_source(const stop_source& _Other) noexcept : _State{_Other._State} {  
        const auto _Local = _State;  
        if (_Local != nullptr) { 
        	// `fetch_add(2)` 而非1
        	// 原因：`_Stop_sources`的最低位用于标记，实际计数是 `值/2` 
            _Local->_Stop_sources.fetch_add(2, memory_order_relaxed);  
        }  
    }  
  
    stop_source(stop_source&& _Other) noexcept : _State{ exchange(_Other._State, nullptr)} {}  
    stop_source& operator=(const stop_source& _Other) noexcept {  
        stop_source{_Other}.swap(*this);  
        return *this;  
    }  
  
    stop_source& operator=(stop_source&& _Other) noexcept {  
        stop_source{move(_Other)}.swap(*this);  
        return *this;  
    }  
  
    ~stop_source() {  
        const auto _Local = _State;  
        if (_Local != nullptr) {  
        	// 1. source计数-1（实际-2）
        	// 右移1位忽略标志位，2>>1 == 1说明是最后一个
            if ((_Local->_Stop_sources.fetch_sub(2, memory_order_acq_rel) >> 1) == 1) {  
                if (_Local->_Stop_tokens.fetch_sub(1, memory_order_acq_rel) == 1) {  
                	// 2. 最后一个source，尝试释放token计数
                    delete _Local;  
                }  
            }  
        }  
    }  
  
    stop_token get_token() const noexcept {  
        const auto _Local = _State;  
        if (_Local != nullptr) {  
            _Local->_Stop_tokens.fetch_add(1, memory_order_relaxed);  
        }  
  
        return stop_token{_Local};  
    }  
    bool stop_requested() const noexcept {  
        const auto _Local = _State;  
        return _Local != nullptr && _Local->_Stop_requested();  
    }  
  
    bool stop_possible() const noexcept {  
        return _State != nullptr;  
    }  
  
    bool request_stop() noexcept {  
        const auto _Local = _State;  
        return _Local && _Local->_Request_stop();  
    }  
  
private:  
    _Stop_state* _State;  // 指向共享停止状态的指针
};
```

### stop_state

```cpp
struct _Stop_state {  
    atomic<uint32_t> _Stop_tokens  = 1; // plus one shared by all stop_sources  
    atomic<uint32_t> _Stop_sources = 2; // plus the low order bit is the stop requested bit  
    _Locked_pointer<_Stop_callback_base> _Callbacks;  // 存储所有注册的停止回调
    // always uses relaxed operations; ordering provided by the _Callbacks lock  
    // (atomic just to get wait/notify support)    
    atomic<const _Stop_callback_base*> _Current_callback = nullptr; // 当前执行的回调
    _Thrd_id_t _Stopping_thread = 0; // 停止线程ID
      
    bool _Stop_requested() const noexcept {  
        return (_Stop_sources.load() & uint32_t{1}) != 0;  
    }  
  
    bool _Stop_possible() const noexcept {  
        return _Stop_sources.load() != 0;  
    }  
  
    bool _Request_stop() noexcept {  
        // Attempts to request stop and call callbacks, returns whether request was successful  
        // 原子地设置停止位，检查是否首次请求
        if ((_Stop_sources.fetch_or(uint32_t{1}) & uint32_t{1}) != 0) {  
            // another thread already requested  
            return false;  
        }  
  
  		// 记录停止线程
        _Stopping_thread = _Thrd_id();  
        // 执行所有回调
        for (;;) {  
            auto _Head = _Callbacks._Lock_and_load();  
            _Current_callback.store(_Head, memory_order_relaxed);  
            _Current_callback.notify_all();  
            if (_Head == nullptr) {  
                _Callbacks._Store_and_unlock(nullptr);  
                return true;  // 所有回调已执行完
            }  
  
            const auto _Next = _STD exchange(_Head->_Next, nullptr);  
            _STL_INTERNAL_CHECK(_Head->_Prev == nullptr);  
            if (_Next != nullptr) {  
                _Next->_Prev = nullptr;  
            }  
  
            _Callbacks._Store_and_unlock(_Next); // unlock before running _Head so other registrations  
            // can detach without blocking on the callback  
            _Head->_Fn(_Head); // might destroy *_Head  
        }  
    }  
};
```

`_Stop_state` 内部主要维护两个原子计数器和一个链表：

- `_Stop_sources`: source 引用计数（实际值/2）（位运算技巧，同时存储两个信息）
- `_Stop_tokens`: token 引用计数（实际值/1）
- `_Callbacks`：`stop_callback` 链表

```cpp
atomic<uint32_t> _Stop_sources = 2;
// 二进制: ...00000010
// 第0位(LSB): 停止请求标志位
// 第1-31位: 实际的source引用计数
```

`_Stop_sources` 用一个 32 位整数同时存储两个信息：是否请求停止，source 引用数

```cpp
atomic<uint32_t> _Stop_tokens = 1;
```
所有`stop_source`共享一个"虚拟token"