Docker 核心组件：
+ Docker 客户端和服务器
+ Docker 镜像
+ Registry
+ Docker 容器

## Docker 客户端和服务器
Docker 是一个客户-服务器 (C/S)架构的程序。Docker 客户端只需向 Docker 服务器或守护进程发出请求，服务器或守护进程将完成所有工作并返回结果。Docker 提供了一个命令行工具 docker 以及一整套 RESTful API。可以在同一台宿主机上运行 Docker 守护进程和客户端，也可以从本地的 Docker 客户端连接到运行在另一台宿主机上的远程 Docker 守护进程。

Docker 的架构：
![](../images/Pasted%20image%2020231121220255.png)
## Docker 镜像
用户基于镜像来运行自己的容器。镜像也是 docker 生命周期中的"构建"部分。镜像基于联合（Union）文件系统的一种层式的结构，由一系列指令一步一步构建出来。
可以把镜像看作容器的"源代码"
## Registry
用来保存用户构建的镜像。Registry 分为公共和私有两种。docker 公司运营的gong

