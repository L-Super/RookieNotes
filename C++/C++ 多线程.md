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

通过 `try_lock_for()` 和 `try_lock_until()` 方法，提供尝试带超时的互斥量。

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

