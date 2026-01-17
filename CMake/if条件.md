if 条件命令

```cmake
if(<condition>)
  <commands>
elseif(<condition>) # optional block, can be repeated
  <commands>
else()              # optional block
  <commands>
endif()
```

## 基本表达式

```
if(<constant>)
```

`constant` 表示常量

| 逻辑值 | 常量值                                                       |
| ------ | ------------------------------------------------------------ |
| true   | 1、ON、YES、TRUE、Y、非0数字                                 |
| false  | 0、OFF、NO、FALSE、N、IGNORE、NOTFOUNT、空字符串、后缀-NOTFOUND结尾 |

命名的布尔常数是不区分大小写。

```
if(<variable>)
```

如果给定的变量被定义为一个非假常数的值，则为真。否则为假，包括该变量未定义。

注意，宏参数不是变量。环境变量也不能用这种方式测试，例如，if(ENV{some_var})将总是评估为假。

```
if(<string>)
```

一个带引号的字符串认为是false，除非字符串的值是真常数之一，或者 策略CMP0054没有设置为 "新"，并且字符串的值恰好是一个受CMP0054行为影响的变量名。

## 条件语法

以下语法适用于if、elseif和while()子句的条件参数。

复合条件按以下优先顺序进行评估：

1. 圆括号.
2. 一元测试： [EXISTS](https://cmake.org/cmake/help/latest/command/if.html#exists), [COMMAND](https://cmake.org/cmake/help/latest/command/if.html#command), and [DEFINED](https://cmake.org/cmake/help/latest/command/if.html#defined).
3. 二元测试： [EQUAL](https://cmake.org/cmake/help/latest/command/if.html#equal), [LESS](https://cmake.org/cmake/help/latest/command/if.html#less), [LESS_EQUAL](https://cmake.org/cmake/help/latest/command/if.html#less-equal), [GREATER](https://cmake.org/cmake/help/latest/command/if.html#greater), [GREATER_EQUAL](https://cmake.org/cmake/help/latest/command/if.html#greater-equal), [STREQUAL](https://cmake.org/cmake/help/latest/command/if.html#strequal), [STRLESS](https://cmake.org/cmake/help/latest/command/if.html#strless), [STRLESS_EQUAL](https://cmake.org/cmake/help/latest/command/if.html#strless-equal), [STRGREATER](https://cmake.org/cmake/help/latest/command/if.html#strgreater), [STRGREATER_EQUAL](https://cmake.org/cmake/help/latest/command/if.html#strgreater-equal), [VERSION_EQUAL](https://cmake.org/cmake/help/latest/command/if.html#version-equal), [VERSION_LESS](https://cmake.org/cmake/help/latest/command/if.html#version-less), [VERSION_LESS_EQUAL](https://cmake.org/cmake/help/latest/command/if.html#version-less-equal), [VERSION_GREATER](https://cmake.org/cmake/help/latest/command/if.html#version-greater), [VERSION_GREATER_EQUAL](https://cmake.org/cmake/help/latest/command/if.html#version-greater-equal), [PATH_EQUAL](https://cmake.org/cmake/help/latest/command/if.html#path-equal), and [MATCHES](https://cmake.org/cmake/help/latest/command/if.html#matches).
4. 一元逻辑运算符： [NOT](https://cmake.org/cmake/help/latest/command/if.html#not).
5. 二元逻辑运算符： [AND](https://cmake.org/cmake/help/latest/command/if.html#and) and [OR](https://cmake.org/cmake/help/latest/command/if.html#or)，从左到右，没有任何短路。

## 示例
```cmake
if (WIN32)
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")  
	message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode.") 
else() 
 	message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode.") 
endif() 

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
```