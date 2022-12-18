[`pass`](https://docs.python.org/zh-cn/3/reference/simple_stmts.html#pass) 语句不执行任何操作。语法上需要一个语句，但程序不实际执行任何动作时，可以使用该语句。

例如：

```python
while True:
    pass  # Busy-wait for keyboard interrupt (Ctrl+C)
```

下面这段代码创建了一个最小的类：

```python
class MyEmptyClass:
    pass
```

[`pass`](https://docs.python.org/zh-cn/3/reference/simple_stmts.html#pass) 还可以用作函数或条件子句的占位符，让开发者聚焦更抽象的层次。此时，程序直接忽略 `pass`：

```python
def initlog(*args):
    pass   # Remember to implement this!
```