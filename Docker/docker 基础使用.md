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
可通过 `docker exec` 命令在容器内部额外启动新进程。可用在容器内运行的进程有两种类型：后台任务和交互式任务。

在容器中运行后台任务：
```sh
docker exec -d daemon_dave touch /etc/new_config_file
```
+ -d：表明需要运行一个后台进程。-d 之后指定容器名以及要执行的命令。
在容器中运行交互式任务：
```sh
docker exec -t -i daemon_dave /bin/bash
```
这条命令会在 daemon_dave 容器内从创建一个新的 bash 会话。
## 停止守护式容器
```sh
docker stop daemon_dave
docker stop 85246a3819b1
```
`docker stop` 会向 Docker 容器进程发送 SIGTERM 信号。如果想快速停止某个容器，可使用 `docker kill` 向容器发送 SIGKILL 信号
## 自动重启容器
由于某些错误而导致容器停止运行，还可以通过 `--restart` 标志，让 Docker 自动重新启动该容器。`--restart` 标志会检查容器的退出代码，并以此来决定是否要重启容器。默认的行为是 Docker 不会重启容器。

```sh
docker run --restart=always --name daemon_dave -d ubuntu /bin/sh -c "while true;do echo hello world;sleep 1;done"
```
`--restart` 标志设置为 always。无论容器退出代码是什么，都会自动重启该容器。


## 深入容器
可使用 `docker inspect` 来获得更多的容器信息

```sh
docker inspect daemon_dave
```
会对容器进行详细的检查，然后返回其配置信息，包括名称、命令、网络配置以及很多有用的数据。
可使用 `-f` 或 `--format` 标志来选定查看结果。

返回容器运行状态：
```
docker inspect --format='{{ .State.Running }}' daemon_dave
```
获取容器 IP 地址：
```
docker inspect --format='{{ .NetworkSettings.IPAddress }}' daemon_dave
```
同时指定多个容器，并显示每个容器的输出结果：
```
docker inspect --format='{{ .Name }} {{ .State.Running }}' daemon_dave hello_docker
```

> 还可以在 `/var/lib/docker` 目录下了解 Docker 的工作原理。该目录下存放着 docker 镜像、容器以及容器的配置。
> 所有的容器都保存在 `/var/lib/docker/containers` 目录下
## 删除容器
如果容器不再使用，可使用 `docker rm` 删除它们

```sh
docker rm 85246a3819b1
```

注：运行中的容器无法删除。

删除所有容器：
```
docker rm `docker ps -a -q`
```
-a 列出所有容器，-q 只返回容器的 ID
