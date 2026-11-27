`python-dotenv` 可以把 `.env` 文件里的配置加载到当前 Python 进程的环境变量中，之后就可以用 `os.getenv()` 读取。开发时可以把端口、接口地址等配置放在 `.env` 里。环境变量的读写见[设置环境变量](设置环境变量.md)。

## 安装

```bash
python -m pip install python-dotenv
```

安装包名是 `python-dotenv`，在代码里从 `dotenv` 导入。

## 基本用法

把 `.env` 和 `main.py` 放在同一个目录下。

```text
demo/
├── .env
└── main.py
```

在 `.env` 里每行写一个 `KEY=value`，用 `#` 写注释，用 `${变量名}` 引用前面定义的变量。

```dotenv
# 本地开发配置
DEMO_HOST=127.0.0.1
DEMO_PORT=8000
DEMO_DEBUG=true
DEMO_URL=http://${DEMO_HOST}:${DEMO_PORT}
```

在 `main.py` 里先加载配置，再读取环境变量。

```python
import os

from dotenv import load_dotenv

load_dotenv()

url = os.getenv('DEMO_URL')
port = int(os.getenv('DEMO_PORT', '8000'))
debug = os.getenv('DEMO_DEBUG', 'false').lower() == 'true'

print(url)
print(port, debug)
```

如果运行前没有设置这些同名环境变量，输出如下。

```text
http://127.0.0.1:8000
8000 True
```

- 从环境变量读取到的值是字符串，需要时用 `int()` 等方法转换。
- `bool('false')` 的结果也是 `True`，因为它是非空字符串。上面的例子通过比较字符串来读取开关。

## 指定配置文件

普通脚本中直接调用 `load_dotenv()` 时，会从调用代码所在目录向上查找 `.env`。如果项目里有多个配置文件，可以传入路径。

```python
from pathlib import Path

from dotenv import load_dotenv

env_path = Path(__file__).resolve().parent / '.env.local'
load_dotenv(env_path)
```

这里用 `__file__` 定位脚本目录，从其他目录启动脚本时也会读取同一个文件。

## 同名变量的优先级

`load_dotenv()` 默认保留已有的环境变量。例如启动程序时已经设置了 `DEMO_PORT=9000`，即使 `.env` 中更新为 `DEMO_PORT=8000`，程序读到的也是 `9000`。

需要让文件里的值覆盖已有环境变量时，传入 `override=True`。

```python
load_dotenv(override=True)
```

## 只读取配置文件

用 `dotenv_values()` 可以直接拿到配置字典，适合在程序里自行处理配置。调用时不会修改 `os.environ`。

```python
from dotenv import dotenv_values

config = dotenv_values('.env')
print(config.get('DEMO_PORT'))  # '8000'
```

`KEY=` 会解析成空字符串。只写 `KEY` 时，`dotenv_values()` 会得到 `None`，而 `load_dotenv()` 会跳过这一项。

## 提交配置模板

可以提交一份 `.env.example`，用示例值和占位符说明项目需要哪些配置。

```dotenv
DEMO_HOST=127.0.0.1
DEMO_PORT=8000
DEMO_DEBUG=false
DEMO_URL=http://${DEMO_HOST}:${DEMO_PORT}
DEMO_API_KEY=your-api-key-here
```

[python-dotenv 官方文档](https://saurabh-kumar.com/python-dotenv/)
