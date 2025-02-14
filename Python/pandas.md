pandas 是一个快速、强大、灵活且易于使用的开源数据分析和操作工具。

## 基本概念
Pandas 主要引入了两种新的数据结构：**Series** 和 **DataFrame**。

- **Series**： 类似于一维数组或列表，是由一组数据以及与之相关的数据标签（索引）构成。Series 可以看作是 DataFrame 中的一列，也可以是单独存在的一维数据结构。
- **DataFrame**： 类似于一个二维表格，可以看作是由多个 Series 按列排列构成的表格，它既有行索引也有列索引，因此可以方便地进行行列选择、过滤、合并等操作。


## Series
![](../images/Pasted%20image%2020250214124204.png)
```python
import pandas as pd

# 创建一个Series对象，指定名称为'A'，值分别为1, 2, 3, 4
# 默认索引为0, 1, 2, 3
series = pd.Series([1, 2, 3, 4], name='A')

# 显示Series对象
print(series)

# 如果你想要显式地设置索引，可以这样做：
custom_index = [1, 2, 3, 4]  # 自定义索引
series_with_index = pd.Series([1, 2, 3, 4], index=custom_index, name='A')

# 显示带有自定义索引的Series对象
print(series_with_index)
```
## DataFrame

```python
pandas.DataFrame(data=None, index=None, columns=None, dtype=None, copy=False)
```

参数说明：

- `data`：数据部分，可以是字典、二维数组、Series、DataFrame 或其他可转换为 DataFrame 的对象。如果不提供此参数，则创建一个空的 DataFrame。
- `index`：行索引，用于标识每行数据。可以是列表、数组、索引对象等。如果不提供此参数，则创建一个默认的整数索引。
- `columns`：列索引，用于标识每列数据。可以是列表、数组、索引对象等。如果不提供此参数，则创建一个默认的整数索引。
- `dtype`：指定数据类型。可以是 NumPy 的数据类型，例如 `np.int64`、`np.float64` 等。如果不提供此参数，则根据数据自动推断数据类型。
- `copy`：是否复制数据。默认为 False，表示不复制数据。如果设置为 True，则复制输入的数据。

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