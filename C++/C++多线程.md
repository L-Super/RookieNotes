# C++多线程

管理线程的函数和类在头文件`<thread>`中声明，而保护共享数据的函数和类在其他头文件中声明。

thread参数为可调用对象

```cpp
void do_some_work();
std::thread my_thread(do_some_work);
```

也可使用类对象
```C++
class background_task
{
public:
  void operator()() const
  {
    do_something();
    do_something_else();
  }
};
background_task f;
std::thread my_thread(f);
```

提供的函数对象会复制到新线程的存储空间当中，函数对象的执行和调用都在线程的内存空间中进行。函数对象的副本应与原始函数对象保持一致，否则得到的结果会与我们的期望不同。

注：

函数名不能带括号，即

```c++
std::thread my_thread(background_task()); //错误
```

相当于声明了一个名为my_thread的函数，带有一个参数(函数指针指向没有参数并返回background_task对象的函数)，返回一个`std::thread`类型的函数，而非启动了一个线程。

使用多组括号①，或使用新统一的初始化语法②，可以避免这个问题：

```c++
std::thread my_thread((background_task()));  // 1
std::thread my_thread{background_task()};    // 2
```

使用lambda表达式也能避免这个问题。

获取线程id

```
this_thread::get_id()
```

## join、detach

join等待线程完成函数，主线程需等待子线程运行结束后才可以结束

detach分离线程函数，又叫守护线程，会让线程在后台运行，即说明主线程不会等待子线程运行结束才结束

```c++
// 可调用对象
thread obj(createthread);
// 判断返回线程对象是否是joinable的
if (obj.joinable())
{
    cout << "可使用join" << endl;
}
//obj.detach();
obj.join();
```

有参线程

```c++
void parameterthread(int i, string &str)

string str("有参线程");
// std::ref强制传引用，已达到修改值的作用
thread obj1(parameterthread, i, std::ref(str));
```

std::ref 强制传引用，以达到修改值的作用，使用std::ref可以在模板传参的时候传入引用，否则无法传递

类对象

```c++
//类对象
mythread m;
thread obj2(m);
obj2.join();
```

类对象容器

```c++
vector<thread> mythread;
for (int i = 0; i < 10; i++)
{
    mythread.push_back(thread(printthread, i));
}
for (auto it = mythread.begin(); it != mythread.end(); it++)
{
    it->join();
}
```



## 互斥量

互斥量是个类对象。多个线程用lock()锁，unlock()解锁

用法：

包含头文件

```
#include<mutex>
```

创建对象

```c++
std::mutex mymutex;//创建一个互斥量

mymutex.lock();
....
mymutex.unlock();
```

先lock()操作共享数据，再unlock()，成对使用。不允许非对称调用。

**lock_guard**

防止忘记unlock()，引入了lock_guard的类模板，替你unlock()

直接取代lock()和unlock()，使用lock_guard之后，不能再使用lock()和unlock()

```c++
lock_guard<mutex> myguard(mymutex);
```

构造函数里执行了lock()，当退出函数作用域时，调用析构函数执行unlock()

## 死锁

至少两个互斥量才会产生死锁

一般解决方案：

保证两个互斥量上锁顺序一致就不会死锁

---------------------



std::lock()**函数模板**

处理多个互斥量

一次锁住两个或两个以上的互斥量，不存在死锁问题。

要么两个互斥量都锁住，要么两个互斥量都没锁住。如果只锁一个，另一个没锁成功，则立即把已锁的解锁。

```c++
std::lock(mymutex1,mymutex2);
.......
mymutex1.unlock();
mymutex2.unlock();
```

缺点：仍需要手动unlock()



改进:

实现处理多个互斥量上锁解锁

```C++
std::lock(mymutex1,mymutex2);

lock_guard<mutex> myguard(mymutex1,adopt_lock);
lock_guard<mutex> myguard(mymutex2,adopt_lock);
```

std::adopt_lock是结构体对象，标记作用，表示这个互斥量已经lock()，不需要在lock_guard\<mutex>里再次lock()

## unique_lock

取代lock_guard，unique_lock是类模板，更灵活，但效率差一点，内存占用多一点。

```
unique_lock<mutex> myunique(mymutex);
```

**第二个参数:**

+ adopt_lock

unique_lock也可以带adopt_lock标记，不希望在unique_lock构造函数中lock这个mutex

+ try_to_lock

尝试用lock()去锁定mutex，如果没有锁定成功，也会立即返回，并不会阻塞在那里。

前提是自己不能先去lock

```c++
unique_lock<mutex> myunique(mymutex,try_to_lock);
if(myunique.owns_lock())
{
	//拿到锁
	....//执行操作
}
```

```c++
std::chrono::milliseconds dura(20000); //1s = 1000ms
std::this_thread::sleep_for(dura); //休息一定时长，这里休息二十秒
```

+ defer_lock

前提不能自己先lock，否则异常

没有给mutex加锁，初始化了一个没有加锁的mutex

```c++
unique_lock<mutex> myunique(mymutex,defer_lock); //没有加锁的mymutex
myunique.lock();
//处理共享代码
myunique.unlock();
........
//非共享
myunique.lock();//可再次lock
//此时也可不手动unlock
```

### unique_lock的成员函数

+ lock() ：加锁
+ unlock()：解锁
+ try_lock()：尝试给互斥量加锁，如果拿不到锁，则返回false；拿到锁，返回true。不阻塞

+ release()：返回它所管理的mutex对象指针，并释放所有权。也就是说，unique_lock和mutex不再有关系

```
unique_lock<mutex> myunique(mymutex);
mutex *ptx = myunique.release();
....
ptx->unlock();//自己负责unlcok
```

例子： [mutex.cpp](C++多线程\mutex.cpp) 

## 单例

```c++
// 单例类
class MyCAS
{
private:
    MyCAS() {} //私有化构造函数
private:
    static MyCAS *m_instance; //静态成员变量
public:
    static MyCAS *GetInstance()
    {
        if(m_instance == NULL)
        {
            m_instance = new MyCAS();
        }
        return m_instance;
    }
    void func()
    {
        cout<<"test"<<endl;
    }
};
//类静态变量初始化
MyCAS *MyCAS::m_instance = NULL;
    
int main()
{
    // 创建一个对象，返回该类指针
    MyCAS *pa = MyCAS::GetInstance();
    // 由于m_instance不为空，故返回m_instance指针
    MyCAS *pb = MyCAS::GetInstance();
}
```

构建完成，但未有delete操作

```c++
// 单例类
class MyCAS
{
private:
    MyCAS() {} //私有化构造函数
private:
    static MyCAS *m_instance; //静态成员变量
public:
    static MyCAS *GetInstance()
    {
        if(m_instance == NULL)
        {
            m_instance = new MyCAS();
            // 创建静态对象，程序退出时，会调用析构函数
            static CGarhuishou cl;
        }
        return m_instance;
    }
    class CGarhuishou // 类中套类，用来释放对象
    {
      public:
        ~CGarhuishou()
        {
            if(MyCAS::m_instance)
            {
                delete MyCAS::m_instance;
                MyCAS::m_instance = NULL;
            }
        }
    };
    void func()
    {
        cout<<"test"<<endl;
    }
};
//类静态变量初始化
MyCAS *MyCAS::m_instance = NULL;
    
int main()
{
    // 创建一个对象，返回该类指针
    MyCAS *pa = MyCAS::GetInstance();
    // 由于m_instance不为空，故返回m_instance指针
    MyCAS *pb = MyCAS::GetInstance();
}
```

在多线程中，容易出现被多次初始化情况，故加上互斥锁。但仅在初始化时就加锁，会导致效率很低，故改进提高效率。

```c++
std::mutex mymutex;
// 单例类
class MyCAS
{
private:
    MyCAS() {} //私有化构造函数
private:
    static MyCAS *m_instance; //静态成员变量
public:
    static MyCAS *GetInstance()
    {
        //改进，提供效率
        //如果if( m_instance!=NUL)条件成立,则肯定表示 m_instance已经被new过了
        // 如果if( m_instance==NUL),不代表 m_instance一定没被new
        //初始化完成后，就不会进入，也就不会再加锁，故提高了效率
        if(m_instance == NULL) //双重锁定（双重检查）
        {
            //加锁
        std::unique_lock<mutex> mylock(mymutex);//自动加锁
        if(m_instance == NULL)
        {
            m_instance = new MyCAS();
            // 创建静态对象，程序退出时，会调用析构函数
            static CGarhuishou cl;
        }
        }
        
        return m_instance;
    }
    class CGarhuishou // 类中套类，用来释放对象
    {
      public:
        ~CGarhuishou()
        {
            if(MyCAS::m_instance)
            {
                delete MyCAS::m_instance;
                MyCAS::m_instance = NULL;
            }
        }
    };
    void func()
    {
        cout<<"test"<<endl;
    }
};
//类静态变量初始化
MyCAS *MyCAS::m_instance = NULL;
void mythread()
 {
     cout<<"start"<<endl;
    MyCAS *pa = MyCAS::GetInstance(); //容易有问题
    cout<<"end"<<endl;
    return;
 }
int main()
{
    std::thread myobj1(mythread);
    std::thread myobj2(mythread);
    myobj1.join();
    myobj2.join();
    return 0;
}
```

## call_once()

保证函数只被调用一次，具备互斥量能力，效率上比互斥量高效，消耗资源少。

需要与一个标记结合使用，std::once_flag(一个结构)，通过这个标记决定对应的函数是否执行。调用成功后，这个标记被设置为已调用状态，就不会被再次调用了。

```c++
std::once_flag flag; //系统定义的标记
std::mutex mymutex;
// 单例类
class MyCAS
{
    static void CreateInstance() //只被调用一次
    {
        m_instance = new MyCAS();
        static CGarhuishou cl;
    }
private:
    MyCAS() {} //私有化构造函数
private:
    static MyCAS *m_instance; //静态成员变量
public:
    static MyCAS *GetInstance()
    {
        // 两个线程统计执行到这里，其中一个线程需要等第一个线程执行完毕
        std::call_once(flag,CreateInstance);
        return m_instance;
    }
    class CGarhuishou // 类中套类，用来释放对象
    {
      public:
        ~CGarhuishou()
        {
            if(MyCAS::m_instance)
            {
                delete MyCAS::m_instance;
                MyCAS::m_instance = NULL;
            }
        }
    };
    void func()
    {
        cout<<"test"<<endl;
    }
};
//类静态变量初始化
MyCAS *MyCAS::m_instance = NULL;
void mythread()
 {
     cout<<"start"<<endl;
    MyCAS *pa = MyCAS::GetInstance(); //容易有问题
    cout<<"end"<<endl;
    return;
 }
int main()
{
    std::thread myobj1(mythread);
    std::thread myobj2(mythread);
    myobj1.join();
    myobj2.join();
    return 0;
}
```

代码： [单例.cpp](C++多线程\单例.cpp) 

## condition_variable、 wait、 notify_one、 notify_all

条件变量std::condition_variable实际上是一个类，是一个和条件相关的一个类，就是等待一个条件达成。这个类需要和互斥量来配合工作，用的时候要生成这个类的对象。

```c++
class A{
public:
// 将收到的消息入到队列的线程
void inMsgRecvQueue()
{
    for(int i=0;i<100000;++i)
    {
        cout<<"inMsgRecvQueue(),插入一个元素"<<i<<endl;
        std::unique_lock<std::mutex> sbguard(mymutex1);
        msgRecvQueue.push_back(i); //假设数字就是收到的命令，推进消息队列里。
        my_cond.notify_one(); //尝试把wait()唤醒，执行该语句后，就会被唤醒
        // .......
        //
    }
    return;
}

// 把消息从队列中取出
void outMsgRecvQueue()
{
    int command = 0;
    while(true)
    {
        std::unique_lock<std::mutex> sbguard(my_mutex);
        // wait用来等一个东西
        // 如果第二个参数lambda表达式返回值是false，那么wait将解锁互斥量，并堵塞到本行
        // 堵到其他某个线程调用notify_one()成员函数为止
        // 如果wait()没有第二个参数，那么就跟返回false一样。
        // 当其他线程用notify_one()将本wait()唤醒，
        // a. wait()不断尝试重新获取互斥量锁，如果获取不到，就会卡在这里等待获取;如果获取到，就继续执行b
        // b. 
            // b.1 如果wait()有第二个参数，就判断这个lambda表达式，如果表达式为false，那么wait将解锁互斥量，并堵塞到本行
            // 等待再次唤醒
            // b.2 如果为true，则wait()返回，执行下一句流程，此时互斥锁加锁
            // 如果wiat()没有第二个参数，则wait()返回，执行下一句流程
        my_cond.wait(sbguard1,[this]{
            if(!msgRecvQueue.empty())
                return true;
            return false;
        })；

        command = msgRecvQueue.front(); //返回第一个元素，但不检查元素是否存在
        msgRecvQueue.popfront();    // 移除第一个元素，但不返回
        sbguard.unlock();

        cout<<"outMagRecvQueue()执行,取出一个元素"<<endl;

    }
}
private:
    std::list<int> msgRecvQueue; //容器（消息队列），代表玩家发送的消息
    std::mutex my_mutex; //互斥量
    std::condition_variable my_cond;//生成一个条件变量对象
};
```

wait()用来等待一个东西，notify_one()通知一个线程，唤醒一个wait()

如果第二个参数返回值是false，那么wait()将解锁互斥量，并阻塞至本行，堵塞到其他线程调用notify_one()成员函数为止。如果是true，直接返回。

如果没有第二个参数，那么就跟返回false一样。

当其他线程用notify_one()将本wait()唤醒，

+ a. wait()不断尝试重新获取互斥量锁，如果获取不到，就会卡在这里等待获取;如果获取到，就继续执行b
+ b.
  + b.1 如果wait()有第二个参数，就判断这个lambda表达式，如果表达式为false，那么wait将解锁互斥量，并堵塞到本行，等待再次唤醒
  + b.2 如果为true，则wait()返回，执行下一句流程，此时互斥锁加锁；如果wiat()没有第二个参数，则wait()返回，执行下一句流程

**notify_all()**

同时唤醒多个线程

## async、future、packaged_task、promise

### std::async、std::future

std::async,std::future创建后台任务并返回值

std::async 是函数模板，用来启动异步任务，启动一个异步任务后，返回std::future对象

异步任务：自动创建一个线程并开始执行对应的线程入口函数，返回future对象，即含有线程入口函数所返回的结果（线程返回的结果）、可以通过调用future对象的成员函数get()来获取结果

std::future提供了一种访问异步操作结果的机制，通过get()成员函数等待线程执行结束并返回结果（只能调用一次,因为get()是移动语义）

```c++
int mythread()
{
	cout << "mythread() start" << " thread id = " << this_thread::get_id() << endl;
	chrono::milliseconds dura(5000); // 5s
	this_thread::sleep_for(dura); // 休息一定时间
	cout << "mythread() end" << " thread id = " << this_thread::get_id() << endl;

	return 5;
}

int main()
{
	cout << "main " << "thread id = " << this_thread::get_id() << endl;
	future<int> result = async(mythread); // 创建一个线程并开始执行
	cout << "continue........." << endl;
	// 通过get()成员函数等待线程执行结束并返回结果，只能调用一次
	cout << result.get() << endl; // 卡在这里等待mythread()执行完毕，拿到结果
	cout << "+++++++++++++" << endl;

	return 0;
}
```

类对象线程：

```c++
class A
{
public:
	int mythread(int mypar)
	{
		cout << "mypar " << mypar << endl;
		cout << "mythread() start" << " thread id = " << this_thread::get_id() << endl;
		chrono::milliseconds dura(5000); // 5s
		this_thread::sleep_for(dura); // 休息一定时间
		cout << "mythread() end" << " thread id = " << this_thread::get_id() << endl;
		return 5;
	}
};
int main()
{
	A a;
	int tmp = 12;
	cout << "main " << "thread id = " << this_thread::get_id() << endl;
	// 第二个参数为对象引用，传实参而不是形参
	future<int> result = async(&A::mythread, &a, tmp);
	cout << "continue........." << endl;
	cout << result.get() << endl; // 卡在这里等待mythread()执行完毕，拿到结果
	cout << "+++++++++++++" << endl;
	return 0;
}
```

通过额外向std::async()传递一个参数，类型是std::launch枚举类型，来达到一些特殊目的

1. launch::defered：延迟调用，表示线程入口函数调用被延迟到std::future的wait()或get()函数调用时才执行。

   ```c++
   future<int> result = async(launch::deferred,&A::mythread.&a,tmp);
   ```

   如果get()或wait()没有调用，线程不会创建。
   
   如果调用，延迟调用，并没有创建新线程，而是在主线程中调用的线程入口函数
   
2. launch::async：在调用async函数时开始创建线程。

std::async的默认参数是`launch::async | launch::deferred`，也就是系统会自行决定异步（创建新线程）还是同步（不创建新线程）方式运行。

**thread和async区别**

thread 如果系统资源紧张，创建线程失败，程序就会异常崩溃。如果线程返回值，不容易拿到值。

async 可能创建也可能不创建线程。如果系统资源紧张无法创建新线程，就不会创建新线程，而是在调用了get()请求结果的线程中执行该异步任务。容易拿到线程入口函数的返回值。



### std::packaged_task

打包任务，把任务包装起来。类模板，模板参数为各种可调用对象。

```c++
//mythread通过packaged_task包装起来
// 函数接口为int mythread(int i)，故<int(int)>
std::packaged_task<int(int)> mypt(mythread);
// 线程直接开始执行，第二个参数作为线程入口函数的参数
thread t1(std::ref(mypt), 1);
t1.join();
// result保存mythread返回的结果
future<int> result = mypt.get_future();

cout << result.get() << endl; 
```

lambda形式

```c++
packaged_task<int(int)> mypt([](int i)
		{
			cout << "i " << i << endl;
			cout << "mythread() start" << " thread id = " << this_thread::get_id() << endl;
			chrono::milliseconds dura(5000); // 5s
			this_thread::sleep_for(dura); // 休息一定时间
			cout << "mythread() end" << " thread id = " << this_thread::get_id() << endl;
			return i;
		});
	thread t1(ref(mypt), 1);
	t1.join();
	future<int> result = mypt.get_future();
	cout << result.get() << endl;

```

packaged_task包装起来的可调用对象还可以直接调用，所以packaged_task对象也是一个可调用对象。

相当于函数调用

```c++
packaged_task<int(int)> mypt([](int i)
		{
			cout << "i " << i << endl;
			cout << "mythread() start" << " thread id = " << this_thread::get_id() << endl;
			chrono::milliseconds dura(5000); // 5s
			this_thread::sleep_for(dura); // 休息一定时间
			cout << "mythread() end" << " thread id = " << this_thread::get_id() << endl;
			return i;
		});
mypt(100);
future<int> result = mypt.get_future();
cout << result.get() << endl;
cout << "+++++++++++++" << endl;
```

容器

```c++
vector<packaged_task<int(int)>> mytask;
// 直接push会报错，需使用移动语义
//mytask.push_back(mypt); //error
mytask.push_back(std::move(mypt));//push后，mypt就为空

packaged_task<int(int)> mypt2;
auto iter = mytask.begin();
mypt2 = std::move(*iter);
mytask.erase(iter); //删除第一个元素，迭代器已失效，后续代码不能再使用iter
mypt2(123);
//.....
```

### std::promise

类模板，能够在某个线程中给它赋值，然后可以在其他线程中，把值取去使用。

```c++
void mythread1(std::promise<int>& tmp, int calc)
{
	// 一系列复杂运算
	calc++;
	chrono::milliseconds dura(5000); // 5s
	this_thread::sleep_for(dura); // 休息一定时间
	int result = calc;
	tmp.set_value(result); //把结果保存到tmp对象中
	return;
}

int main()
{
	promise<int> myprom;
	thread t(mythread1, ref(myprom), 180);
	t.join();
	future<int> result = myprom.get_future();
	cout << result.get() << endl;
	return 0;
}
```

在另一个线程获取值

```c++
void mythread1(std::promise<int>& tmp, int calc)
{
	// 一系列复杂运算
	calc++;
	chrono::milliseconds dura(5000); // 5s
	this_thread::sleep_for(dura); // 休息一定时间
	int result = calc;
	tmp.set_value(result); //把结果保存到tmp对象中
	return;
}

void mythread2(future<int>& tmp)
{
	auto result = tmp.get();
	cout << "mythread2 result " << result << endl;
}

int main()
{
	promise<int> myprom;
	thread t(mythread1, ref(myprom), 180);
	t.join();
	future<int> result = myprom.get_future();
	//cout << result.get() << endl;
	thread t2(mythread2, std::ref(result));
	t2.join();
	return 0;
}
```

### std::future其他成员

wait_for()等待一定时间

```c++
future<int> result = async(mythread); // 创建一个线程并开始执行
	cout << "continue........." << endl;
	// future_status 枚举类型
	// wait_for等待一定时间
future_status status = result.wait_for(chrono::seconds(6)); //等待6s
future_status status = result.wait_for(6s); //wait_for()可直接传入时间，1s,2ms,3min均可
```

future_status枚举类型

```c++
future_status status = result.wait_for(chrono::seconds(1)); //等待1s
// 等待1s钟，如果没有返回，则status = timeout
if (status == future_status::timeout)
{
	// 超时：表示线程还未执行完成
	cout << "超时，线程还未执行完毕" << endl;
}
-----------------------------------------------------------------
// future_status status = result.wait_for(chrono::seconds(6)); //等待6s
// 等待6s后，线程返回则status = ready
else if (status == future_status::ready)
{
	cout << "线程成功返回" << endl;
	cout << result.get() << endl;
}
----------------------------------------------------------------
// future<int> result = async(launch::deferred,mythread); // 创建一个线程并延迟执行
else if (status == future_status::deferred)
{
	// 如果async参数被设置为launch::deferred
	cout << "线程被延迟执行" << endl;
	cout << result.get() << endl;
}
```

#### std::shared_future

shared_future的get()函数是复制数据

```c++
int mythread(int mypar)
{
	cout << "mythread() start" << " thread id = " << this_thread::get_id() << endl;
	chrono::milliseconds dura(5000); // 5s
	this_thread::sleep_for(dura); // 休息一定时间
	cout << "mythread() end" << " thread id = " << this_thread::get_id() << endl;
	return mypar;
}
void mythread2(shared_future<int> &future_s)
{
	cout << "mythread()2 start" << " thread id = " << this_thread::get_id() << endl;
	auto result = future_s.get();
	cout << "thread2 result = " << result << endl;
	return;
}

int main()
{
	cout << "main " << "thread id = " << this_thread::get_id() << endl;
	std::packaged_task<int(int)> mypt(mythread);
	thread t1(std::ref(mypt), 1);
	t1.join();
	// result保存mythread返回的结果
	future<int> result = mypt.get_future();
	//shared_future<int> result_s(std::move(result));
	shared_future<int> result_s(result.share());
	thread t2(mythread2, std::ref(result_s));
	t2.join();

	return 0;
}
```

这两句等效，执行完毕后，result为空，result_s有值

```C++
shared_future<int> result_s(std::move(result));
shared_future<int> result_s(result.share());
```

valid()判断是否为有效值

```c++
// 若第一次有效，则为ture
bool canget = result.valid();
shared_future<int> result_s(result.share());
// 此时变成无效，false
canget = result.valid();
```

通过shared_future就可以使用多次get

```c++
shared_future<int> result_s(result.share());
// 多次get，因为是复制值
auto threadresult = result_s.get();
threadresult = result_s.get();
```

也可以直接使用shared_future构造

```c++
shared_future<int> result_s(mypt.get_future());
```



## std::atomic原子操作

使用mutex互斥量会导致效率变低。可以把原子操作理解为不需要用互斥量加锁（无锁）技术的多线程并发，或者原子操作是多线程中，程序执行片段不会被打断

原子操作一般针对一个变量，而不是代码段。一般是指“不可分割的操作”

```c++
atomic<int> mycount = 0;
mutex mymutex;
void mythread()
{
	for (int i = 0; i < 1000000; i++)
	{
		mycount++;//atomic对象，原子操作（不会被打断）
	}
	return;
}

int main()
{
	thread obj1(mythread);
	thread obj2(mythread);
	obj1.join();
	obj2.join();
	cout << "两个线程执行完毕，结果为 " << mycount << endl;
	return 0;
}
```

一般atomic原子操作，针对++,--,+=,&=,|=,^=是支持的，其他可能不支持

```c++
mycount++;//atomic对象，原子操作（不会被打断）
mycount += 1; //true
mycount = mycount + 1; //error
```











+++++++++

> 参考资料：[C++ 并发编程（从C++11到C++17](https://paul.pub/cpp-concurrency/)
