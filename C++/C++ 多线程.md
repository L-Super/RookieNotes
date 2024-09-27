线程：每个线程相互独立运行，并且可以在不同的指令序列中运行。进程中的所有线程都共享地址空间，并且能访问到大部分数据，全局变量仍然是全局的，指针、对象的引用或数据可以在线程之间传递。

## std::thread

线程在 std:: thread 对象创建时启动。


- `join()` 等待线程执行完成
- `detach()` 让线程在后台运行，这就意味着与主线程不能直接交互。如果线程分离，就不能有 `std::thread` 对象能引用它，分离线程的确在后台运行，所以分离的线程不能汇入。分离线程通常称为守护线程 (daemon threads)

C++运行库保证，当线程退出时，相关资源的能够正确回收。

### 传递参数

```cpp
int main()
{
    int n = 0;
    foo f;
    baz b;
    std::thread t1; // t1 不是线程
    std::thread t2(f1, n + 1); // 按值传递
    std::thread t3(f2, std::ref(n)); // 按引用传递
    std::thread t4(std::move(t3)); // t4 现在运行 f2()。t3 不再是线程
    std::thread t5(&foo::bar, &f); // t5 在对象 f 上运行 foo::bar()
    std::thread t6(b); // t6 在对象 b 的副本上运行 baz::operator()
    t2.join();
    t4.join();
    t5.join();
    t6.join();
}
```

`std::ref` 强制传引用，以达到修改值的作用。

### 管理当前线程

| 函数        | 说明                                 |
| :---------- | :----------------------------------- |
| yield       | 让出处理器，重新调度各执行线程       |
| get_id      | 返回当前线程的线程 id                |
| sleep_for   | 使当前线程的执行停止指定的时间段     |
| sleep_until | 使当前线程的执行停止直到指定的时间点 |

上面是一些在线程内部使用的 API，它们用来对当前线程做一些控制。

## 互斥量

### std::mutex

通过实例化 `std::mutex` 创建互斥量实例，`lock()` 可对互斥量上锁，`unlock()` 为解锁。

> [!note]
>
> 成对使用，不允许非对称调用。

### std::lock_guard

RAII 模板类，在构造时提供已锁的互斥量，并在析构时进行解锁，从而保证了互斥量能被正确解锁。

```cpp
std::lock_guard<std::mutex> guard(some_mutex);

std::lock_guard guard(some_mutex); // C++17 模板类参数推导

// 锁定两个互斥体而不死锁
std::lock(m1, m2);
// 保证两个已锁定互斥体在作用域结尾解锁
std::lock_guard<std::mutex> lock1(m1, std::adopt_lock);
std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);
```

### std::unique_lock

`std::unique_lock` 比 `std::lock_guard` 更灵活，但效率差一点，内存占用多一点。

可在构造函数传入第二个参数进行管理：

- `std::adopt_lock`：表示互斥量已经 `lock()`，不需要再次 `lock()`
- `std::try_to_lock`：尝试用 lock () 去锁定 mutex，如果没有锁定成功，也会立即返回，并不会阻塞在那里。

- `std::defer_lock`：不进行加锁

```cpp
/*********/
// 锁定两个互斥体而不死锁
std::lock(m1, m2);
// 保证两个已锁定互斥体在作用域结尾解锁
std::unique_lock<std::mutex> lock1(m1, std::adopt_lock);
std::unique_lock<std::mutex> lock2(m2, std::adopt_lock);

/*********/
unique_lock<mutex> lk_1(mymutex, try_to_lock);
if(lk_1.owns_lock())
{
	//拿到锁, 执行操作
}

/*********/
unique_lock<mutex> lk_2(mymutex, defer_lock); // 未加锁的mymutex
lk_2.lock();
// some process
lk_2.unlock();
// ........

lk_2.lock();//可再次lock
```

成员函数：

- `lock()` ：加锁
- `unlock()`：解锁
- `try_lock() `：尝试给互斥量加锁，如果拿不到锁，则返回 false；拿到锁，返回 true。不阻塞
- `release()`：返回它所管理的 mutex 对象指针，并释放所有权。即，unique_lock 和 mutex 不再有关系

### std::lock

处理多个互斥量，避免死锁。

```cpp
std::lock(mymutex1, mymutex2);

std::lock_guard<mutex> myguard(mymutex1, std::adopt_lock);
std::lock_guard<mutex> myguard(mymutex2, std::adopt_lock);
```

### std::scoped_lock

C++17 提供。`std::lock` 的 RAII 包装类，通常它比裸调用 `std::lock` 更好。

```cpp
std::scoped_lock lock(m1, m2);
```
### std::recursive_mutex

递归锁允许同一线程多次获得该互斥锁，用于解决同一线程需要多次获取互斥量时死锁的问题。

```cpp
struct Complex
{
  std::mutex mutex;
  int i;
  Complex() : i(0) {}
  void mul(int x)
  {
    std::lock_guard<std::mutex> lock(mutex);
    1 *= x;
  }
  void div(int x)
  {
    std::lock_guard<std::mutex> lock(mutex);
    i /= x;
  }
  void both(int x, int y)
  {
    std::lock_guard<std::mutex> lock(mutex);
    mul(x);
    div(y);
  }
};

int main()
{
  Complex complex;
  complex.both(32, 23);
  return 0;
}
```

这段代码运行后会发生死锁，因为在调用`both()`时获取了互斥量，之后再调用`mul()`又获取相同的互斥量，但是这个互斥量已经被当前线程获取了，无法释放，这时就会发生死锁。

解决这个死锁的问题，一个简单的办法就是用递归锁（`std::recursive_mutex`），它允许同一线程多次获得互斥量：

```cpp
struct Complex
{
  std::recursive_mutex mutex;
  int i;
  Complex()
    : i(0)
  {
  }
  void mul(int x)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex);
    1 *= x;
  }
  void div(int x)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex);
    i /= x;
  }
  void both(int x, int y)
  {
    std::lock_guard<std::recursive_mutex> lock(mutex);
    mul(x);
    div(y);
  }
};

int main()
{
  Complex complex;
  complex.both(32, 23);
  return 0;
}
```

> [!note]
>
> 尽量不使用递归锁
>
> - 需要递归锁的地方往往可以简化，避免复杂逻辑
> - 递归锁效率更低一些
> - 虽然递归锁可以允许同一线程多次获得同一个互斥量，可重复获得的最大次数未定义，一旦超过一定次数，再对 lock 进行调用会抛出`std::system`错误

### std::timed_mutex

超时的独占锁。通过 `try_lock_for()` 和 `try_lock_until()` 方法，提供尝试带超时的互斥量。

```cpp
using namespace std::chrono_literals;
 
std::mutex cout_mutex; // 控制到 std::cout 的访问
std::timed_mutex mutex;
 
void job(int id)
{
    // 尝试在100毫秒内获取锁
    if (mutex.try_lock_for(100ms))
    {
        //  成功获取锁
        std::this_thread::sleep_for(100ms);
        mutex.unlock();
    }
    else
    {
        // 获取锁失败
    }
}
```

### std::shared_mutex 

C++ 17 读写锁，写线程独占访问，读线程共享并发访问。

```cpp
class dns_entry; 
 
class dns_cache 
{ 
  std::map<std::string,dns_entry> entries; 
  mutable std::shared_mutex entry_mutex; 
public: 
  dns_entry find_entry(std::string const& domain) const 
  { 
    // 护共享和只读权限，得多线程可以同时调用
    std::shared_lock<std::shared_mutex> lk(entry_mutex);
    std::map<std::string,dns_entry>::const_iterator const it= 
       entries.find(domain); 
    return (it==entries.end())?dns_entry():it->second; 
  } 
  void update_or_add_entry(std::string const& domain, 
                           dns_entry const& dns_details) 
  { 
    // 提供独占访问权限，会阻止其他线程对数据进行修改，并且阻止线程调用find_entry()。
    std::lock_guard<std::shared_mutex> lk(entry_mutex);
    entries[domain]=dns_details; 
  } 
}; 
```


## 死锁

死锁通常是对锁的使用不当造成。

### 避免死锁指导

- 避免嵌套锁

线程获得一个锁时，别再取获取第二个。每个线程只持有一个锁，就不会产生死锁。当需要获取多个锁时，使用`std::lock`、`std::scoped_lock`，避免死锁。

- 避免在持有锁时调用外部代码

因为代码是外部提供的，所以没有办法确定外部的行为。外部程序可能做任何事情，包括获取锁。在持有锁的情况下，如果用外部代码要获取一个锁，就会违反第一个指导意见，并造成死锁 (有时这是无法避免的)。

- 使用固定顺序获取锁

当硬性要求获取两个或两个以上的锁，并且不能使用 std:: lock 单独操作来获取它们时，最好在每个线程上，用固定的顺序获取它们 (锁)。

- 使用层次锁结构

虽然，定义锁的顺序是一种特殊情况，但层次锁的意义在于，在运行时会约定是否进行检查。这个建议需要对应用进行分层，并且识别在给定层上所有互斥量。当代码试图对互斥量上锁，而低层已持有该层锁时，不允许锁定。可以通过每个互斥量对应的层数，以及每个线程使用的互斥量，在运行时检查锁定操作是否可以进行。

## std::call_once

仅调用函数一次，即使从多个线程调用。

该调用中抛出了异常，那么异常将传播给 `std::call_once` 的调用方，并且不翻转 flag，这样后续还可以尝试继续调用。

```cpp
std::once_flag flag1, flag2;
 
void simple_do_once()
{
    std::call_once(flag1, [](){ std::cout << "简单样例：调用一次\n"; });
}
 
void may_throw_function(bool do_throw)
{
    if (do_throw)
    {
        std::cout << "抛出：call_once 会重试\n"; // 这会出现不止一次
        throw std::exception();
    }
    std::cout << "没有抛出，call_once 不会再重试\n"; // 保证一次
}
 
void do_once(bool do_throw)
{
    try
    {
        std::call_once(flag2, may_throw_function, do_throw);
    }
    catch (...) {}
}
 
int main()
{
    std::thread st1(simple_do_once);
    st1.join();
 
    std::thread t1(do_once, true); // 抛出异常，后续可继续调用
    std::thread t2(do_once, true); // 抛出异常，后续可继续调用
    std::thread t3(do_once, false); // 执行成功，后续不再调用
    std::thread t4(do_once, true); // 不再调用
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
```

## 条件变量

`std::condition_variable` 和 `std::condition_variable_any`，前者和 mutex 工作，后者和合适的互斥量工作。 `std::condition_variable_any` 更加通用，但在性能和系统资源的使用方面会有更多的开销。

通知：

- `notify_one()`：通知一个等待的线程
- `notify_all() `：通知所有等待的线程

等待：

- `wait()`：阻塞当前线程，直到条件变量被唤醒
- `wait_for() `：阻塞当前线程，直到条件变量被唤醒，或到指定时长后
- `wait_until()`：阻塞当前线程，直到条件变量被唤醒，或直到指定时间点后

```cpp
std::mutex mut; 
std::queue<data_chunk> data_queue; 
std::condition_variable data_cond; 
 
void data_preparation_thread() 
{ 
  while(more_data_to_prepare()) 
  { 
    data_chunk const data=prepare_data(); 
    std::lock_guard<std::mutex> lk(mut);
    // 上锁，压入队列
    data_queue.push(data);
    // 对等待的线程(如果有等待线程)进行通知
    data_cond.notify_one();
  } 
} 
 
void data_processing_thread() 
{ 
  while(true) 
  { 
    // 需使用unique_lock
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait( 
         lk,[]{return !data_queue.empty();});  // 5 
    data_chunk data=data_queue.front(); 
    data_queue.pop(); 
    lk.unlock();  // 6 
    process(data); 
    if(is_last_chunk(data)) 
      break; 
  } 
} 
```

首先，队列中有两个线程，两个线程之间会对数据`data_queue`进行传递

`wait()` 会去检查这些条件 (通过 Lambda 函数)，当条件满足 (Lambda 函数返回 true) 时返回。如果条件不满足 (Lambda 函数返回 false)，`wait()` 将解锁互斥量，并且置于阻塞或等待状态。

当准备数据的线程调用 `notify_one()` 通知条件变量时，处理数据的线程从睡眠中苏醒，重新获取互斥锁，并再次进行条件检查。在条件满足的情况下，从 `wait()` 返回并继续持有锁。当条件不满足时，线程将对互斥量解锁，并重新等待。这就是为什么用 ` std::unique_lock ` 而不使用 ` std::lock_guard ` 的原因——等待中的线程必须在等待期间解锁互斥量，并对互斥量再次上锁，而 std:: lock_guard 没有这么灵活。


如果 `wait()` 第二个参数返回值是 false，那么 `wait()` 将解锁互斥量，并阻塞至本行，堵塞到其他线程调用 `notify_one()` 成员函数为止。如果是 true，直接返回，执行下一条语句。

如果没有第二个参数，那么跟返回 false 一样。

当其他线程用 `notify_one()` 将本 `wait()` 唤醒：

1. `wait()` 不断尝试重新获取互斥量锁，如果获取不到，就会阻塞这里等待获取; 如果获取到，就继续执行第二步
2. 如果 `wait()` 有第二个参数，就判断这个 lambda 表达式，如果表达式为 false，那么将解锁互斥量，并堵塞到本行，等待再次唤醒；如果为 true，则 `wait()` 返回，执行下一句流程，此时互斥锁加锁；如果 `wiat()` 没有第二个参数，则 `wait()` 返回，执行下一句流程

## std::future

C++标准库中有两种 future，unique future ( `std::future` ) 和 shared futures ( `std::shared_future` )。`std::future`只能与指定事件相关联，而 `std::shared_future` 能关联多个事件。

当线程需要等待特定事件时，某种程度上来说需要知道期望的结果。

当调用抛出一个异常时，这个异常会存储到 future 中，future 的状态置为“就绪”，之后调用 `get()` 会抛出已存储的异常。

> [!note]
> 一个 future 上只能调用一次 `get()` 函数，第二次调用为未定义行为，通常导致程序崩溃。
> 每个 `shared_future` 上仍然还是只能调用一次 `get()` 函数。

> C++ 标准建议实现在调用 `get()` 前检测 ` valid () ` 是 ` false ` 的情况，并抛出以 ` std::future_errc::no_state ` 作为错误条件的 ` std::future_error `。

```cpp
int main()
{
    std::promise<int> p;
    std::future<int> f = p.get_future();
 
    std::thread t([&p]
    {
        try
        {
            // 抛出异常的代码
            throw std::runtime_error("Example");
        }
        catch (...)
        {
            try
            {
                // 存储 promise 中抛出的任何异常
                p.set_exception(std::current_exception());
            }
            catch (...) {} // set_exception() 也可能抛出异常
        }
    });
 
    try
    {
        std::cout << f.get();
    }
    catch (const std::exception& e)
    {
        std::cout << "来自线程的异常: " << e.what() << '\n';
    }
    t.join();
}
```

这里使用`std::current_exception()`来检索抛出的异常。当异常类型已知时，可用 `std::copy_exception()` 作为替代方案，`std::copy_exception()`会直接存储新的异常而不抛出:

```cpp
some_promise.set_exception(std::copy_exception(std::logic_error("foo ")));
```
`std::shared_future`的实例同步`std::future`实例的状态。当`std::future`对象没有与其他对象共享同步状态所有权，那么所有权必须使用`std::move`将所有权传递到`std::shared_future`

```cpp
std::promise<int> p;
std::future<int> f(p.get_future()); 
assert(f.valid()); // f 是合法的 
std::shared_future<int> sf(std::move(f)); 
assert(!f.valid()); // f 现在是不合法的 
assert(sf.valid()); // sf 现在是合法的
```

future 也有`wait_for()`与 `wait_util()`，并返回`future_status`

| 常量                      | 解释                                                         |
| ------------------------- | ------------------------------------------------------------ |
| `future_status::deferred` | 共享状态包含一个使用惰性求值的延迟函数，仅在明确请求时才计算其结果 |
| `future_status::ready`    | 共享状态已就绪                                               |
| `future_status::timeout`  | 已超时                                                       |

```cpp
int main()
{
    std::future<int> future = std::async(std::launch::async, []()
    {
        std::this_thread::sleep_for(3s);
        return 8;
    });
 
    std::cout << "等待...\n";
    std::future_status status;
 
    do
    {
        switch (status = future.wait_for(1s); status)
        {
            case std::future_status::deferred:
                std::cout << "延后\n";
                break;
            case std::future_status::timeout:
                std::cout << "超时\n";
                break;
            case std::future_status::ready:
                std::cout << "就绪!\n";
                break;
        }
    }
    while (status != std::future_status::ready);
 
    std::cout << "结果为 " << future.get() << '\n';
}
```

## std::async

启动一个异步任务。 std:: async 允许通过添加额外的调用参数，向函数传递额外的参数。

std:: launch 枚举类型：

- `launch::defered`：表明函数调用延迟到 wait () 或 get () 函数调用时才执行
- `launch::async`：表明函数必须在其所在的独立线程上执行
- `launch::deferred | launch::async`：`std::async`的默认参数，系统会自行决定异步（创建新线程）还是同步（不创建新线程）方式运行。

```cpp
int main()
{
  std::future<int> the_answer=std::async(find_the_answer_to_ltuae);
  do_other_stuff();
  // get()阻塞线程直到future为就绪为止，并返回计算结果。
  std::cout<<"The answer is "<<the_answer.get()<<std::endl;
}
```

>[!note]
>如果从 `std::async` 获得的 `std::future` 没有被移动或绑定到引用，那么在完整表达式结尾， `std::future`的析构函数将阻塞到异步计算完成，实质上令如下代码同步：
>
>```cpp
>std::async(std::launch::async, []{ f(); }); // 临时量的析构函数等待f()
>std::async(std::launch::async, []{ g(); }); // f() 完成前不开始
>```
>

## std::packaged_task

打包任务，把任务包装起来。会将 future 与函数或可调用对象进行绑定。当调用 `std::packaged_task`对象时，就会调用相关函数或可调用对象，当 future 状态为就绪时，会存储返回值。这可以用在构建线程池或其他任务的管理中。

```cpp
int f(int x, int y) { return std::pow(x,y); }
 
void task_lambda()
{
    std::packaged_task<int(int, int)> task([](int a, int b){
        return std::pow(a, b); 
    });
    std::future<int> result = task.get_future();
 
    task(2, 9);
 
    std::cout << "task_lambda:\t" << result.get() << '\n';
}
 
void task_bind()
{
    std::packaged_task<int()> task(std::bind(f, 2, 11));
    std::future<int> result = task.get_future();
 
    task();
 
    std::cout << "task_bind:\t" << result.get() << '\n';
}
 
void task_thread()
{
    std::packaged_task<int(int, int)> task(f);
    std::future<int> result = task.get_future();
 
    std::thread task_td(std::move(task), 2, 10);
    task_td.join();
 
    std::cout << "task_thread:\t" << result.get() << '\n';
}
```

## std::promise

 `std::promise`和`std::future` 提供一种机制：future 可以阻塞等待线程，提供数据的线程可以使用 promise 对相关值进行设置，并将 future 的状态置为“就绪”。

```cpp
void accumulate(std::vector<int>::iterator first,
                std::vector<int>::iterator last,
                std::promise<int> accumulate_promise)
{
    int sum = std::accumulate(first, last, 0);
    accumulate_promise.set_value(sum); // 提醒 future
}
 
void do_work(std::promise<void>& barrier)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    barrier.set_value();
}
 
int main()
{
    // 用 promise<int> 在线程间传递结果。
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
    std::promise<int> accumulate_promise;
    std::future<int> accumulate_future = accumulate_promise.get_future();
    std::thread work_thread(accumulate, numbers.begin(), numbers.end(),
                            std::move(accumulate_promise));
 
    // future::get() 将等待直至该 future 拥有合法结果并取得它
    // 无需在 get() 前调用 wait()
    // accumulate_future.wait(); // 等待结果
    std::cout << "result=" << accumulate_future.get() << '\n';
    work_thread.join(); // wait for thread completion
 
    // 用 promise<void> 在线程间对状态发信号
    std::promise<void> barrier;
    std::future<void> barrier_future = barrier.get_future();
    std::thread new_work_thread(do_work, std::ref(barrier)); // 使用ref传引用
    barrier_future.wait();
    new_work_thread.join();
}
```

## 并发相关的错误

通常，并发相关的错误通常有两大类:

- 不必要的阻塞
- 条件竞争

### 不必要的阻塞

线程阻塞的时候，不能处理任何任务，因为它在等待其他“条件”的达成。通常这些“条件”就是一个互斥量、一个条件变量或一个future，也可能是一个I/O操作。

分为几类：

- 死锁：死锁的情况下，两个线程会互相等待。当线程产生死锁，应该完成的任务就会搁置。如，某线程是负责对用户界面操作的线程，在死锁的情况下，用户界面就会无响应。
- 活锁：与死锁的情况类似。不同的地方在于线程不是阻塞等待，而是在循环中持续检查，例如:自旋锁。比较严重的情况下，其表现和死锁一样(应用不会做任何处理，停止响应)，因为线程还在循环中被检查，而不是阻塞等待，所以CPU的使用率还居高不下。不太严重的情况下，使用随机调度，活锁的问题还可以解决。
- I/O阻塞或外部输入：当线程被外部输入所阻塞，线程也就不能做其他事情了（即使，等待输入的情况永远不会发生）。



### 条件竞争

很多条件竞争表现为死锁与活锁。而且，并非所有条件竞争都是恶性的 ——对独立线程相关操作的调度，决定了条件竞争发生的时间。很多条件竞争是良性的，比如:哪一个线程 去处理任务队列中的下一个任务。不过，很多并发错误的引起也是因为条件竞争。

分为几类：

- 数据竞争：因为未同步访问一块共享内存，将会导致代码产生未定义行为。数据竞争通常发生在错误的使用原子操作上，做同步线程的时候，或没使用互斥量保护共享数据时。
- 破坏不变量：主要表现为悬空指针（因为其他线程已经将要访问的数据删除了），随机存储错误（因为局部更新，导致线程读取了不一样的数据），以及双重释放（比如：当两个线程对同一个队列同时执行pop操作，想要删除同一个关联数据）等等。破坏不变量可以看作为“基于数据”的问题。当独立线程需要以一定顺序执行某些操作时，错误的同步会导致条件竞争，比如：破坏访问顺序。
- 生命周期问题：线程会访问不存在的数据，这可能是因为删除或销毁了数据，或者转移到其他对象中去了。 生命周期问题，通常是在一个线程引用了局部变量，在线程还没有完成前，局部变量已释放。不过这个问题并不只存在这种情况下。当手动调用`join()`等待线程完成工作，需要保证异常抛出时，`join()`还会等待其他未完成工作的线程。这是线程中基本异常安全的应用。（虽然这类问题也能归结为破坏了不变量，不过这里将其作为一个单独的类别给出。）





> 《C++ 并发编程-第 2 版》
> 
> [并发支持库 ](https://zh.cppreference.com/w/cpp/thread)
>
> [C++ 并发编程（从C++11到C++17）](https://paul.pub/cpp-concurrency/)
