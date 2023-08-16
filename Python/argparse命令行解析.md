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
- action - 命令行遇到参数时的动作，默认值是 store，被用来存储某个值或将多个参数值收集到一个列表中
  - store_const，表示赋值为const；
  - append，将遇到的值存储成列表，也就是如果参数重复则会保存多个值;
  - append_const，将参数规范中定义的一个值保存到一个列表；
  - count，存储遇到的次数；此外，也可以继承 argparse.Action 自定义参数解析；
- nargs - 应该读取的命令行参数个数，可以是具体的数字，或者是?号，当不指定值时对于 Positional argument 使用 default，对于 Optional argument 使用 const；或者是 * 号，表示 0 或多个参数；或者是 + 号表示 1 或多个参数
- const - action 和 nargs 所需要的常量值
- default - 不指定参数时的默认值
- type - 命令行参数应该被转换成的类型
- choices - 参数可允许的值的一个容器
- required - 表示该参数至少要有一个 (仅针对可选参数)
- help - 参数的帮助信息，当指定为 `argparse.SUPPRESS` 时表示不显示该参数的帮助信息
- metavar - 在 help 说明中的参数名称，对于必选参数默认就是参数名称，对于可选参数默认是全大写的参数名称
- dest - 解析后的参数名称，默认情况下，对于可选参数选取最长的名称，中划线转换为下划线

## 示例

```python
import argparse

parser = argparse.ArgumentParser(description='Search some files')
 
parser.add_argument(dest='filenames',metavar='filename', nargs='*')
# 允许某个参数重复出现多次，并将它们追加到一个列表中。required 表示该参数至少要有一个。-p 和 —pat 表示两个参数名形式都可使用
parser.add_argument('-p', '--pat',metavar='pattern', required=True,
                    dest='patterns', action='append',
                    help='text pattern to search for')
# 根据参数是否存在来设置一个 Boolean 标志 
parser.add_argument('-v', dest='verbose', action='store_true',
                    help='verbose mode')
# 接受一个单独值并将其存储为一个字符串
parser.add_argument('-o', dest='outfile', action='store',
                    help='output file')
# 接受一个值，但是会将其和可能的选择值做比较，以检测其合法性
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

输出：

```sh
$python3 search.py -h
usage: search.py [-h] [-p pattern] [-v] [-o OUTFILE] [--speed {slow,fast}]
                 [filename [filename ...]]
 
Search some files
 
positional arguments:
  filename
 
optional arguments:
  -h, --help            show this help message and exit
  -p pattern, --pat pattern
                        text pattern to search for
  -v                    verbose mode
  -o OUTFILE            output file
  --speed {slow,fast}   search speed
```

```sh
$python3 search.py foo.txt bar.txt
usage: search.py [-h] -p pattern [-v] [-o OUTFILE] [--speed {fast,slow}]
                 [filename [filename ...]]
search.py: error: the following arguments are required: -p/--pat
 
$python3 search.py -v -p spam --pat=eggs foo.txt bar.txt
filenames = ['foo.txt', 'bar.txt']
patterns  = ['spam', 'eggs']
verbose   = True
outfile   = None
speed     = slow
 
$python3 search.py -v -p spam --pat=eggs foo.txt bar.txt -o results
filenames = ['foo.txt', 'bar.txt']
patterns  = ['spam', 'eggs']
verbose   = True
outfile   = results
speed     = slow
 
$python3 search.py -v -p spam --pat=eggs foo.txt bar.txt -o results --speed=fast
filenames = ['foo.txt', 'bar.txt']
patterns  = ['spam', 'eggs']
verbose   = True
outfile   = results
speed     = fast
```


> [Python Cookbook 中文版第三版-13.3 解析命令行选项](https://www.bookstack.cn/books/python-cookbook-3rd)