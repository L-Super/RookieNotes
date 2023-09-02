在 Linux 系统中编写 Python 脚本, 第一行需要添加 Shebang (也称 Hashbang)。Shebang 用于指示系统这个脚本需要使用哪个解释器来执行, 其语法如下:
```py
#!/usr/bin/env python3
```

- `#!` 是 Shebang 的标志, 必须作为脚本的第一行。
- `/usr/bin/env` 用于查找python解释器的路径。
- `python3` 指定使用python3解释器。
