

只需在 Python 循环中包裹你的迭代器，就能产生一个进度条。它可以和 for 循环、pandas dataframe的 apply 函数以及 Python 的 map 函数等等配合使用。

```python
import time
from tqdm import tqdm

for i in tqdm(range(1000)):
    # 进度条每0.1s前进一次
    time.sleep(0.1)
```

终端会输出进度：

```bash
3%|▎         | 28/1000 [00:02<01:39,  9.80it/s]
```

+ 3% 代表进度百分比
+ 28/1000 已完成项/总项 

+ 00:02 已用时间
+ 01:39 剩余时间
+ 9.80it/s 表示每秒9.8项



## 常见场景

### 文件下载

只需在 Python 循环中包裹你的迭代器，一行代码就能产生一个精美的进度条。