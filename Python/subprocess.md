`subprocess` 用于创建和管理子进程。允许执行外部命令，并与这些命令进行交互。例如：执行系统命令、调用其他程序，并获取它们的输出或错误信息。

推荐使用 `run()`，而不是旧版 `call()`

```python
subprocess.run(args, *, stdin=None, input=None, stdout=None, stderr=None, capture_output=False, shell=False, cwd=None, timeout=None, check=False, encoding=None, errors=None, text=None, env=None, universal_newlines=None, **other_popen_kwargs)
```

运行 args 指令，等待指令完成,，然后返回一个 [`CompletedProcess`](https://docs.python.org/zh-cn/3.11/library/subprocess.html#subprocess.CompletedProcess) 实例。

常见参数：

- args：执行的命令，可以为列表，字符串（当 shell=True 时）
- stdin、stdout 和 stderr：子进程的标准输入、输出和错误
- capture_output： 为 True 时，则 stdout 和 stderr 将被捕获
- timeout：设置以秒为单位的超时，达到超时限制，子进程将被杀掉并弹出 TimeoutExpired 异常
- check：如果为 True, 并且进程以非零状态码退出,  [`CalledProcessError`]( https://docs.python.org/zh-cn/3.11/library/subprocess.html#subprocess.CalledProcessError "subprocess. CalledProcessError") 异常将被抛出.
- text / encoding：字符串处理。text=True：以文本模式打开 stdout、stderr 和 stdin（等同于 universal_newlines=True）
-  shell：如果为 True，通过 shell 执行命令（⚠️ 警告：shell=True 存在安全风险，特别是处理用户输入时）
- cwd：工作目录
- env：环境变量

返回值：CompletedProcess 对象

CompletedProcess 参数：

- **args**：执行的命令
- **returncode**：退出状态码。0 表示运行正常。一个负值 `-N` 表示子进程被信号 `N` 中断 (仅 POSIX)
- **stdout**：标准输出，如果未捕获, 则为 `None`
- **stderr**：标准错误输出，如果未捕获, 则为 `None`
- **check_returncode**()：如果 `returncode` 非零, 抛出 [`CalledProcessError`](https://docs.python.org/zh-cn/3.11/library/subprocess.html#subprocess.CalledProcessError)

## 示例

```python
import subprocess

# 使用列表
subprocess.run(["ls", "-l"])
# shell为True时，字符串
subprocess.run('ls -l /tmp', shell=True)

# 获取输出
result = subprocess.run(['ls', '-l'], capture_output=True, text=True)
# 打印命令的输出
print(f"返回码: {result.returncode} 输出: {result.stdout}")

# 改变工作目录为tmp
subprocess.run(['ls'], cwd='/tmp')

# 不安全 - 可能被注入攻击
user_input = "file.txt; rm -rf /"
subprocess.run(f'cat {user_input}', shell=True)  # 危险！

try:
    subprocess.run(['false'], check=True)
except subprocess.CalledProcessError as e:
    print(f"命令失败，返回码: {e.returncode}")
```

异常处理：

```python
try:
    result = subprocess.run(
        ['git', 'status'],
        capture_output=True,
        text=True,
        check=True,
        timeout=10
    )
    print(result.stdout)
except subprocess.CalledProcessError as e:
    print(f"命令执行失败: {e.stderr}")
except subprocess.TimeoutExpired:
    print("命令超时")
except FileNotFoundError:
    print("命令不存在")
```

传递输入数据：

```python
result = subprocess.run(
    ['python', '-c', 'import sys; print(sys.stdin.read().upper())'],
    input='hello world',
    text=True,
    capture_output=True
)
print(result.stdout)  # 'HELLO WORLD\n'
```

重定向到文件：

```python
with open('output.txt', 'w') as f:
    subprocess.run(['ls', '-l'], stdout=f)
```

环境变量：

```python
import os
my_env = os.environ.copy()
my_env['MY_VAR'] = 'value'
subprocess.run(['printenv', 'MY_VAR'], env=my_env)
```