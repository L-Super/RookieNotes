pandas 是一个快速、强大、灵活且易于使用的开源数据分析和操作工具。

## 基本概念
Pandas 主要引入了两种新的数据结构：**Series** 和 **DataFrame**。

- **Series**： 类似于一维数组或列表，是由一组数据以及与之相关的数据标签（索引）构成。Series 可以看作是 DataFrame 中的一列，也可以是单独存在的一维数据结构。
- **DataFrame**： 类似于一个二维表格，可以看作是由多个 Series 按列排列构成的表格，它既有行索引也有列索引，因此可以方便地进行行列选择、过滤、合并等操作。

DataFrame 由 Index、Key、Value 组成：
![](../images/Pasted%20image%2020250213232416.png)

**DataFrame 可视为由多个 Series 组成的数据结构：**
![](../images/Pasted%20image%2020250213232330.png)
两个 Series 对象相加得到一个 DataFrame 对象：
![](../images/Pasted%20image%2020250213232356.png)

```python
import pandas as pd

# 创建两个Series对象
series_apples = pd.Series([1, 3, 7, 4])
series_bananas = pd.Series([2, 6, 3, 5])

# 将两个Series对象相加，得到DataFrame，并指定列名
df = pd.DataFrame({ 'Apples': series_apples, 'Bananas': series_bananas })

# 显示DataFrame
print(df)
```
输出：
```bash
   Apples  Bananas
0       1        2
1       3        6
2       7        3
3       4        5
```
