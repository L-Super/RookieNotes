Docker 镜像是由文件系统叠加而成。最底端是一个引导文件系统，即 bootfs。当一个容器启动后，它会被移到内存中，而引导文件系统则会被卸载（unmount），以留出更多的内存供 initrd 磁盘镜像使用。

Docker 镜像第二层是 root 文件系统 rootfs，位于引导文件系统之上。rootfs 可以是一种或多种操作系统（Debian，Ubuntu）
在 Docker 里，root 文件系统永远是只读状态，并且 Docker 利用联合加载（union mount）技术又会在 root 文件系统层上加载更多的只读文件系统。联合加载指的是一次同时加载多个文件系统，但是外面看起来只能看到一个文件系统。联合加载会将各层文件系统叠加到一起，最终的文件系统会包括所有底层的文件和目录。

Docker 将这样的文件系统称为镜像。一个镜像可以放到另一个镜像的顶部。位于下面的镜像成为父镜像（parent image），依次类推，直到镜像栈的最底部，称为基础镜像（base image）。最后，当从一个镜像启动容器时，Docker 会在该镜像的最顶层加载一个读写文件系统。在 Docker 中运行的程序就是在这个读写层中执行的。

![docker_file_system.excalidraw](../Excalidraw/docker_file_system.excalidraw.md)

当 Docker 第一次启动容器时，初始的读写层是空的。当文件系统发生变化时，这些变化都会应用到这一层上。

## 列出镜像
列出主机上可用的镜像
```sh
docker images
```
在默认的 Registry（Docker Hub 或自己的 Registry）中，镜像是保存在仓库中的。镜像仓库包括镜像、层以及关于镜像的元数据。
每个镜像仓库都可以存放很多镜像（如 Ubuntu 仓库包含了 Ubuntu12.04、13.04、14.04 等的镜像）

为了区分同一个仓库的不同镜像，提供了标签（tag）的功能。可用通过在仓库名后加上冒号和标签名指定仓库中的某一镜像：
```sh
docker run -t -i --name new_container ubuntu:12.04 /bin/bash
```
Docker Hub 中有用户仓库（user repository）和顶层仓库（top-level repository）两种。
用户仓库的命名由用户名和仓库名两部分组成，如 jamtur01/puppet
顶层仓库只包含仓库名部分，如 Ubuntu 仓库。

## 拉取镜像
`docker run` 命令从镜像启动容器时，如果镜像不在本地，会先从 Docker Hub 下载该镜像。如果没有指定具体的镜像标签，会自动下载 latest 标签的镜像。

可通过 `docker pull` 命令来预先拉取镜像到本地。

## 查找镜像
查找所有 Docker Hub 上公共的可用镜像
```
docker search
```
这条命令会完成镜像查找工作，并返回如下信息：
+ 仓库名
+ 镜像描述
+ 用户评价（Stars）：反映镜像的受欢迎程度
+ 是否官方（Official）：由上游开发者管理的镜像（如 fedora 镜像由 Fedora 团队管理）
+ 自动构建（Automated）：表示由 Docker Hub 的自动构建（Automated Build）流程创建的。

![](../images/Pasted%20image%2020231124113026.png)

## 构建镜像
构建 Docker 镜像有以下两种方法：
+ `docker commit` 命令
+ `docker build` 和 Dockerfile 文件

现在不推荐使用 `docker commit` 命令，应该使用更灵活、更强大的 Dockerfile 构建镜像。
### commit 创建镜像
可想象为往版本控制系统里提交变更。
先创建一个容器，然后在容器里做出修改，最后再将修改提交为一个新镜像。

1. 创建容器
```sh
docker run -i -t ubuntu /bin/bash
```
2. 安装 nginx
```sh
apt install nginx
```
然后 `exit` 命令从容器退出
3. 运行 commit
先使用 `docker ps -l -q` 命令得到刚创建的容器 ID：
```sh
# docker ps -l -q
64c45d46f54a
```
然后执行 commit：
```sh
# docker commit 64c45d46f54a chumoshi/nginx                                                                    
sha256:f4c3e80666f5ac11f02769015ca291aacac441ad868fed1a32c3412e868cdbec
```
commit 指定了容器 ID 以及目标镜像仓库和镜像名。

> `docker commit` 提交的只是创建容器的镜像与容器的当前状态之间有差异的部分，这使得该更新非常轻量。

查看新创建的镜像：
```
# docker images chumoshi/nginx
REPOSITORY       TAG       IMAGE ID       CREATED              SIZE
chumoshi/nginx   latest    f4c3e80666f5   About a minute ago   180MB
```
也可以在提交镜像时指定更多的数据（包括标签）来详细描述所做的修改：
```
docker commit -m="custom image" --author="Chu Moshi" 64c45d46f54a chumoshi/nginx:webserver
```
在这条命令中，
+ -m 指定新创建的镜像的提交信息
+ --author 列出镜像的作者信息
然后就是容器 ID，镜像的用户名和仓库名并增加了 webserver 的标签。

### Dockerfile 构建镜像

Dockerfile 使用基于 DSL 语法指令来构建 Docker 镜像，之后使用 `docker build` 命令基于该 Dockerfile 中的指令构建一个新的镜像。
#### 创建 Dockerfile
创建一个目录并在里面创建初始的 Dockerfile

```
mkdir docker_demo
cd docker_demo/
vim Dockerfile
```

Dockerfile 内容：
```dockerfile
# Version 0.0.1
FROM ubuntu:20.04
LABEL maintainer="chumoshi"
RUN apt update
RUN apt install -y nginx
RUN echo 'Hi, I am your container' \
    >/usr/share/nginx/html/index.html
EXPOSE 80
```
Dockerfile 由一系列指令和参数组成。每条指令都必须大写，且后面要跟随一个参数。指令会按从上到下的顺序执行，所以需要合理安排指令的顺序。

每条指令都会创建一个新的镜像层并对镜像进行提交。
大体指令执行流程如下：
+ Docker 从基础镜像运行一个容器
+ 执行一条指令，对容器做出修改
+ 执行类似 `docker commit` 的操作，提交一个新的镜像层
+ 再基于刚提交的镜像运行一个新容器
+ 执行下一条指令，直到所有指令都执行完毕

如果某些原因（如某条指令执行失败）没有正常结束，那么将得到一个可用的镜像。对调试很有帮助，可基于该最后创建的镜像运行，调试指令失败的问题。

1. 每个 Dockerfile 的第一条命令都应该是 FROM。FROM 指定一个已存在的镜像，后续指令都将基于该镜像进行，这个镜像称为基础镜像（base image）
2. LABEL 使用键值对的形式，添加镜像的元数据，这里添加的作者信息。
3. RUN 指令会在当前镜像中运行指定的命令。默认情况下，RUN 指令会在 shell 里使用命令包装器 `/bin/sh -c` 来执行。如果在不支持 shell 的平台上运行或者不希望在 shell 中运行（比如避免 shell 字符串篡改），也可以使用 exec 格式的 RUN 指令：
```dockerfile
RUN ["apt", " install", "-y", "nginx"]
```
在这种方式中，使用数组来指定运行的命令和传递给该命令的每个参数
3. EXPOSE 指令告诉容器内的应用程序将会使用容器的指定端口
可以指定多个 EXPOSE 指令来向外部公开多个端口

#### 基于 Dockerfile 构建新镜像

执行 `docker build` 命令时，Dockerfile 中所有指令都会被执行并提交，并且在该命令成功结束后返回一个新镜像。

```
docker build -t="chumoshi/docker_demo" .
```
-t 选项为新镜像设置了仓库和名称。
![](../images/Pasted%20image%2020231124165549.png)

也可在构建镜像时，设置标签：
```
docker build -t="chumoshi/docker_demo:v1" .
```

> 如果没有制定任何标签，会自动为镜像设置一个 latest 标签

还可以指定一个 Git 仓库的源地址来指定 Dockerfile 的位置：
```
docker build -t="chumoshi/docker_demo:v1" git@github.com:chumoshi/docker-demo
```

#### 调试指令执行失败
假设软件包名写错，如 nginx 写成 ngin：
![](../images/Pasted%20image%2020231124170749.png)

可以看到 Docker 会提示哪一步执行失败，可以根据错误信息进行修正。

#### 构建缓存
Docker 构建过程中会将之前的镜像层看作缓存。比如，第 1-3步执行成功，再次构建时，会直接从第 3 步开始。有时，需要确保构建过程中不会使用缓存，可以使用 `--no-cache` 标志：
```
docker build --no-cache -t="chumoshi/docker_demo" .
```



## 发布镜像到Docker Hub
### 创建并登录 Docker Hub
最重要的一环是如何共享和发布镜像。可将镜像推送到 Docker Hub 或私有 Registry 中。

```
docker login
```
完成 Docker Hub 登录工作，并将认证信息保存起来供以后使用。

```sh
# docker login
Log in with your Docker ID or email address to push and pull images from Docker Hub. If you don't have a Docker ID, head over to https://hub.docker.com/ to create one.
You can log in with your password or a Personal Access Token (PAT). Using a limited-scope PAT grants better security and is required for organizations using SSO. Learn more at https://docs.docker.com/go/access-tokens/

Username: chumoshi
Password: 
WARNING! Your password will be stored unencrypted in /root/.docker/config.json.
Configure a credential helper to remove this warning. See
https://docs.docker.com/engine/reference/commandline/login/#credentials-store

Login Succeeded
```

> 个人认证信息会保存到 `$HOME/.dockercfg` 文件中


### 推送到 Docker Hub
镜像构建完毕之后，可以上传到 Docker Hub。
```
docker push 
```

以上面的 nginx 为例：
```sh
# docker push chumoshi/nginx
Using default tag: latest
The push refers to repository [docker.io/chumoshi/nginx]
955d0d297c3a: Pushed 
256d88da4185: Mounted from library/ubuntu 
latest: digest: sha256:9538ace0b30d4f07d839f076277beac15b26422e5a5da4ddb965682c5788034e size: 741
```
然后就可以在 Docker Hub 上看到上传的镜像
![](../images/Pasted%20image%2020231124155240.png)

## 删除镜像
如果不需要镜像了，可以使用 `docker rmi` 命令删除。
```
docker rmi
```
该命令只会将本地的镜像删除，如果要删除 Docker Hub 上的镜像仓库，需要在网站上进行删除。