## 运行第一个容器
```
docker run -i -t ubuntu /bin/bash
```
+ -i：保证容器中 STDIN 是开启的
+ -t：为创建的 docker 分配一个伪 tty 终端
+ ubuntu：Ubuntu 基础镜像
+ /bin/bash：在容器中运行/bin/bash 命令启动一个 bash shell
Docker 会检查本地是否存在 ubuntu 镜像，如果没有，就会连接 Docker Hub Registry，查看是否有该镜像。一旦找到镜像，就会下载并保存到本地宿主机中。
随后，Docker 在文件系统内部用这个镜像创建了一个新容器。该容器拥有自己的网络、IP 地址以及一个用来和宿主机进行通信的桥接网络接口。
当容器创建完毕之后，Dokcer 就会执行容器中的 `/bin/bash` 命令，这时就可以看到容器内的 shell 了。

```sh
# docker run -i -t ubuntu /bin/bash
Unable to find image 'ubuntu:latest' locally
latest: Pulling from library/ubuntu
aece8493d397: Pull complete 
Digest: sha256:2b7412e6465c3c7fc5bb21d3e6f1917c167358449fecac8176c6e496e5c1f05f
Status: Downloaded newer image for ubuntu:latest
root@37b0673c0d19:/# 
```

输入 exit，可以返回宿主机。退出容器后，容器也随之停止运行。
`docker ps -a ` 可以看到容器状态为已停止

![](../images/Pasted%20image%2020231121225217.png)

```sh
# docker ps -a
CONTAINER ID  IMAGE   COMMAND      CREATED         STATUS       NAMES
37b0673c0d19  ubuntu  "/bin/bash"  13 minutes ago  Exited (0)   stoic_sinoussi
```
## 容器命名
Docker 会为创建的每个容器自动生成一个随机的名称。比如刚刚创建的容器名 stoic_sinoussi。
如果想要为容器指定一个名称，可以使用 `--name` 标志实现：
```sh
docker run --name hello_docker -i -t ubuntu /bin/bash
```
![](../images/Pasted%20image%2020231121230146.png)
## 重新启动已停止的容器
```sh
docker start hello_docker
```
除了容器名，还可以用容器 ID 指定容器
```sh
docker start 85246a3819b1
```

也可以使用 `docker restart` 重新启动容器
## 附着到容器上
重新附着到容器会话上
```sh
docker attach hello_docker
docker attach 85246a3819b1
```
容器名、容器 ID 均可。

## 创建守护式容器
即长期运行的容器。守护式容器没有交互式会话，适合运行应用程序和服务。

```sh
docker run --name daemon_dave -d ubuntu /bin/sh -c "while true;do echo hello world;sleep 1;done"
```
+ -d：后台运行
这里在后台执行一个 while 循环
## 获取容器日志

```sh
docker logs daemon_dave
```

例如：
```sh
# docker logs daemon_dave
hello world
hello world
hello world
hello world
hello world
hello world
hello world
hello world
hello world
```
还可以使用 `-f` 参数监控 Docker 的日志，与 `tail -f` 命令相似
```sh
# docker logs -f daemon_dave
hello world
hello world
hello world
hello world
hello world
hello world
hello world
hello world
```
`Ctrl + C` 退出日志追踪
### 跟踪容器日志的某一片段

获取日志的最后 10 行内容：
```sh
docker logs --tail 10 daemon_dave
```
跟踪容器的最新日志：
```sh
docker logs --tail 0 -f daemon_dave
```
每条日志加上时间戳，可以使调试更简单：
```sh
docker logs -ft daemon_dave
```
## 查看容器内的进程
```sh
docker top daemon_dave
```
## 在容器内运行进程

## 停止守护式容器

## 自动重启容器

## 深入容器
## 删除容器
