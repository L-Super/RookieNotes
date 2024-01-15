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
用来保存用户构建的镜像。Registry 分为公共和私有两种。Docker 公司运营的公共 Registry 叫做 Docker Hub
## 容器
Docker 可以帮你构建和部署容器，只需要把自己的应用程序或服务打包放进容器即可。
可以认为，镜像是 Docker 生命周期中的构建和打包阶段，而容器则是启动或执行阶段。

总结起来，Docker 容器就是：
+ 一个镜像格式
+ 一系列标准的操作
+ 一个执行环境


