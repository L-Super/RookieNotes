对代码进行单元测试是几乎每个软件工程师都要完成的工作。本文以C++语言为基础，讲解如何进行单元测试并生成测试报告。

# 前言

测试是软件开发过程中一个必须的环节，测试确保软件的质量符合预期。

对于工程师自己来说，单元测试也是提升自信心的一种方式。

直接交付没有经过测试的代码是不太好的，因为这很可能会浪费整个团队的时间，在一些原本早期就可以发现的问题上。而单元测试，就是发现问题一个很重要的环节。

本文以C++语言为基础，讲解如何进行单元测试并生成测试报告。

在工具上，我们会使用下面这些：

-   GCC
-   CMake
-   Google Test
-   gcov
-   lcov

# 演示项目

为了方便本文的讲解，我专门编写了一个演示项目作为代码示例。

演示项目的源码可以在我的Github上获取：[paulQuei/gtest-and-coverage](https://github.com/paulQuei/gtest-and-coverage)。

你可以通过下面几条命令下载和运行这个项目：

```
git clone https://github.com/paulQuei/gtest-and-coverage.git
cd gtest-and-coverage
./make_all.sh
```

> 要运行这个项目，你的机器上必须先安装好前面提到的工具。如果没有，请阅读下文以了解如何安装它们。

> 如果你使用的是Mac系统，下文假设你的系统上已经安装了[brew](https://brew.sh/)包管理器。如果没有，请通过下面这条命令安装它：

```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

## 项目结构

演示项目的目录结构如下：

```
.
├── CMakeLists.txt
├── googletest-release-1.8.1.zip
├── include
│   └── utility.h
├── make_all.sh
├── src
│   └── utility.cpp
└── test
    └── unit_test.cpp
```

这里演示的内容是：以测试一个我们要提供的软件库为例，讲解如何对其进行单元测试并生成测试报告。

为了简单起见，这个软件库只有一个头文件和一个实现文件。

> 当然，在实际上的项目中，一个软件库会通常包含更多的文件，不过这并不影响我们要说明的问题。

演示项目中的文件说明如下：

文件名称

说明

make_all.sh

入口文件，会执行：编译，测试和生成报告等所有工作

CMakeLists.txt

项目的编译文件

googletest-release-1.8.1.zip

google test源码压缩包

utility.h

待测试的软件库的头文件

utility.cpp

待测试的软件库的实现文件

unit_test.cpp

对软件库进行单元测试的代码

## 测试环境

演示项目在如下的环境中测试过。

-   MacBook Pro
    -   操作系统：macOS Mojave 10.14.1
    -   编译器：Apple LLVM version 10.0.0 （clang-1000.11.45.2）
    -   CMake：cmake version 3.12.1
    -   Google Test: 1.8.1
    -   lcov: lcov version 1.13
-   Ubuntu
    -   操作系统：Ubuntu 16.04.5 LTS
    -   编译器：gcc （Ubuntu 5.4.0-6ubuntu1~16.04.10） 5.4.0 20160609
    -   CMake：cmake version 3.5.1
    -   Google Test：1.8.1
    -   lcov：lcov version 1.12

# 关于[CMake 案例](CMake%20案例.md)

为了简化编译的过程，这里使用CMake作为编译工具。关于CMake的更多内容请参见请官网：[https://cmake.org](https://cmake.org/)。

关于如何安装CMake请参见这里：[Installing CMake](https://cmake.org/install/)。

另外，你也可以通过一条简单的命令来安装CMake：

-   Mac系统：

```
brew install cmake
```

-   Ubuntu系统

```
sudo apt install cmake
```

由于篇幅所限，这里不打算对CMake做过多讲解，读者可以访问其官网或者在网络上搜寻其使用方法。

这里仅仅对演示项目中用到的内容做一下说明。演示项目中的`CMakeLists.txt`内容如下：

```
cmake_minimum_required(VERSION 2.8.11) ①
project(utility) ②

set(CMAKE_CXX_STANDARD 11) ③

set(GTEST googletest-release-1.8.1) ④
include_directories("./include" "${GTEST}/googletest/include/")
link_directories("build/gtest/googlemock/gtest/")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} --coverage") ⑤

add_library(${CMAKE_PROJECT_NAME}_lib src/utility.cpp) ⑥

add_executable(unit_test test/unit_test.cpp) ⑦
target_link_libraries(unit_test ${CMAKE_PROJECT_NAME}_lib gtest gtest_main pthread) ⑧
```

以编号为序，这段代码说明如下：

1.  设置使用的CMake最低版本号为2.8.11。
2.  指定项目的名称为”utility”，项目名称可以通过`${CMAKE_PROJECT_NAME}`进行引用。
3.  指定使用C++11。
4.  这里的三行是编译google test，并将其头文件路径和编译结果的库文件路径添加到环境中。因为后面在编译单元测试代码的时候需要用到。
5.  添加`--coverage`到编译器flag中，这个参数是很重要的，**因为这是生成代码覆盖率所必须的**。关于该编译参数的说明见这里：[Program Instrumentation Options](https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html#Instrumentation-Options)。
6.  编译我们的软件库，这里将生成`libutility_lib.a`库文件。
7.  编译单元测试的可执行文件。
8.  单元测试的可执行文件需要链接我们开发的软件库以及google test的库。另外，google test依赖了pthread，所以这个库也需要。

# 关于测试

软件测试有很多种分类方式。从测试的级别来说，可以大致分为：

-   单元测试
-   集成测试
-   系统测试

这其中，单元测试是最局部和具体的。它通常需要对代码中的每一个类和函数进行测试。

单元测试通常由开发者完成，需要针对代码逻辑进行测试。所以它是一种[白盒测试](https://en.wikipedia.org/wiki/White-box_testing)。

# 关于xUnit

xUnit是几种单元测试框架的总称。最早源于Smalltalk的单元测试框架SUnit，它是由[Kent Beck](https://en.wikipedia.org/wiki/Kent_Beck)开发的。

除此之外，还有针对Java语言的JUnit，针对R语言的RUnit。

在本文中，我们使用Google开发的xUnit框架：Google Test。

# Google Test介绍

Google Test的项目主页在Github上：[Github: Google Test](https://github.com/google/googletest)。

实际上，这个项目中同时包含了GoogleTest和GoogleMock两个工具，本文中我们只会讲解第一个。

Google Test支持的操作系统包含下面这些：

-   Linux
-   Mac OS X
-   Windows
-   Cygwin
-   MinGW
-   Windows Mobile
-   Symbian

目前有很多的项目都使用了Google Test，例如下面这些：

-   [Chromium projects](http://www.chromium.org/)
-   [LLVM](http://llvm.org/)
-   [Protocol Buffers](https://github.com/google/protobuf)
-   [OpenCV](http://opencv.org/)
-   [tiny-dnn](https://github.com/tiny-dnn/tiny-dnn)

# 编译Google Test

关于如何编译Google Test请参见这里：[Generic Build Instructions](https://github.com/google/googletest/tree/master/googletest)。

为了便于读者使用，我们在演示项目中包含了Google Test 1.8.1的源码压缩包。并且在CMake文件中，同时包含了Google Test的编译和使用配置工作。

如果使用演示项目，读者将不需要手动处理Google Test的编译和安装工作。

# 使用Google Test

## 演示项目代码说明

为了便于下文说明，演示项目中包含了几个简单的函数。

> 可以从这里下载源码以便查看其中的内容：[paulQuei/gtest-and-coverage](https://github.com/paulQuei/gtest-and-coverage)。

演示项目中的软件库包含一个头文件和一个实现文件。头文件内容如下：

```
// utility.h

#ifndef INCLUDE_UTILITY_
#define INCLUDE_UTILITY_

enum CalcType {
    ADD,
    MINUS,
    MULTIPLE,
    DIVIDE
};

class Utility {
public:
    int ArithmeticCalculation(CalcType op, int a, int b);

    double ArithmeticCalculation(CalcType op, double a, double b);

    bool IsLeapYear(int year);
};

#endif
```

这个头文件说明如下：

-   头文件包含了三个函数，前两个用来做`int`和`double`类型的四则运算。最后一个判断输入的年份是否是闰年。
-   当然，在实际的工程中，前两个函数合并实现为一个泛型函数更为合适。但这里之所以分成两个，是为了查看代码覆盖率所用。
-   关于[闰年](https://en.wikipedia.org/wiki/Leap_year)说明如下：
    -   能被4整除但不能被100整除的年份为普通闰年。
    -   能被100整除，也同时能被400整除的为世纪闰年。
    -   其他都不是闰年。
    -   例如：1997年不是闰年，2000年是闰年，2016年是闰年，2100不是闰年。

这三个函数的实现也不复杂：

```
// utility.cpp

#include "utility.h"

#include <iostream>
#include <limits>

using namespace std;

int Utility::ArithmeticCalculation(CalcType op, int a, int b) {
    if (op == ADD) {
        return a + b;
    } else if (op == MINUS) {
        return a - b;
    } else if (op == MULTIPLE) {
        return a * b;
    } else {
        if (b == 0) {
            cout << "CANNO Divided by 0" << endl;
            return std::numeric_limits<int>::max();
        }
        return a / b;
    }
}

double Utility::ArithmeticCalculation(CalcType op, double a, double b) {
    if (op == ADD) {
        return a + b;
    } else if (op == MINUS) {
        return a - b;
    } else if (op == MULTIPLE) {
        return a * b;
    } else {
        if (b == 0) {
            cout << "CANNO Divided by 0" << endl;
            return std::numeric_limits<double>::max();
        }
        return a / b;
    }
}

bool Utility::IsLeapYear(int year) {
    if (year % 100 == 0 && year % 400 == 0) {
        return true;
    }
    if (year % 100 != 0 && year % 4 == 0) {
        return true;
    }
    return false;
}
```

## 开始测试

接下来我们就要对上面这些代码进行测试了。

要使用Google Test进行测试，整个过程也非常的简单。只要进行下面三部：

1.  创建一个测试用的cpp文件
2.  为上面这个测试用的cpp文件编写Makefile（或者CMake文件）。同时链接：
    -   待测试的软件库
    -   `gtest`库
    -   `gtest_main`库
    -   `pthread`库（Google Test使用了这个库所以需要）
3.  编写测试代码，编译并运行测试的可执行程序。

并且，测试代码写起来也非常的简单，像下面这样：

```
#include "utility.h"

#include "gtest/gtest.h"

TEST(TestCalculationInt, ArithmeticCalculationInt) {
    Utility util;
    EXPECT_EQ(util.ArithmeticCalculation(ADD, 1, 1), 2);
    EXPECT_EQ(util.ArithmeticCalculation(MINUS, 2, 1), 1);
    EXPECT_EQ(util.ArithmeticCalculation(MULTIPLE, 3, 3), 9);
    EXPECT_EQ(util.ArithmeticCalculation(DIVIDE, 10, 2), 5);
    EXPECT_GT(util.ArithmeticCalculation(DIVIDE, 10, 0), 999999999);
}
```

是的，就是这么简单的几行代码，就对整数四则运算的函数进行了测试。

`TEST`后面所包含的内容称之为一条case，通常我们会为每个函数创建一个独立的case来进行测试。一个测试文件中可以包含很多条case。同时，一条case中会包含很多的判断（例如`EXPECT_EQ...`）。

> 注意：在做单元测试的时候，保证每条case是独立的，case之间没有前后依赖关系是非常重要的。

当然，测试代码中包含的判断的多少将影响测试结果的覆盖率。所以在编写每条case的时候，我们需要仔细思考待测试函数的可能性，有针对性的进行测试代码的编写。

这段代码应该很好理解，它分别进行了下面这些测试：

-   1 + 1 = 2
-   2 - 1 = 1
-   3 x 3 = 9
-   10 / 2 = 5
-   10 / 0 > 999999999

你可能会发现，这段代码里面甚至没有main函数。它也依然可以生成一个可执行文件。这就是我们链接`gtest_main`所起的作用。

在实际的测试过程中，你想判断的情况可能不止上面这么简单。下面我们来看看Google Test还能做哪些测试。

## 测试判断

Google Test对于结果的判断，有两种形式：

-   `ASSERT_*`：这类判断是Fatal的。一旦这个判断出错，则直接从测试函数中返回，不会再继续后面的测试。
-   `EXPECT_*`：这类判断是Nonfatal的。它的效果是，如果某个判断出错，则输出一个错误信息，但是接下来仍然会继续执行后面的测试。

可以进行的判断方法主要有下面这些：

### 布尔判断

Fatal

Nonfatal

说明

`ASSERT_TRUE(condition)`

`EXPECT_TRUE(condition)`

判断 condition 为 true

`ASSERT_FALSE(condition)`

`EXPECT_FALSE(condition)`

判断 condition 为 false

### 二进制判断

Fatal

Nonfatal

说明

`ASSERT_EQ(expected, actual)`

`EXPECT_EQ(expected, actual)`

判断两个数值相等

`ASSERT_NE(val1, val2)`

`EXPECT_NE(val1, val2)`

val1 != val2

`ASSERT_LT(val1, val2)`

`EXPECT_LT(val1, val2)`

val1 < val2

`ASSERT_LE(val1, val2)`

`EXPECT_LE(val1, val2)`

val1 <= val2

`ASSERT_GT(val1, val2)`

`EXPECT_GT(val1, val2)`

val1 > val2

`ASSERT_GE(val1, val2)`

`EXPECT_GE(val1, val2)`

val1 >= val2

说明：

-   EQ：EQual
-   NE：Not Equal
-   LT：Less Than
-   LE：Less Equal
-   GT：Greater Than
-   GE：Greater Equal

### 字符串判断

Fatal

Nonfatal

说明

`ASSERT_STREQ(expected, actual)`

`EXPECT_STREQ(expected, actual)`

两个C string相同

`ASSERT_STRNE(str1, str2)`

`EXPECT_STRNE(str1, str2)`

两个C string不相同

`ASSERT_STRCASEEQ(exp, act)`

`EXPECT_STRCASEEQ(exp, act)`

忽略大小写，两个C string相同

`ASSERT_STRCASENE(str1, str2)`

`EXPECT_STRCASENE(str1, str2)`

忽略大小写，两个C string不相同

### 浮点数判断

Fatal

Nonfatal

说明

`ASSERT_FLOAT_EQ(exp, act)`

`EXPECT_FLOAT_EQ(exp, act)`

两个float数值相等

`ASSERT_DOUBLE_EQ(exp, act)`

`EXPECT_DOUBLE_EQ(exp, act)`

两个double数值相等

`ASSERT_NEAR(val1, val2, abs_err)`

`EXPECT_NEAR(val1, val2, abs_err)`

val1和val2的差距不超过abs_err

### 异常判断

Fatal

Nonfatal

说明

`ASSERT_THROW(stmt, exc_type)`

`EXPECT_THROW(stmt, exc_type)`

stmt抛出了exc_type类型的异常

`ASSERT_ANY_THROW(stmt)`

`EXPECT_ANY_THROW(stmt)`

stmt抛出了任意类型的异常

`ASSERT_NO_THROW(stmt)`

`EXPECT_NO_THROW(stmt)`

stmt没有抛出异常

## Test Fixture

在某些情况下，我们可能希望多条测试case使用相同的测试数据。例如，我们的演示项目中，每条case都会需要创建`Utility`对象。

有些时候，我们要测试的对象可能很大，或者创建的过程非常的慢。这时，如果每条case反复创建这个对象就显得浪费资源和时间了。此时，我们可以使用Test Fixture来共享测试的对象。

要使用Test Fixture我们需要创建一个类继承自Google Test中的`::testing::Test`。

还记得我们前面说过，我们要尽可能的保证每条测试case是互相独立的。但是，当我们在多条case之间共享有状态的对象时，就可能出现问题。

例如，我们要测试的是一个队列数据结构。有的case会向队列中添加数据，有的case会从队列中删除数据。case执行的顺序不同，则会导致Queue中的数据不一样，这就可能会影响case的结果。

为了保证每条case是独立的，我们可以在每条case的执行前后分别完成准备工作和清理工作，例如，准备工作是向队列中添加三个数据，而清理工作是将队列置空。

这两项重复性的工作可以由`::testing::Test`类中的`Setup`和`TearDown`两个函数来完成。

> 我们演示用的Utility类是无状态的，所以不存在这个问题。因此，这里我们仅仅在`Setup`和`TearDown`两个函数中打印了一句日志。

使用Test Fixture后，我们的代码如下所示：

```
class UtilityTest : public ::testing::Test {

protected:

void SetUp() override {
    cout << "SetUp runs before each case." << endl;
}

void TearDown() override {
    cout << "TearDown runs after each case." << endl;
}

Utility util;

};
```

这段代码说明如下：

1.  `Setup`和`TearDown`两个函数标记了`override`以确认是重写父类中的方法，这是C++11新增的语法。
2.  我们的Utility类是无状态的，因此`Setup`和`TearDown`两个函数中我们仅仅打印日志以便确认。
3.  将`Utility util`设置为`protected`以便测试代码中可以访问。（从实现上来说，测试case的代码是从这个类继承的子类，当然，这个关系是由Google Test工具完成的）。

要使用这里定义的Test Fixture，测试case的代码需要将开头的`TEST`变更为`TEST_F`。

> 这里`_F`就是Fixture的意思。

使用`TEST_F`的case的代码结构如下：

```
TEST_F(TestCaseName, TestName) {
  ... test body ...
}
```

这里的`TestCaseName`必须是Test Fixture的类名。

所以我们的测试代码写起来是这样：

```
TEST_F(UtilityTest, ArithmeticCalculationDouble) {
    EXPECT_EQ(util.ArithmeticCalculation(ADD, 1.1, 1.1), 2.2);
}

TEST_F(UtilityTest, ArithmeticCalculationIsLeapYear) {
    EXPECT_FALSE(util.IsLeapYear(1997));
    EXPECT_TRUE(util.IsLeapYear(2000));
    EXPECT_TRUE(util.IsLeapYear(2016));
    EXPECT_FALSE(util.IsLeapYear(2100));
}
```

> 我们针对`ArithmeticCalculation`方法故意只进行了一种情况的测试。这是为了最终生成代码覆盖率所用。

## 运行测试

编写完单元测试之后，再执行编译工作便可以运行测试程序以查看测试结果了。

测试的结果像下面这样：

![](images/test_case.png)

如果测试中包含了失败的case，则会以红色的形式输出。同时，会看到失败的case所处的源码行数，这样可以很方便的知道哪一个测试失败了，像下面这样：

![](images/test_case_fail.png)

如果只想有选择性的跑部分case，可以通过`--gtest_filter`参数进行过滤，这个参数支持`*`通配符。

像下面这样：

```
$ ./build/unit_test --gtest_filter=*ArithmeticCalculationInt
Running main() from googletest/src/gtest_main.cc
Note: Google Test filter = *ArithmeticCalculationInt
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from TestCalculationInt
[ RUN      ] TestCalculationInt.ArithmeticCalculationInt
CANNO Divided by 0
[       OK ] TestCalculationInt.ArithmeticCalculationInt (0 ms)
[----------] 1 test from TestCalculationInt (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (0 ms total)
[  PASSED  ] 1 test.
```

> 如果想要更好的理解这些内容。请读者下载演示项目之后完成下面这些操作：
> 
> 1.  在`utility.h`和`utility.cpp`中添加一些新的函数。
>     
> 2.  在新添加的函数中故意包含一个bug。
>     
> 3.  为新添加的函数编写测试代码，并测试出函数中包含的bug。
>     

# 代码覆盖率

在进行单元测试之后，我们当然希望能够直观的看到我们的测试都覆盖了哪些代码。

理论上，如果我们能做到100%的覆盖我们的所有代码，则可以说我们的代码是没有Bug的。

> 但实际上，100%的覆盖率要比想象得困难。对于大型项目来说，能够达到80% ~ 90%的语句覆盖率就已经很不错了。

## 覆盖率的类型

先来看一下，当我们在说“覆盖率”的时候我们到底是指的什么。

实际上，代码覆盖率有下面几种类型：

-   **函数覆盖率**：描述有多少比例的函数经过了测试。
-   **语句覆盖率**：描述有多少比例的语句经过了测试。
-   **分支覆盖率**：描述有多少比例的分支（例如：`if-else`，`case`语句）经过了测试。
-   **条件覆盖率**：描述有多少比例的可能性经过了测试。

这其中，函数覆盖率最为简单，就不做说明了。

语句覆盖率是我们最常用的。因为它很直观的对应到我们写的每一行代码。

而分支覆盖率和条件覆盖率可能不太好理解，需要做一下说明。

以下面这个C语言函数为例：

```
int foo (int x, int y) {
    int z = 0;
    if ((x > 0) && (y > 0)) {
        z = x;
    }
    return z;
}
```

这个函数中包含了一个`if`语句，因此`if`语句成立或者不成立构成了两个分支。所以如果只测试了`if`成立或者不成立的其中之一，其分支覆盖率只有 `1/2 = 50%`。

而条件覆盖率需要考虑每种可能性的情况。

对于`if (a && b)`这样的语句，其一共有四种可能的情况：

1.  a = true, b = true
2.  a = true, b = false
3.  a = false, b = true
4.  a = false, b = false

> 请读者思考一下：对于三层`if`嵌套，每个`if`语句包含三个布尔变量的代码，如果要做到100%的条件覆盖率，一共要测试多少种情况。
> 
> 很显示，在编写代码的时候，尽可能的减少代码嵌套，并且简化逻辑运算是一项很好的习惯。
> 
> 便于测试的代码也是便于理解和维护的，反之则反。

有了这些概念之后，我们就可以看懂测试报告中的覆盖率了。

## gcov

[gcov](https://gcc.gnu.org/onlinedocs/gcc/gcov.html)是由GCC工具链提供的代码覆盖率生成工具。它可以很方便的和GCC编译器配合使用。

通常情况下，安装好GCC工具链，也就同时包含了`gcov`命令行工具。

> 对于代码覆盖率工具所做的工作，可以简单的理解为：标记一次运行过程中，哪些代码被执行过，哪些没有执行。
> 
> 因此，即便没有测试代码，直接运行编译产物也可以得到代码的覆盖率。只不过，通常情况下这样得到的覆盖率较低罢了。

### 使用

这里我们以另外一个简单的代码示例来说明gcov的使用。

这段代码如下：

```
// test.c

#include <stdio.h>

int main (void) {

  for (int i = 1; i < 10; i++) {
      if (i % 3 == 0)
        printf ("%d is divisible by 3\n", i);
      if (i % 11 == 0)
        printf ("%d is divisible by 11\n", i);
  }

  return 0;
}
```

这是一个仅仅包含了main函数的c语言代码，main函数的逻辑也很简单。

我们将这段代码保存到文件`test.c`。

要通过gcov生成代码覆盖率。需要在编译时，增加参数`--coverage`：

```
gcc --coverage test.c
```

> `--coverage`等同于编译参数`-fprofile-arcs -ftest-coverage`以及在链接时增加`-lgcov`。

此处的编译结果除了得到可执行文件`a.out`，还会得到一个`test.gcno`文件。该文件包含了代码与行号的信息，在生成覆盖率时会需要这个文件。

> 很显然，带`--coverage`编译参数得到的编译产物会比不带这个参数要包含更多的信息，因此编译产物会更大。所以这个参数只适合在需要生成代码覆盖率的时候才加上。对于正式发布的编译产物，不应该添加这个编译参数。

当我们执行上面编译出来的可执行文件`a.out`时，我们还会得到每个源码文件对应的`gcda`后缀的文件。由`test.gcno`和`test.gcda`这两个文件，便可以得到代码的覆盖率结果了。

> 关于这两个文件的说明请参见这里：[Brief description of gcov data files](https://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/gcov-Data-Files.html)

只需要通过gcov指定源文件的名称（不需要带后缀）：`gcov test`，便可以得到包含覆盖率的结果文件 `test.c.gcov`了。

回顾一下我们刚刚的操作内容：

```
$ gcc --coverage test.c
$ ll
total 72
-rwxr-xr-x  1 Paul  staff    26K 11 10 14:41 a.out
-rw-r--r--  1 Paul  staff   240B 11 10 14:41 test.c
-rw-r--r--  1 Paul  staff   720B 11 10 14:41 test.gcno
$ ./a.out 
3 is divisible by 3
6 is divisible by 3
9 is divisible by 3
$ ll
total 80
-rwxr-xr-x  1 Paul  staff    26K 11 10 14:41 a.out
-rw-r--r--  1 Paul  staff   240B 11 10 14:41 test.c
-rw-r--r--  1 Paul  staff   212B 11 10 14:42 test.gcda
-rw-r--r--  1 Paul  staff   720B 11 10 14:41 test.gcno
$ gcov test
File 'test.c'
Lines executed:85.71% of 7
test.c:creating 'test.c.gcov'

$ ll
total 88
-rwxr-xr-x  1 Paul  staff    26K 11 10 14:41 a.out
-rw-r--r--  1 Paul  staff   240B 11 10 14:41 test.c
-rw-r--r--  1 Paul  staff   623B 11 10 14:42 test.c.gcov
-rw-r--r--  1 Paul  staff   212B 11 10 14:42 test.gcda
-rw-r--r--  1 Paul  staff   720B 11 10 14:41 test.gcno
```

我们可以`cat test.c.gcov`一下，查看覆盖率的结果：

```
        -:    0:Source:test.c
        -:    0:Graph:test.gcno
        -:    0:Data:test.gcda
        -:    0:Runs:1
        -:    0:Programs:1
        -:    1:// test.c
        -:    2:
        -:    3:#include <stdio.h>
        -:    4:
        -:    5:int main (void) {
        -:    6:
       20:    7:  for (int i = 1; i < 10; i++) {
        9:    8:      if (i % 3 == 0)
        3:    9:        printf ("%d is divisible by 3\n", i);
        9:   10:      if (i % 11 == 0)
    #####:   11:        printf ("%d is divisible by 11\n", i);
        9:   12:  }
        -:   13:
        1:   14:  return 0;
        -:   15:}
```

这个结果应该还是很容易理解的，最左边一列描述了代码的覆盖情况：

-   `-`： 表示该行代码被覆盖了
-   整数： 表示被执行的次数
-   `#####`：表示该行没有被覆盖

## lcov

[gcov](https://gcc.gnu.org/onlinedocs/gcc/gcov.html)得到的结果是本文形式的。但很多时候，我们可能希望得到更加美观和便于浏览的结果。

此时就可以使用[lcov](http://ltp.sourceforge.net/coverage/lcov.php)了。

lcov是gcov工具的图形前端。它收集多个源文件的gcov数据，并生成描述覆盖率的HTML页面。生成的结果中会包含概述页面，以方便浏览。

lcov支持我们前面提到的所有四种覆盖率。

这个链接是lcov生成的报告样例：[lcov - code coverage report](http://ltp.sourceforge.net/coverage/lcov/output/index.html)。

### 安装

lcov并非包含在GCC中，因此需要单独安装。

-   Mac系统

```
brew install lcov
```

-   Ubuntu系统

```
sudo apt install lcov
```

### 使用

对于lcov的使用方法可以通过下面这条命令查询：

```
lcov --help
```

通过输出我们可以看到，这个命令的参数有简短（例如`-c`）和完整（例如`--capture`)两种形式，其作用是一样的。

这里主要关注的下面这几个参数：

-   `-c` 或者 `--capture` 指定从编译产物中收集覆盖率信息。
-   `-d DIR` 或者 `--directory DIR` 指定编译产物的路径。
-   `-e FILE PATTERN` 或者 `--extract FILE PATTERN` 从指定的文件中根据PATTERN过滤结果。
-   `-o FILENAME` 或者 `--output-file FILENAME` 指定覆盖率输出的文件名称。

另外还有需要说明的是：

-   lcov默认不会打开分支覆盖率，因此我们还需要增加这个参数来打开分支覆盖率的计算：`--rc lcov_branch_coverage=1`
-   lcov输出的仍然是一个中间产物，我们还需要通过lcov软件包提供的另外一个命令`genhtml`来生成最终需要的html格式的覆盖率报告文件。同样的，为了打开分支覆盖率的计算，我们也要为这个命令增加`--rc lcov_branch_coverage=1`参数

最后，`make_all.sh`脚本中包含的相关内容如下：

```
COVERAGE_FILE=coverage.info
REPORT_FOLDER=coverage_report
lcov --rc lcov_branch_coverage=1 -c -d build -o ${COVERAGE_FILE}_tmp
lcov --rc lcov_branch_coverage=1  -e ${COVERAGE_FILE}_tmp "*src*" -o ${COVERAGE_FILE}
genhtml --rc genhtml_branch_coverage=1 ${COVERAGE_FILE} -o ${REPORT_FOLDER}
```

这段代码从我们前面编译的结果中收集覆盖率结果，并将结果输出到`coverage.info_tmp`文件中。但是这里面会包含非项目源码的覆盖率（例如google test），所以我们又通过另外一条命令来指定”src”文件夹进行过滤。最后，通过`genhtml`得到html格式的报告。

可以通过浏览器查看覆盖率报告的结果，像下面这样：

![](images/report_overview.png)

从这个报告的首页，我们已经可以看到代码的语句覆盖率（Lines），函数覆盖率（Functions）以及分支覆盖率（Branches）。而对于条件覆盖率可以从详细页面中看到。如下图所示：

![](images/code_coverage.png)

在上面这张图中，我们可以看到哪些代码被覆盖了，哪些没有。而对于对于`if-else`之类的语句，也能很清楚的看到条件覆盖率的覆盖情况。例如，对于代码的27行，只覆盖了`if`成立时的情况，没有覆盖`if`不成立时的情况。

# 更进一步

本文中，我们已经完整的完成了从编写单元测试到覆盖率生成的整个过程。

但实际上，对于这项工作我们还可以做得更多一些。例如下面这两项工作：

## 使用Google Mock

Google Mock是Google Test的扩展，用于编写和使用C++ Mock类。

在面向对象的编程中，[Mock对象](https://en.wikipedia.org/wiki/Mock_object)是模拟对象，它们以预先设定的方式模仿真实对象的行为。程序员通常会创建一个Mock对象来测试某个其他对象的行为，这与汽车设计师使用碰撞测试假人来模拟人类在车辆碰撞中的动态行为的方式非常相似。

关于Google Mock的更多内容请参见：[Google Mock的文档](https://github.com/google/googletest/blob/master/googlemock/README.md)。

## 持续集成

对于演示项目的覆盖率报告是通过手动执行脚本文件生成的。

而在实际的项目中，可能同时有很多人在开发同一个项目，每一天项目中都会有很多次的代码提交。我们不可能每次手动的执行编译和生成覆盖率报告结果。这时就可以借助一些[持续集成](https://en.wikipedia.org/wiki/Continuous_integration)的工具，定时自动地完成项目的编译，测试和覆盖率报告结果的生成工作。

可以在持续集成工具中包含我们编写的脚本，然后将覆盖率报告的html结果发布到某个Web服务器上，最后再以邮件的形式将链接地址发送给大家。

这样就可以很方便的让整个团队看到所有模块的测试结果和覆盖率情况了。

完成了一整套这样的工作，可以非常好的提升整个项目的质量。

# 参考文献与推荐读物

-   [CMake Cookbook](https://www.amazon.com/CMake-Cookbook-Building-packaging-software-ebook/dp/B079TVFDP5/)
-   [Google Test](https://github.com/google/googletest)
-   [googletest Generic Build Instructions](https://github.com/google/googletest/blob/master/googletest/README.md)
-   [Googletest Primer](https://github.com/google/googletest/blob/master/googletest/docs/primer.md)
-   [A quick introduction to the Google C++ Testing Framework](https://www.ibm.com/developerworks/aix/library/au-googletestingframework.html)
-   [Google Test Quick Reference](https://qiangbo-workspace.oss-cn-shanghai.aliyuncs.com/2018-12-05-gtest-and-coverage/PlainGoogleQuickTestReferenceGuide1.pdf)
-   [Coverage testing with gcov](http://www.network-theory.co.uk/docs/gccintro/gccintro_81.html)
-   [Generating Code Coverage with Qt 5 and GCOV on Mac OS](https://medium.com/@kelvin_sp/generating-code-coverage-with-qt-5-and-gcov-on-mac-os-4999857f4676)
-   [Wikipedia: xUnit](https://en.wikipedia.org/wiki/XUnit)
-   [Wikipedia: Code Coverage](https://en.wikipedia.org/wiki/Code_coverage)
-   [lcov - the LTP GCOV extension](http://ltp.sourceforge.net/coverage/lcov.php)
-   [gcov—a Test Coverage Program](http://gcc.gnu.org/onlinedocs/gcc/Gcov.html)