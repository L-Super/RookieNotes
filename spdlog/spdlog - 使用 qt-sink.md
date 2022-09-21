# spdlog - 使用 qt-sink

spdlog里面有很多sink，其中也有`qt-sink`，所以尝试了一下。

首先，`qt-sink`是指可以将spdlog的日志输出定向（不知此词是否用对）到Qt的QTextEdit、QTextBrowser等控件中进行日志的显示。

示例：

首先需要创建一个用于显示日志的控件，比如`QTextEdit`、`QTextBrowser`类等等

```c++
#include "spdlog/sinks/qt_sinks.h"
auto logger = spdlog::qt_logger_mt("QLogger",ui->textBrowser);
logger->info("hello QTextBrowser");
logger->warn("this msg from spdlog");
```

然后就可以在widget里看到对应的日志输出，而不是控制台了。