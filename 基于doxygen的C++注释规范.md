# 基于doxygen的C++注释规范

+ @brief 简要说明
+ @retval 返回值 简要说明 - 对返回值的说明
+ @see 简要参考内容 - 参考其他函数信息
+ @deprecated 简要说明 - 函数过时说明，可能会在将来的版本中取消
+ @note 简要说明  - 函数注意事项

# 文件头

```C++
/*!
* @file Ctext.h
* @brief 概述 
* 
*详细概述 
* 
* @author 作者名字
* @version 版本号(maj.min，主版本.分版本格式) 
* @date 日期 
*/
```

# 命名空间

```php
/**
 * @brief 命名空间的简单概述 \n(换行)
 * 命名空间的详细概述
 */
namespace OST
 {
 }
```

# 类、结构体

```php
/**
 * @brief 类的简单概述 \n(换行)
 * 类的详细概述
 */
class Example
 {
 };
```

#  函数注释原则  

```c++
/** 
 * @brief 函数简要说明
 * @param index    参数1
 * @param t        参数2 @see CTest
 *
 * @return 返回说明
 *     -<em>false</em> fail
 *     -<em>true</em> succeed
 * @retval 1 测试成功
 * @retval -1 测试失败
 */
bool Test(int index, const CTest& t);
```

# 变量及枚举

变量

```c++
int m_a;     ///< 成员变量1m_a说明
double m_b; ///< 成员变量2m_b说明
 
/// @brief 成员变量m_c简要说明
///
/// 成员变量m_c的详细说明，这里可以对变量进行
///详细的说明和描述，具体方法和函数的标注是一样的
float m_c;
```

枚举

```c++
/// @brief xxx枚举变量的简要说明
///
/// xxx枚举变量的详细说明--枚举变量的详细说明和函数的详细说明
///的写法是一致的。每个枚举变量下可以进行单独说明
enum{
    em_1,///< 枚举值1的说明
    em_2,///< 枚举值2的说明
    em_3 ///< 枚举值3的说明
};
```

`\brief` 和 `@brief`区别不大，更多使用的是后者

> 参考链接：
>
> [C++标准注释原则 - 基于doxygen的C++注释](https://blog.csdn.net/czyt1988/article/details/8901191)