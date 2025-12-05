## 1. 核心概念：建立心智模型

在学习代码之前，先理解什么是“异步”和“协程”。

### 这是一个“单线程”的戏法
Python 的 `asyncio` 是**单线程**的。这意味着同一时刻 CPU 只能做一件事。那它怎么实现并发（看起来像同时做几件事）呢？

**比喻：餐厅服务员**

*   **多线程/多进程 (Traditional)**：一家餐厅雇了 10 个服务员。每个服务员只服务一桌，客人还在看菜单，服务员就傻等着，直到客人点完菜才去服务下一桌。
*   **协程 (Asyncio)**：一家餐厅只有 **1 个服务员**（即单线程）。
    *   服务员去 A 桌点菜，A 说“我再看 2 分钟菜单”。
    *   服务员**不会傻等**，而是利用这 2 分钟，转身去 B 桌结账。
    *   B 桌结完账，A 桌正好准备好了，服务员再回来服务 A。

**关键词：**

*   **Event Loop (事件循环)**：就是那个唯一的“服务员”，不停地在各个任务之间巡视、切换。
*   **Coroutine (协程)**：这是一个可以被“暂停”和“恢复”的函数。
*   **Await (等待)**：就是“交出控制权”。当你遇到需要等待 IO（网络请求、文件读写、睡觉）的时候，使用 `await` 告诉 Event Loop：“我要等一会儿，你先去忙别的吧。”

---

## 2. 只有三个核心对象 (Awaitables)

在文档中，你经常看到“可等待对象（Awaitable）”这个词。只有三种东西放在 `await` 后面是合法的：

1.  **协程 (Coroutines)**: 用 `async def` 定义的函数。
2.  **任务 (Tasks)**: 将协程打包，以便在后台“并发”运行。
3.  **未来对象 (Futures)**: 底层对象，通常由库编写者处理，代表一个未来会有结果的占位符。

---

## 3. 代码

`asyncio` 是 Python 中用于编写异步代码的标准库，可以轻松创建、执行和管理异步任务。通过 async/await 语法来声明[协程](https://docs.python.org/zh-cn/3/glossary.html#term-coroutine) 是编写 asyncio 应用的推荐方式。

`asyncio` 的编程模型是一个消息循环。

`async` 关键字用于定义异步函数，`await` 关键字用于暂停异步函数的执行，等待另一个异步任务完成。

### 定义与运行

普通的函数调用是直接执行，而协程调用只会返回一个对象，不会执行。必须把它扔进 Event Loop 才能动。

```python
import asyncio
import time

# 1. 定义协程：必须使用 async def
async def say_hello():
    print("开始...")
    # 2. 让出控制权：模拟IO操作（比如网络请求），这里必须用 asyncio.sleep，不能用 time.sleep
    await asyncio.sleep(1) 
    print("...结束")

# 3. 入口点
if __name__ == "__main__":
    # asyncio.run() 负责创建 Loop，运行协程，最后关闭 Loop
    asyncio.run(say_hello())
```

### 串行 vs 并发 (由 Task 决定)

这是新手最容易困惑的地方：**写了 `async` 不代表就是并发**。

**❌ 错误的并发（仍然是串行）：**
如果你直接 `await` 一个接一个的协程，它们还是按顺序执行的。

```python
import asyncio
import time

async def do_work(name, delay):
    print(f"{name} 开始工作")
    await asyncio.sleep(delay)
    print(f"{name} 完成工作")

async def main():
    start = time.time()
  
    # 这样写是串行的！因为 await 会阻塞在这里直到函数返回
    await do_work("A", 2) 
    await do_work("B", 2) 
  
    print(f"总耗时: {time.time() - start:.2f} 秒") # 结果是 4 秒

if __name__ == "__main__":
    asyncio.run(main())
```

**✅ 正确的并发（使用 Task）：**
要让它们同时跑，必须把协程包装成 **Task**。`asyncio.create_task()` 会立即把协程扔进 Event Loop 调度，不需要等待它完成就可以往下走。

```python
async def main():
    start = time.time()
  
    # 1. 创建任务：这相当于告诉 Loop "你去后台运行这两个任务"
    task1 = asyncio.create_task(do_work("A", 2))
    task2 = asyncio.create_task(do_work("B", 2))
  
    print("任务已创建，正在后台运行...")
  
    # 2. 等待结果：此时 task1 和 task2 已经在同时也运行了
    await task1
    await task2
  
    print(f"总耗时: {time.time() - start:.2f} 秒") # 结果是 2 秒

if __name__ == "__main__":
    asyncio.run(main())
```

### 批量管理 (Gather)

如果有 100 个任务，手写 100 个 `create_task` 不现实。通常使用 `asyncio.gather`。

```python
async def main():
    # 创建一个任务列表
    tasks = [do_work(f"Worker-{i}", 1) for i in range(3)]
  
    # gather 会自动调度这些协程，并收集所有结果
    # 它会并发运行列表里的所有任务
    results = await asyncio.gather(*tasks)
    
    await asyncio.gather(
        do_work("A", 2),
        do_work("B", 3),
        do_work("C", 4),
    )
  
    # results 是一个列表，包含了每个协程的返回值
    print("所有任务完成")
```

Python 3.11+ 引入了 `asyncio.TaskGroup`，它是 `gather` 的更安全替代品

```python
async def main():
    async with asyncio.TaskGroup() as tg:
        task1 = tg.create_task(do_work("A", 2))

        task2 = tg.create_task(do_work("B", 2))

        print(f"started at {time.strftime('%X')}")

    # 当存在上下文管理器时 await 是隐式执行的。

    print(f"finished at {time.strftime('%X')}")
```

`async with` 语句将等待分组中的所有任务结束。在等待期间，仍可将新任务添加到分组中 (例如，通过将 `tg` 传入某个协程并在该协程中调用 `tg.create_task()`)。一旦最后的任务完成并退出 `async with` 代码块，将无法再向分组添加新任务。

### 第四阶段：超时控制 (wait_for)

防止某个协程卡死无限等待。

```python
async def eternal_task():
    await asyncio.sleep(3600) # 睡一个小时
    return "醒了"

async def main():
    try:
        # 如果 1 秒内没做完，就强制抛出 TimeoutError
        await asyncio.wait_for(eternal_task(), timeout=1)
    except asyncio.TimeoutError:
        print("任务超时了！")
```

---

## 4. 常见误区与警告 (必读)

这是从官方文档中提炼出的**坑**，一定要注意：

1.  **别用 `time.sleep()`**
    
    *   `time.sleep()` 会阻塞整个线程。如果你在一个 `async` 函数里写了 `time.sleep(10)`，整个程序（包括所有其他并发任务）都会卡死 10 秒。
    *   **必须用** `await asyncio.sleep(10)`。
    
2.  **普通的库可能是阻塞的**
    *   如果你用 `requests` 库去发网络请求，它是不支持 async 的，会像 `time.sleep` 一样卡死 Loop。
    *   **解决**：使用支持异步的库（如 `httpx` 或 `aiohttp`），或者使用 `asyncio.to_thread()` 把同步代码扔到线程里跑。

    ```python
    # 假如必须用 requests (这是个同步库)
    def blocking_io():
        return requests.get("http://example.com")
    
    async def main():
        # 在异步函数里运行同步代码，使用 to_thread (Python 3.9+)
        response = await asyncio.to_thread(blocking_io)
    ```

3.  **不要到处 `async`**
    *   如果一个函数里没有 `await` 操作（纯计算），没必要把它定义成 `async`。协程不仅是为了好玩，是有切换开销的。它的优势在于 IO 密集型任务（读写文件、数据库、网络），而不是 CPU 密集型任务（算圆周率）。

---

## 5. 总结：协程编写模板

当你写 Python 协程时，脑子里要有这个模板：

1.  **定义**：`async def` 函数。
2.  **IO 操作**：遇到 IO 就 `await`。
3.  **并发**：
    *   简单并发：`asyncio.create_task()`
    *   批量并发：`asyncio.gather()`
4.  **启动**：`asyncio.run(main())`
