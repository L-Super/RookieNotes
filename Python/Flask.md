Flask 是一个使用 Python 语言编写的 Web 框架，它可以让你高效地编写 Web。Flask 是典型的微框架，作为 Web 框架来说，它仅保留了核心功能：**请求响应处理**和**模板渲染**。这两类功能分别由 Werkzeug（WSGI 工具库）完成和 Jinja（模板渲染库）完成。

使用 PyCharm 创建工程时，可直接选择 Flask 工程，会创建 Flask 工程模板。

最简 demo：

```python
from flask import Flask

app = Flask(__name__)

@app.route('/')
def index():
    return 'hello world'

if __name__ == '__main__':
    app.run()
```

装饰器的参数`/`代表根目录，浏览器访问`127.0.0.1:5000`（5000 Flask默认端口）就会显示hello world。