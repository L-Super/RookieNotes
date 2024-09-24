线程：每个线程相互独立运行，并且可以在不同的指令序列中运行。进程中的所有线程都共享地址空间，并且能访问到大部分数据，全局变量仍然是全局的，指针、对象的引用或数据可以在线程之间传递。

## std::thread

线程在std::thread对象创建时启动。

### join()与detach()

`join()`等待线程执行完成



`detach()`让线程在后台运行，这就意味着与主线程不能直接交互。如果线程分离，就不能有std::thread对象能引用它，分离线程的确在后台运行，所以分离的线程不能汇入。

C++运行库保证，当线程退出时，相关资源的能够正确回收。

分离线程通常称为守护线程(daemon threads)

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

## 互斥量

### std::mutex

通过实例化 `std::mutex` 创建互斥量实例，`lock()`可对互斥量上锁，`unlock()`为解锁。

> [!note]
>
> 成对使用，不允许非对称调用。


### std::lock_guard

RAII模板类，在构造时提供已锁的互斥量，并在析构时进行解锁，从而保证了互斥量能被正确解锁。

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

`std::unique_lock`比`std::lock_guard`更灵活，但效率差一点，内存占用多一点。

可在构造函数传入第二个参数进行管理：

- `std::adopt_lock`：表示互斥量已经`lock()`，不需要再次`lock()`
- `std::try_to_lock`：尝试用lock()去锁定mutex，如果没有锁定成功，也会立即返回，并不会阻塞在那里。

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

- lock() ：加锁
- unlock()：解锁
- try_lock()：尝试给互斥量加锁，如果拿不到锁，则返回false；拿到锁，返回true。不阻塞
- release()：返回它所管理的mutex对象指针，并释放所有权。即，unique_lock和mutex不再有关系

### std::lock

处理多个互斥量，避免死锁。

```cpp
std::lock(mymutex1, mymutex2);

std::lock_guard<mutex> myguard(mymutex1, std::adopt_lock);
std::lock_guard<mutex> myguard(mymutex2, std::adopt_lock);
```

### std::scoped_lock

C++17提供。`std::lock`的RAII包装类，通常它比裸调用 `std::lock` 更好。

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
  Complex()
    : i(0)
  {
  }
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

> [note!]
>
> 尽量不使用递归锁
>
> - 需要递归锁的地方往往可以简化，避免复杂逻辑
> - 递归锁效率更低一些
> - 虽然递归锁可以允许同一线程多次获得同一个互斥量，可重复获得的最大次数未定义，一旦超过一定次数，再对lock进行调用会抛出`std::system`错误

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

因为代码是外部提供的，所以没有办法确定外部的行为。外部程序可能做任何事情，包括获取锁。在持有锁的情况下，如果用外部代码要获取一个锁，就会违反第一个指导意见，并造成死锁(有时这是无法避免的)。

- 使用固定顺序获取锁

当硬性要求获取两个或两个以上的锁，并且不能使用 std::lock 单独操作来获取它们时，最好在每个线程上， 用固定的顺序获取它们(锁)。

- 使用层次锁结构

虽然，定义锁的顺序是一种特殊情况，但层次锁的意义在于，在运行时会约定是否进行检查。这个建议需要 对应用进行分层，并且识别在给定层上所有互斥量。当代码试图对互斥量上锁，而低层已持有该层锁时，不 允许锁定。可以通过每个互斥量对应的层数，以及每个线程使用的互斥量，在运行时检查锁定操作是否可以 进行。



## 条件变量

`std::condition_variable` 和 `std::condition_variable_any`，前者和mutex工作，后者和合适的互斥量工作。 `std::condition_variable_any` 更加通用，但在性能和系统资源的使用方面会有更多的开销。

通知：

- notify_one()：通知一个等待的线程
- notify_all()：通知所有等待的线程

等待：

- wait()：阻塞当前线程，直到条件变量被唤醒
- wait_for()：阻塞当前线程，直到条件变量被唤醒，或到指定时长后
- wait_until()：阻塞当前线程，直到条件变量被唤醒，或直到指定时间点后

```cpp
class A{
public:
void inMsgRecvQueue()
{
    for(int i=0;i<100000;++i)
    {
        std::unique_lock<std::mutex> guard(mymutex1);
        msgRecvQueue.push_back(i);
        my_cond.notify_one(); //尝试唤醒wait()，执行该语句后，就会被唤醒
        // .......
    }
}

void outMsgRecvQueue()
{
    int command = 0;
    while(true)
    {
        std::unique_lock<std::mutex> guard(my_mutex);
        // wait用于等待
        my_cond.wait(guard,[this]{
            if(!msgRecvQueue.empty())
                return true;
            return false;
        })；

        command = msgRecvQueue.front();
        msgRecvQueue.popfront();
    }
}
private:
    std::list<int> msgRecvQueue;
    std::mutex my_mutex;
    std::condition_variable my_cond;
};
```

如果wait()第二个参数返回值是false，那么wait()将解锁互斥量，并阻塞至本行，堵塞到其他线程调用notify_one()成员函数为止。如果是true，直接返回。

如果没有第二个参数，那么跟返回false一样。

当其他线程用notify_one()将本wait()唤醒：

1. wait()不断尝试重新获取互斥量锁，如果获取不到，就会阻塞这里等待获取;如果获取到，就继续执行第二步
2.  如果wait()有第二个参数，就判断这个lambda表达式，如果表达式为false，那么wait将解锁互斥量，并堵塞到本行，等待再次唤醒；如果为true，则wait()返回，执行下一句流程，此时互斥锁加锁；如果wiat()没有第二个参数，则wait()返回，执行下一句流程



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

首先，队列中中有两个线程，两个线程之间会对数据`data_queue`进行传递

`wait()`会去检查这些条件(通过Lambda函数)，当条件满足(Lambda函数返回true)时返回。如果条件不满足 (Lambda函数返回false)，wait()将解锁互斥量，并且置于阻塞或等待状态。

当准备数据的线程调用`notify_one()`通知条件变量时，处理数据的线程从睡眠中苏醒，重新获取互斥锁，并再次进行条件检查。在条件满足的情况下，从wait()返回并继续持有锁。当条件不满足时，线程将对互斥量解锁，并重 新等待。这就是为什么用 `std::unique_lock` 而不使用 `std::lock_guard` 的原因——等待中的线程必须在等待期间解锁互斥量，并对互斥量再次上锁，而 std::lock_guard 没有这么灵活。
