[`signal`]( https://docs.python.org/zh-cn/3.11/library/signal.html#module-signal "signal: Set handlers for asynchronous events.") --- 设置异步事件处理程序
## 一般规则

[`signal. signal ()`]( https://docs.python.org/zh-cn/3.11/library/signal.html#signal.signal "signal. signal") 函数允许定义在接收到信号时执行的自定义处理程序。

## 应用
在 Linux 下，如果未对信号处理，来自键盘的中断 (CTRL + C) 一般会触发异常。默认的动作是引发 [`KeyboardInterrupt`]( https://docs.python.org/zh-cn/3.11/library/exceptions.html#KeyboardInterrupt "KeyboardInterrupt")。

所以，可以使用信号，在程序退出时，进行相关的清理工作

```python
import signal
def sigint_handler(signum, frame):
    # 清理工作
    print("程序退出")
    # arg表示退出状态的整数（默认为 0），也可以是其他类型的对象。通常将 0 视为“成功终止”，非零值视为“异常终止”
    sys.exit(1)

if __name__ == '__main__':
    signal.signal(signal.SIGINT, sigint_handler)   #由Interrupt Key产生，通常是CTRL+C或者DELETE产生的中断

```