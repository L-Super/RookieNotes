`@dataclass` 是 Python 3.7 起引入在 `dataclasses` 模块里的**装饰器**，用于简化类的定义过程。只要写**类属性**就能**自动生成**所有“样板代码”：

- `__init__`
- `__repr__`
- `__eq__`（以及 `__lt__` 等排序方法）
- `__hash__`（可选）
- 可选的 `frozen`、`slots`、`kw_only` 等额外特性

示例：
```python
from dataclasses import dataclass

@dataclass
class Point:
    x: float
    y: float

p1 = Point(1.0, 2.0)
p2 = Point(1.0, 2.0)
print(p1)          # Point(x=1.0, y=2.0)
print(p1 == p2)    # True
```
可以为字段设置默认值，或者使用 _field_ 提供更复杂的默认值：
```python
from dataclasses import dataclass, field

@dataclass
class Person:
	name: str
	age: int = 30
	email: str = field(default="example@example.com")
	items: list = field(default_factory=list) # 可变类型（如列表），需要使用 _default_factory_ 来避免共享同一个对象
	_id: int = field(init=False, repr=False) # 不放进构造函数、不打印

p = Person("Alice")

print(p) # 输出: Person(name='Alice', age=30, email='example@example.com')
```

转换为字典或元组：
```python
from dataclasses import dataclass, asdict, astuple

@dataclass
class Point:
	x: int
	y: int

p = Point(1, 2)

print(asdict(p)) # 输出: {'x': 1, 'y': 2}
print(astuple(p)) # 输出: (1, 2)
```