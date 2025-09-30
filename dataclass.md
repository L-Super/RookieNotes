`@dataclass` 是 Python 3.7+ 引入的装饰器，用于自动生成特殊方法，简化类的定义。

## 传统类 vs dataclass

**传统写法：**
```python
class Track:
    def __init__(self, track_id, title, artists, album, pic_id, lyric_id, source):
        self.track_id = track_id
        self.title = title
        self.artists = artists
        self.album = album
        self.pic_id = pic_id
        self.lyric_id = lyric_id
    
    def __repr__(self):
        return f"Track(track_id={self.track_id}, title={self.title}, ...)"
    
    # 还需要实现 __eq__, __hash__ 等方法...
```

**dataclass 写法：**
```python
from dataclasses import dataclass
from typing import List, Optional

@dataclass
class Track:
    track_id: str
    title: str
    artists: List[str]
    album: Optional[str]
    lyric_id: Optional[str]
```

## dataclass 自动生成的功能

1. **`__init__`** - 构造函数
2. **`__repr__`** - 可读的字符串表示
3. **`__eq__`** - 相等比较
4. **`__ne__`** - 不等比较

## 使用示例

```python
# 创建实例
track = Track(
    track_id="123",
    title="Example Song",
    artists=["Artist 1", "Artist 2"],
    album="Example Album",
    pic_id=None,
    lyric_id="lyric_456",
    source="netease"
)

print(track)
# 输出: Track(track_id='123', title='Example Song', artists=['Artist 1', 'Artist 2'], ...)

# 比较两个实例
track2 = Track("123", "Example Song", ["Artist 1", "Artist 2"], ...)
print(track == track2)  # True
```

## 额外功能

你可以添加默认值和其他配置：

```python
@dataclass
class Track:
    track_id: str
    title: str = "Unknown Title"  # 默认值
    artists: List[str] = None

```

dataclass 让代码更简洁、易读，特别适合用于主要存储数据的类。

> [dataclasses --- 数据类](https://docs.python.org/zh-cn/3.13/library/dataclasses.html)