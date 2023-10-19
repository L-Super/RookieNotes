

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

## 参数介绍

```python
class tqdm():
  """
  Decorate an iterable object, returning an iterator which acts exactly
  like the original iterable, but prints a dynamically updating
  progressbar every time a value is requested.
  """


    
    # override defaults via env vars
    @envwrap("TQDM_", is_method=True, types={'total': float, 'ncols': int, 'miniters': float, 'position': int, 'nrows': int})
    def __init__(self, iterable=None, desc=None, total=None, leave=True, file=None,
                 ncols=None, mininterval=0.1, maxinterval=10.0, miniters=None,
                 ascii=None, disable=False, unit='it', unit_scale=False,
                 dynamic_ncols=False, smoothing=0.3, bar_format=None, initial=0,
                 position=None, postfix=None, unit_divisor=1000, write_bytes=False,
                 lock_args=None, nrows=None, colour=None, delay=0.0, gui=False,
                 **kwargs):
```


    ----------
Parameters

+ iterable  : iterable, optional

  可迭代的对象, 在手动更新时不需要进行设置

+ desc  : str, optional
  进度条的描述文字

+ total  : int or float, optional
  预期迭代次数。如果未指定，则在可能的情况下使用 len（可迭代）。作为最后手段，则只显示基本进度统计信息（无 ETA，无进度条）。如果 `gui` 为 True，并且此参数需要后续更新，请指定一个初始的任意大的正数。

+ leave  : bool, optional
  如果 True（默认为 True） ，则在迭代终止时保留进度条。

+ file  : `io.TextIOWrapper` or `io.StringIO`, optional
  指定输出进度消息的位置。默认是终端（sys. stderr）。使用 `file.write (str)` 和 `file.rush ()` 方法。


+ ncols  : int, optional
  调整进度条宽度, 默认是根据环境自动调节长度, 如果设置为0, 就没有进度条, 只有输出的信息

+ mininterval  : float, optional
  最短进度显示更新间隔。默认值：0.1秒。

+ maxinterval  : float, optional
  最大进度显示更新间隔。默认值: 10秒。在长时间显示更新延迟之后，自动调整 `miniters` 以对应 `mininterval`。只有在启用 ` dynamic_miniters ` 或 monitor 线程时才能工作。

+ miniters  : int or float, optional
   最小进度显示更新间隔（以迭代为单位）。

+ ascii  : bool or str, optional
     If unspecified or False, use unicode (smooth blocks) to fill the meter. The fallback is to use ASCII characters " 123456789#".

+ disable  : bool, optional
     是否禁用整个 progressbar 包装器。 默认值：false。If set to None, disable on non-TTY.
      
+ unit  : str, optional
     将用于定义每个迭代的单元的字符串。默认：it。
      
+ unit_scale  : bool or int or float, optional
          If 1 or True, the number of iterations will be reduced/scaled
          automatically and a metric prefix following the
          International System of Units standard will be added
          (kilo, mega, etc.) [default: False]. If any other non-zero
          number, will scale `total` and `n`.
      
+ dynamic_ncols  : bool, optional
          If set, constantly alters `ncols` and `nrows` to the
          environment (allowing for window resizes) [default: False].
      
+ smoothing  : float, optional
          Exponential moving average smoothing factor for speed estimates
          (ignored in GUI mode). Ranges from 0 (average speed) to 1
          (current/instantaneous speed) [default: 0.3].
      
+ bar_format  : str, optional
          Specify a custom bar string formatting. May impact performance.
          [default: '{l_bar}{bar}{r_bar}'], where
          l_bar='{desc}: {percentage:3.0f}%|' and
          r_bar='| {n_fmt}/{total_fmt} [{elapsed}<{remaining}, '
              '{rate_fmt}{postfix}]'
          Possible vars: l_bar, bar, r_bar, n, n_fmt, total, total_fmt,
              percentage, elapsed, elapsed_s, ncols, nrows, desc, unit,
              rate, rate_fmt, rate_noinv, rate_noinv_fmt,
              rate_inv, rate_inv_fmt, postfix, unit_divisor,
              remaining, remaining_s, eta.
          Note that a trailing ": " is automatically removed after {desc}
          if the latter is empty.
      
+ initial  : int or float, optional
    初始计数器值。重新启动进度条时很有用。默认值：0. If using float, consider specifying `{n:.3f}`
          or similar in `bar_format`, or specifying `unit_scale`.
      
+ position  : int, optional
    指定进度条偏移位置（从0开始）。如果未指定，则自动。对于一次管理多个条（例如，来自线程）很有用。
      
+ postfix  : dict or *, optional
     指定要在工具栏末尾显示的其他统计信息。Calls `set_postfix(**postfix)` if possible (dict).
      
+ unit_divisor  : float, optional
     默认值：1000，除非unit_scale为True，否则将被忽略。
      
+ write_bytes  : bool, optional
     是否写入字节。如果(默认值: False)将写入 Unicode。
      
+ lock_args  : tuple, optional
          Passed to `refresh` for intermediate output
          (initialisation, iterating, and updating).
      
+ nrows  : int, optional
     屏幕高度。If specified, hides nested bars outside this
          bound. If unspecified, attempts to use environment height.
          The fallback is 20.
      
+ colour  : str, optional
     进度条颜色 (e.g. 'green', ' #00ff00 ').
      
+ delay  : float, optional
     在 (默认值：0）秒之后才显示。
      
+ gui  : bool, optional
          WARNING: internal parameter - do not use.
          Use tqdm.gui.tqdm(...) instead. If set, will attempt to use
          matplotlib animations for a graphical output [default: False].

## 常见场景

### 文件下载

```python
import requests
from tqdm import tqdm

response = requests.get(url, stream=True)  # 把stream参数设置为True
file_size = int(response.headers['Content-Length'])
chunk = 1
chunk_size = 1024
num_bars = int(file_size / chunk_size)

with open(filename, 'wb') as fp:
    for chunk in tqdm(response.iter_content(chunk_size=chunk_size), total=num_bars, unit='KB', desc=filename, leave=True): 
        fp.write(chunk)
```

首先获取到要下载文件的大小。然后，我们在 tqdm 中设置总的进度条数目为文件大小除以每个进度条代表的字节数。这样，当我们下载每一个字节块并写入文件时，tqdm 就会自动更新进度条。

### 手动更新

```python
from tqdm import tqdm
import time
with tqdm(total=100) as pbar:
    for i in range(10):
        # 执行一些耗时的操作
        time.sleep(0.1)
        pbar.update(10)
```

使用 update 方法手动更新进度条的值。





> https://github.com/tqdm/tqdm