在学习原子操作后，少有使用过，遗忘了不少，并还是有很多不了解的地方。比如，最近突然冒出的问题：

`std::atomic` 在赋值、比较过程中，是原子操作吗？

为了验证结果，这里写了一个 demo 用来验证 `a == 1` 和 `a = 1` 过程中，是否也是原子操作。

```cpp
#include <atomic>

using namespace std;
int main() {
  atomic<int> a;

  if(a == 1)
  {
      a = 2;
  }
  else if(a.load() == 3)
  {
      a.store(4);
  }
  a.load();
}
```

汇编代码如下：

```
main:
        mov     eax, DWORD PTR [rsp-4]
        cmp     eax, 1
        je      .L4
        mov     eax, DWORD PTR [rsp-4]
        cmp     eax, 3
        je      .L5
.L3:
        mov     eax, DWORD PTR [rsp-4]
        mov     eax, 0
        ret
.L4:
        mov     eax, 2
        xchg    eax, DWORD PTR [rsp-4]
        jmp     .L3
.L5:
        mov     eax, 4
        xchg    eax, DWORD PTR [rsp-4]
        jmp     .L3
```

这样看，不是很直观，可以移步 godbolt 网站进行查看。

```html
https://godbolt.org/z/n4bs3e3Kn
```

由汇编结果得出结论：

1. `a == 1` 与 `a.load() == 2` 是一致的
2. `a = 2` 与 `a.store(2)`

然后，再查阅相关资料，从《C++ 标准库》一书中了解到，`store()` 和 `load()` 这些操作都保证是 atomic（不可切割的），同时，还提到：

> 对于 atomic 类型，你可以继续使用有用而寻常的操作，像是赋值、自动转换为整型、递增、递减等
>
> ```cpp
> std::atomic<bool> ab (false);
> ab = true;
> if (ab){
>  ...
> }
> 
> std::atomic<int> ai (0);
> int x = ai;
> ai = 10;
> ai++;
> ai-=17;
> ```



通过源码也可获知，重载了`operator=(const _Ty _Value)`运算符，调用了`store()`，实现了原子操作。同理，其他运算符`==`、`++`等，皆重载了对应的运算符。

```cpp
// MSVC atomic
_EXPORT_STD template <class _Ty>
struct atomic : _Choose_atomic_base_t<_Ty> { // atomic value
	_Ty operator=(const _Ty _Value) volatile noexcept {
        static_assert(_Deprecate_non_lock_free_volatile<_Ty>, "Never fails");
        this->store(_Value);
        return _Value;
    }

    _Ty operator=(const _Ty _Value) noexcept {
        this->store(_Value);
        return _Value;
    }
}；
```



那么，问题的答案，现在清楚了。

## 补充

这段话需要细细品味

>  `store ()`会对影响所及的内存区执行一个所谓 release 操作，确保此前所有内存操作不论是否为 atomic，在 store 发挥效用之前都变成“可被其他线程看见”
>
>  `load ()`会对影响所及的内存区执行一个所谓 acquire 操作，确保随后所有内存操作不论是否为 atomic，在 load 之后都变成“可被其他线程看见”

我的理解：“所影响的内存区”指这个原子变量的内存区以及所关联的内存区，“可被其他线程看见”相当于，在执行原子操作时，把这个过程隐藏了，其他线程看不见，那么其他线程就无法进行更改，执行完原子操作之后，再显示出来，这样其他线程能看见了，就可以进行读写操作了。
简而言之，就是`store()` 先对所在内存区进行 delete 清空操作，不用管之前的内存区是什么样子，因为已经清空了，然后再进行加锁的赋值操作，赋值完后解锁。`load()` 先对所在内存区进行加锁操作，获取到值后，解锁。

**需要注意的点**

用 val 赋值初始化原子变量时，不是原子操作。

```cpp
std::atomic<T> a = val;//这个不是原子操作
std::atomic<T> b (val);//原子操作
```

递增、递减其实是重载操作，调用的`fetch_add ()` 和`fetch_sub ()`并返回拷贝的值，`+=` 和`-=`等同于`fetch_add ()` 和`fetch_sub ()`