argparse 是 Python 内置的一个用于命令项选项与参数解析的模块



主要有三个步骤：

- 创建 ArgumentParser() 对象
- 调用 add_argument() 方法添加参数
- 使用 parse_args() 解析添加的参数



## add_argument() 方法

add_argument() 方法定义如何解析命令行参数：

```
ArgumentParser.add_argument(name or flags...[, action][, nargs][, const][, default][, type][, choices][, required][, help][, metavar][, dest])
```



- name or flags - 选项字符串的名字或者列表，例如 foo 或者 -f, --foo
- action - 命令行遇到参数时的动作，默认值是 store
  - store_const，表示赋值为const；
  - append，将遇到的值存储成列表，也就是如果参数重复则会保存多个值;
  - append_const，将参数规范中定义的一个值保存到一个列表；
  - count，存储遇到的次数；此外，也可以继承 argparse.Action 自定义参数解析；
- nargs - 应该读取的命令行参数个数，可以是具体的数字，或者是?号，当不指定值时对于 Positional argument 使用 default，对于 Optional argument 使用 const；或者是 * 号，表示 0 或多个参数；或者是 + 号表示 1 或多个参数
- const - action 和 nargs 所需要的常量值
- default - 不指定参数时的默认值
- type - 命令行参数应该被转换成的类型
- choices - 参数可允许的值的一个容器
- required - 可选参数是否可以省略 (仅针对可选参数)
- help - 参数的帮助信息，当指定为 `argparse.SUPPRESS` 时表示不显示该参数的帮助信息
- metavar - 在 usage 说明中的参数名称，对于必选参数默认就是参数名称，对于可选参数默认是全大写的参数名称
- dest - 解析后的参数名称，默认情况下，对于可选参数选取最长的名称，中划线转换为下划线



## 示例

```python
import argparse

parser = argparse.ArgumentParser(description='Search some files')
 
parser.add_argument(dest='filenames',metavar='filename', nargs='*')
 
parser.add_argument('-p', '--pat',metavar='pattern', required=True,
                    dest='patterns', action='append',
                    help='text pattern to search for')
 
parser.add_argument('-v', dest='verbose', action='store_true',
                    help='verbose mode')
 
parser.add_argument('-o', dest='outfile', action='store',
                    help='output file')
 
parser.add_argument('--speed', dest='speed', action='store',
                    choices={'slow','fast'}, default='slow',
                    help='search speed')
 
args = parser.parse_args()
 
# Output the collected arguments
print(args.filenames)
print(args.patterns)
print(args.verbose)
print(args.outfile)
print(args.speed)
```





> 
>
> [Python Cookbook 中文版第三版-13.3 解析命令行选项](https://www.bookstack.cn/books/python-cookbook-3rd)