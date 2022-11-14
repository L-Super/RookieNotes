# Pimpl 与 unique_ptr

## 问题

当使用`pimpl`手法搭配智能指针`unique_ptr`时，会出现一个容易忽略的问题

```cpp
//widget.h
class Widget {                     
public:
    Widget();
    …

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
```

```cpp
//widget.cpp
#include "widget.h"                 
#include "gadget.h"
#include <string>
#include <vector>

struct Widget::Impl {
    std::string name;
    std::vector<double> data;
    Gadget g1,g2,g3;
};

Widget::Widget()                    //根据条款21，通过std::make_unique
: pImpl(std::make_unique<Impl>())   //来创建std::unique_ptr
{}

```

以上的代码能编译，但是，最普通的`Widget`用法却会导致编译出错：

```cpp
#include "widget.h"

Widget w;                           //错误！
```

错误信息根据编译器不同会有所不同，但其文本一般会提到一些有关于“把`sizeof`或`delete`应用到未完成类型上”的信息。对于未完成类型，使用以上操作是禁止的。

## 原因

在对象`w`被析构时（例如离开了作用域），问题出现了。在这个时候，它的析构函数被调用。根据编译器自动生成的特殊成员函数的规则（见 [Item17](https://github.com/kelthuzadx/EffectiveModernCppChinese/blob/master/3.MovingToModernCpp/item17.md)），编译器会自动为我们生成一个析构函数。 在这个析构函数里，编译器会插入一些代码来调用类`Widget`的数据成员`pImpl`的析构函数。 `pImpl`是一个`std::unique_ptr<Widget::Impl>`，也就是说，一个使用默认删除器的`std::unique_ptr`。 默认删除器是一个函数，它使用`delete`来销毁内置于`std::unique_ptr`的原始指针。

然而，**在使用`delete`之前，通常会使默认删除器使用C++11的特性`static_assert`来确保原始指针指向的类型不是一个未完成类型。** **当编译器为`Widget w`的析构生成代码时，它会遇到`static_assert`检查并且失败，这通常是错误信息的来源。** 这些**错误信息只在对象`w`销毁的地方出现，因为类`Widget`的析构函数，正如其他的编译器生成的特殊成员函数一样，是暗含`inline`属性的。 错误信息自身往往指向对象`w`被创建的那行**，因为这行代码明确地构造了这个对象，导致了后面潜在的析构。

## 解决

**为了解决这个问题，你只需要确保在编译器生成销毁`std::unique_ptr<Widget::Impl>`的代码之前， `Widget::Impl`已经是一个完成类型（*complete type*）。** 当编译器“看到”它的定义的时候，该类型就成为完成类型了。 但是 `Widget::Impl`的定义在`widget.cpp`里。成功编译的关键，就是在`widget.cpp`文件内，让编译器在“看到” `Widget`的析构函数实现之前（也即编译器插入的，用来销毁`std::unique_ptr`这个数据成员的代码的，那个位置），先定义`Widget::Impl`。

**只需要先在`widget.h`里，只声明类`Widget`的析构函数，但不要在这里定义它**：

```cpp
class Widget {                  //跟之前一样，在“widget.h”中
public:
    Widget();
    ~Widget();                  //只有声明语句
    …

private:                        //跟之前一样
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
```

在`widget.cpp`文件中，结构体`Widget::Impl`被定义之后，再定义析构函数：

```cpp
#include "widget.h"                 //跟之前一样，在“widget.cpp”中
#include "gadget.h"
#include <string>
#include <vector>

struct Widget::Impl {               //跟之前一样，定义Widget::Impl
    std::string name;
    std::vector<double> data;
    Gadget g1,g2,g3;
}

Widget::Widget()                    //跟之前一样
: pImpl(std::make_unique<Impl>())
{}

Widget::~Widget()                   //析构函数的定义（译者注：这里高亮）
{}
```

这样就可以了，并且这样增加的代码也最少，但是，如果你想强调编译器生成的析构函数会做正确的事情——你声明`Widget`的析构函数的唯一原因是导致它的定义在 Widget 的实现文件中（译者注：指`widget.cpp`）生成，你可以使用“`= default`”定义析构函数体：

```cpp
Widget::~Widget() = default;        //同上述代码效果一致
```

## by the way

同理，对于移动操作也是一样的。

```cpp
class Widget {                                  //仍然在“widget.h”中
public:
    Widget();
    ~Widget();

    Widget(Widget&& rhs) = default;             //思路正确，
    Widget& operator=(Widget&& rhs) = default;  //但代码错误
    …

private:                                        //跟之前一样
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
```

这样的做法会导致同样的错误，和之前的声明一个不带析构函数的类的错误一样，并且是因为同样的原因。

 编译器生成的**移动赋值操作符**，在重新赋值之前，需要先销毁指针`pImpl`指向的对象。然而在`Widget`的头文件里，`pImpl`指针指向的是一个未完成类型。

**移动构造函数**的情况有所不同。 移动构造函数的问题是编译器自动生成的代码里，包含有抛出异常的事件，在这个事件里会生成销毁`pImpl`的代码。然而，销毁`pImpl`需要`Impl`是一个完成类型。

因为这个问题同上面一致，所以解决方案也一样——把移动操作的定义移动到实现文件里：

```cpp
//widget.h
class Widget {                          
public:
    Widget();
    ~Widget();

    Widget(Widget&& rhs);               //只有声明
    Widget& operator=(Widget&& rhs);
    …

private:                                //跟之前一样
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

//widget.cpp
#include <string>                   
…
    
struct Widget::Impl { … };          //跟之前一样

Widget::Widget()                    //跟之前一样
: pImpl(std::make_unique<Impl>())
{}

Widget::~Widget() = default;        //跟之前一样

Widget::Widget(Widget&& rhs) = default;             //这里定义
Widget& Widget::operator=(Widget&& rhs) = default;
```
