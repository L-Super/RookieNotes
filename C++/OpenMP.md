OpenMP (Open Multi-Processing)是一套支持跨平台共享内存方式的多线程并发的编程API

OpenMP 是 C++编译器的一个扩展。它已经被 C+编译器支持很久了，它是一个可靠的跨平台的多线程使用方式。

OpenMP由三部分组成：

- 编译指令
- 运行时库程序
- 环境变量

指令语法：
```
#pragma omp directive-name  [clause[ [,] clause]...] new-line
```

常用指令：
## `#pragma omp parallel`
parallel指令用于定义一个并行执行的代码块。`parallel`指令后面的那个代码块将会并行地执行
## `pragma omp parallel for`
将 for 循环拆分给 N 个线程去执行，这样每个线程都只需要执行整个 for 循环的其中一部分，所以可以实现并行计算。
## 使用

GCC 使用 `-fopenmp` 选项开启
MSVC 在 VS 设置里开启

CMake 中
```cmake
add_executable(OpenMP_Demo main.cpp)  
  
find_package(OpenMP)  
if(OpenMP_CXX_FOUND) 
	message(STATUS "OPENMP FOUND")
    target_link_libraries(OpenMP_Demo PUBLIC OpenMP::OpenMP_CXX)  
endif()
```
查看编译器选项、头文件位置等信息
```cmake
include(CMakePrintHelpers)  
  
cmake_print_properties(  
        TARGETS  
        OpenMP::OpenMP_CXX  
        PROPERTIES  
        INTERFACE_COMPILE_OPTIONS  
        INTERFACE_INCLUDE_DIRECTORIES  
        INTERFACE_LINK_LIBRARIES  
)
```
MinGW 编译器输出：
```bash
"D:\Program Files\JetBrains\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe" -DCMAKE_BUILD_TYPE=Debug "-DCMAKE_MAKE_PROGRAM=D:/Program Files/JetBrains/CLion 2023.3.4/bin/ninja/win/x64/ninja.exe" -G Ninja -S F:\Code\CppProjects\OpenMP-Demo -B F:\Code\CppProjects\OpenMP-Demo\cmake-build-debug
-- The C compiler identification is GNU 11.2.0
-- The CXX compiler identification is GNU 11.2.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: D:/Qt/Tools/mingw1120_64/bin/gcc.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: D:/Qt/Tools/mingw1120_64/bin/c++.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found OpenMP_C: -fopenmp (found version "4.5") 
-- Found OpenMP_CXX: -fopenmp (found version "4.5") 
-- Found OpenMP: TRUE (found version "4.5")  
-- OPENMP FOUND
-- 
 Properties for TARGET OpenMP::OpenMP_CXX:
   OpenMP::OpenMP_CXX.INTERFACE_COMPILE_OPTIONS = "$<$<COMPILE_LANGUAGE:CXX>:SHELL:-fopenmp>"
   OpenMP::OpenMP_CXX.INTERFACE_INCLUDE_DIRECTORIES = <NOTFOUND>
   OpenMP::OpenMP_CXX.INTERFACE_LINK_LIBRARIES = "D:/Qt/Tools/mingw1120_64/lib/gcc/x86_64-w64-mingw32/11.2.0/libgomp.dll.a;D:/Qt/Tools/mingw1120_64/x86_64-w64-mingw32/lib/libmingwthrd.a;D:/Qt/Tools/mingw1120_64/x86_64-w64-mingw32/lib/libmingwthrd.a"

-- Configuring done (1.9s)
-- Generating done (0.0s)
-- Build files have been written to: F:/Code/CppProjects/OpenMP-Demo/cmake-build-debug

[已完成]

```
### 常用示例
开启 8 个线程
```cpp
#pragma omp parallel num_threads(8)  
    {  
        printf("Hello OpenMP! I'm thread id %d. Total threads is %d.\n", omp_get_thread_num(),omp_get_num_threads());  
    }
```
4 个线程执行 for 循环：
```cpp
#pragma omp parallel for num_threads(4)  
    for (int i = 0; i < 8; ++i)  
    {  
        printf("Hello OpenMP! I'm thread id %d. Total threads is %d. i: %d\n", omp_get_thread_num(),omp_get_num_threads(), i);  
    }
```
执行加法：
```cpp
#include <vector>  
  
int main()  
{  
    int size = 10 * 1024 * 1024;  
    std::vector<int> squares(size, 0);  
#pragma omp parallel for  
    for (int i = 0; i < size; ++i)  
    {  
        squares[i] = i * i;  
    }  
    return 0;  
}
```
 OpenMP 实现的角度来看，上面的代码等同于这样：
```cpp
#include <omp.h>
#include <vector>
#include <cstdint>

int main() 
{
    int size = 10 * 1024 * 1024;
    std::vector<int> squares(size, 0); 
#pragma omp parallel 
    { 
        int thread_id = omp_get_thread_num(); // 线程 ID, 范围从 0 到 N - 1
        int thread_nums = omp_get_num_threads(); // 线程的数量
        int first = thread_id * size / thread_nums;
        int last = (thread_id + 1) * size / thread_nums;
        for (int i = first; i < last; ++i)
        {
            squares[i] = i * i;
        }
    }
    return 0;
}
```


## 函数列表
```cpp
// 设置并行区域线程数
void omp_set_num_threads(int _Num_threads);

// 返回当前线程数目，在串行代码中调用将返回1
int omp_get_num_threads(void);

// 返回程序的最大可用线程数量
int omp_get_max_threads(void);

// 返回当前线程id，从0开始编号
int omp_get_thread_num(void);

// 返回程序可用的处理器数
int omp_get_num_procs(void);

// 启用或禁用可用线程数的动态调整(缺省情况下启用动态调整)。
void omp_set_dynamic(int _Dynamic_threads);

// 确定在程序中此处是否启用了动态线程调整。
int omp_get_dynamic(void);

// 确定线程是否在并行区域的动态范围内执行
int omp_in_parallel(void);

// 启用或禁用嵌套并行操作
void omp_set_nested(int _Nested);

// 确定在程序中此处是否启用了嵌套并行操作
int omp_get_nested(void);


// 初始化一个（嵌套）互斥锁
void omp_init_lock(omp_lock_t * _Lock);
void omp_init_nest_lock(omp_nest_lock_t * _Lock);

// 销毁一个（嵌套）互斥锁并释放内存
void omp_destroy_lock(omp_lock_t * _Lock);
void omp_destroy_nest_lock(omp_nest_lock_t * _Lock);

// 获得一个（嵌套）互斥锁
void omp_set_lock(omp_lock_t * _Lock);
void omp_set_nest_lock(omp_nest_lock_t * _Lock);

// 释放一个（嵌套）互斥锁
void omp_unset_lock(omp_lock_t * _Lock);
void omp_unset_nest_lock(omp_nest_lock_t * _Lock);

// 试图获得一个（嵌套）互斥锁，并在成功时放回真，失败时返回假
int omp_test_lock(omp_lock_t * _Lock);
int omp_test_nest_lock(omp_nest_lock_t * _Lock);

// 从过去的某一时刻经历的时间，一般用于成对出现，进行时间比较
double omp_get_wtime(void);

// 得到clock ticks的秒数
double omp_get_wtick(void);

```

> [https://www.openmp.org](https://www.openmp.org/)
> [Visual C++ 中的 OpenMP](https://learn.microsoft.com/zh-cn/cpp/parallel/openmp/openmp-in-visual-cpp?view=msvc-170)
> [OpenMP · Modern CMake (modern-cmake-cn.github.io)](https://modern-cmake-cn.github.io/Modern-CMake-zh_CN/chapters/packages/OpenMP.html)
> [第3章 检测外部库和程序 - 3.5 检测OpenMP的并行环境 - 《CMake菜谱（CMake Cookbook中文版）》](https://www.bookstack.cn/read/CMake-Cookbook/content-chapter3-3.5-chinese.md?wd=C%2b%2b%e5%b9%b6%e8%a1%8c)
> [OpenMP教程 | wjin (w-jin.github.io)](https://w-jin.github.io/tech/openmp/)
> [OpenMP入门教程（一） - Rogn - 博客园 (cnblogs.com)](https://www.cnblogs.com/lfri/p/10111315.html)
> [OpenMP 并行编程指南 | Senlin's Blog (senlinzhan.github.io)](https://senlinzhan.github.io/2017/06/25/openmp/)