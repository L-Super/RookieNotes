Python中的数据结构只要是可迭代的（iterable），比如列表、集合等等，那么都可以通过下面这种方式遍历：

```Plain Text
for item in <iterable>:
    ...
```

## 字典遍历

**字典本身只有键是可迭代的**，如果我们要遍历它的值或者是键值对，就需要通过其内置的函数values()或者items()实现。其中，values()返回字典的值的集合，items()返回键值对的集合。

```Python
d = {'name': 'jason', 'dob': '2000-01-01', 'gender': 'male'}
for k in d: # 遍历字典的键
    print(k)
name
dob
gender

for v in d.values(): # 遍历字典的值
    print(v)
jason
2000-01-01
male

for k, v in d.items(): # 遍历字典的键值对
    print('key: {}, value: {}'.format(k, v))
key: name, value: jason
key: dob, value: 2000-01-01
key: gender, value: male
```

## 通过集合中的索引来遍历元素

我们通常通过range()这个函数，拿到索引，再去遍历访问集合中的元素。比如下面的代码，遍历一个列表中的元素，当索引小于5时，打印输出：

```python
l = [1, 2, 3, 4, 5, 6, 7]
for index in range(0, len(l)):
    if index < 5:
        print(l[index])

1
2
3
4
5
```


当我们同时需要索引和元素时，还有一种更简洁的方式，通过Python内置的函数enumerate()。用它来遍历集合，不仅返回每个元素，并且还返回其对应的索引

```Python
l = [1, 2, 3, 4, 5, 6, 7]
for index, item in enumerate(l):
    if index < 5:
        print(item)

1
2
3
4
5
```






