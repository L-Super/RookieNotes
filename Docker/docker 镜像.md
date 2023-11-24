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