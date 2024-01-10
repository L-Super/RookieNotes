lambda 表达式实现，其实是编译器为我们创建了一个类，这个类重载了 `()` 运算符，让我们可以像调用函数一样使用。所以，你写的 lambda 表达式，真正的实现是这个样子的： 

```cpp
// 用户代码
[](X& elem){ elem.op(); }

// 编译器生成的代码
class _SomeComplierGeneratedName_{
public:
    void operator() (X& elem) const
    {
        elem.op();
    }
};
```


![](../Go/images/image_thumb.png) 

而对于捕获变量的lambda表达式来说，编译器在创建类的时候，通过成员函数的形式保存了需要捕获的变量，所以看起来是这个样子： 

```cpp
// 用户代码
[&total, offset](X& elem){ total += elem.get1() + offset; }

// 编译器生成的代码
class _SomeComplierGeneratedName_{
public:
    _SomeComplierGeneratedName_(int& t, int o):total_(t),offset_(o) {}
    void operator() (X& elem) const
    {
        elem.op();
    }
private:
    int& total_;//引用捕获的上下文
    int offset_;//按值捕获的上下文
};

```


![](../Go/images/image_thumb4.png)

正是由于编译器帮我们实现了细节，使我们的代码变得优雅和简洁了许多。

> [C++11中的lambda，std::function以及std:bind ](https://paul.pub/cpp-lambda-function-bind/)

