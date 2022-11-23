## [Linux 信号](Linux进程.md##信号)
## signal()

设置一个函数来处理信号

#include <signal.h>

声明：

```c
void (*signal(int sig, void (*func)(int)))(int)
```
