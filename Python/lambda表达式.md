```
lambda arguments: expression
```
- lambda 函数是匿名的，它们没有函数名称，只能通过赋值给变量或作为参数传递给其他函数来使用。
- lambda 函数通常只包含一行代码，这使得它们适用于编写简单的函数。

```python
x = lambda a : a + 10
print(x(5))

x = lambda a, b : a * b
print(x(5, 6))
```

与 `map` 函数一起使用：
```python
my_list = [1, 2, 3, 4, 5, 6]
new_list = list(map(lambda x: x*2, my_list))
print(new_list) # [2, 4, 6, 8, 10, 12]
```

与 `filter` 一起使用：
```python
my_list = [18, -3, 5, 0, -1, 12]
new_list = list(filter(lambda x: x > 0, my_list))
print(new_list) # [18, 5, 12]
```