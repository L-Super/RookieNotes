[spdlog](https://github.com/gabime/spdlog) 是一个高性能，仅需包含头文件即可编译的C++日志库。

## 目录结构分析

主要目录结构分析如下

```
spdlog
    ├─example  示例代码
    ├─include  真正的实现目录
    │  └─spdlog
    │      ├─details  细节功能函数目录，os,registry相关
    │      ├─fmt  绑定的fmt库目录
    │      ├─sinks  落地文件格式实现
    │      └─*.h    异步模式，日志库接口等实现
    ├─src  *.cpp 文件，组成编译模块生成静态库使用
    ├─tests  测试代码
	├─cmake  cmake相关
```

主要文件解释：

- spdlog/spdlog.h 为日志库接口，提供日志宏的属性控制函数。
- spdlog/logger.h 为日志管理器，为前后端连接的枢纽。
- spdlog/async.h 为异步模式接口。
- spdlog/sinks/base_sink.h 为日志文件格式父类，所有的日志文件格式都是继承该类来实现不同功能。
- spdlog/details/registry.h 用于登记所有的logger，及一些默认的属性，如日志格式、日志写入等级。
- spdlog/pattern_formatter.h 格式化输出