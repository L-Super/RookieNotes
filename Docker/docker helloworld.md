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