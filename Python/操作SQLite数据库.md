导入 SQlite 库

```py
import sqlite3
```

然后就可以连接数据库文件了。

```python
connection = sqlite3.connect("test.sqlite")
```

会尝试连接当前目录下的 `test.sqlite` 的数据库。如果数据库文件不存在，会自动创建它。

> .db、.db3、.sqlite后缀都可表示SQLite数据库

刚创建的 `connection` 变量是数据库连接对象。为了能执行数据库 SQL 查询语句，还需要创建一个数据库指针对象：

```python
cursor = connection.cursor()
```

大部分操作都需要依靠这个 `cursor` 指针对象。

先来创建一个产品数据表：

```python
cursor.execute('CREATE TABLE products(name, price)')
```

往表中添加点数据：

```python
cur.execute("""
    INSERT INTO products(name, price) VALUES
    ('iPhone', 5000),
    ('MacBook', 10000)
    """)
```

在 SQL 语句中「硬编码」字段值不是什么好注意，以上代码更妥当的写法应该是这样：

```python
cursor.executemany('INSERT INTO products(name, price) VALUES(?, ?)', [
    ('iPhone', 5000),
    ('MacBook', 10000),
    ])
```

紧接着还需要调用一下数据库连接对象的 `commit` 方法，否则数据不会保存到数据库文件。

```py
connection.commit()
```

更新数据的操作基本类似：

```python
cursor.execute('UPDATE products SET name=? WHERE name=?', 
               ('iPhone 14', 'iPhone'))
connection.commit()
```

还有删除操作：

```python
cursor.execute('DELETE FROM products WHERE name=?',
               ('iPhone 14', ))
connection.commit()
```

查询数据库数据有两种方式。第一种是查询并获取单条记录：

```python
result = cursor.execute('SELECT * FROM products WHERE name=?', ('iPhone',))
print(result.fetchone())
```

第二种是查询并获取多条记录：

```python
result = cursor.execute('SELECT * FROM products')
for rs in result:
    print(rs)
```

以上查询默认会返回如下元组类型的结果：

```text
('iPhone', 5000)
('MacBook', 10000)
```

如果希望以字典的方式返回结果，可以自定义一下数据库连接对象的 `row_factory` 属性：

```python
connection.row_factory = lambda cursor, row : {
        key: value for key, value in zip(
            [col[0] for col in cursor.description], row)
        }
```

再次执行数据查询语句，结果会如下所示：

```text
{'name': 'iPhone', 'price': 5000}
{'name': 'MacBook', 'price': 10000}
```