FastAPI 是一个用 Python 编写 Web API 的框架。它可以根据函数的类型注解解析和校验请求参数，并自动生成 OpenAPI 文档。

## 安装和运行

```bash
python -m pip install "fastapi[standard]"
```

最简 demo，保存为 `main.py`。

```python
from fastapi import FastAPI

app = FastAPI()

@app.get('/')
async def index():
    return {'message': 'hello world'}
```

在 `main.py` 所在目录运行开发服务器。

```bash
python -m uvicorn main:app --reload
```

- `GET http://127.0.0.1:8000/` 返回 `{"message":"hello world"}`。
- `/docs` 可以查看接口文档并直接发送请求。

- `main:app` 表示从 `main.py` 导入 `app`。如果入口在 `app/main.py`，可以从项目根目录运行 `python -m uvicorn app.main:app --reload`。
- `--reload` 会在代码修改后自动重启服务，适合本地开发。部署时去掉 `--reload`，如果需要让容器外部访问，可以加上 `--host 0.0.0.0`。

`fastapi dev main.py` 也是官方 CLI 支持的开发命令。它会从文件中查找应用对象，底层同样使用 Uvicorn。

## 路径参数和查询参数

在上面的 `main.py` 中追加一个接口。

```python
@app.get('/items/{item_id}')
async def get_item(item_id: int, detail: bool = False):
    return {'item_id': item_id, 'detail': detail}
```

访问 `/items/42?detail=true`，返回如下。

```json
{"item_id":42,"detail":true}
```

`item_id` 来自路径，`detail` 来自查询字符串。这里的类型注解决定如何转换参数。访问 `/items/abc` 时，`abc` 无法转换成整数，FastAPI 会返回 HTTP 422。

## 请求体

用 Pydantic 的 `BaseModel` 描述 JSON 请求体。继续在 `main.py` 中追加下面的代码，并把导入放到文件顶部。

```python
from pydantic import BaseModel


class Item(BaseModel):
    name: str
    price: float


@app.post('/items', status_code=201)
async def create_item(item: Item):
    # 实际项目可以在这里把数据写入数据库
    return item
```

在 `/docs` 中向 `POST /items` 发送 `{"name":"键盘","price":199.5}`，接口返回 HTTP 201 和请求中的商品数据。

## lifespan 管理数据库连接

应用启动时可以建立数据库连接或连接池，供路由复用，应用关闭时再释放这些资源。FastAPI 用 `FastAPI(lifespan=lifespan)` 注册这段逻辑。


```python
@asynccontextmanager
async def lifespan(app: FastAPI):
    # Load the ML model
    ml_models["answer_to_everything"] = fake_answer_to_everything_ml_model
    yield
    # Clean up the ML models and release the resources
    ml_models.clear()

app = FastAPI(lifespan=lifespan)

@app.get("/predict")
async def predict(x: float):
    result = ml_models["answer_to_everything"](x)
    return {"result": result}
```


旧项目可能用 `@app.on_event('startup')` 和 `@app.on_event('shutdown')`。这组接口已经弃用。迁移时，把初始化放到 `yield` 前，把清理放到退出上下文时执行。资源没有异步上下文管理器时，可以用 `try/finally` 包住 `yield`，在 `finally` 中关闭资源。传入 `lifespan` 后，旧的事件处理器不会执行。

- [FastAPI CLI](https://fastapi.tiangolo.com/fastapi-cli/)
- [手动运行 ASGI 服务器](https://fastapi.tiangolo.com/deployment/manually/)
- [FastAPI 入门](https://fastapi.tiangolo.com/tutorial/first-steps/)
- [FastAPI 生命周期](https://fastapi.tiangolo.com/advanced/events/)
- [测试生命周期](https://fastapi.tiangolo.com/advanced/testing-events/)
- [aiosqlite API](https://aiosqlite.omnilib.dev/en/stable/api.html)
