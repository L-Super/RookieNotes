
#### 查看端口占用
```shell
netstat -tunlp | grep 8000
```
#### 后台启动程序
```shell
nohup ./MonitorStorageServer &
```
#### 拷贝依赖库到 lib 目录

拷贝 Pmair 应用程序所有的依赖到当前目录下的 lib 子目录下。

```shell
ldd Pamir | awk '{print $3}' | xargs -i cp -L {} ./lib
```

#### 查看 CPU 信息
查看 CPU 处理器信息：

```
  $ lscpu
```

查看内存信息：

```
  $ lsmem
```

查看磁盘信息：

```
  $ fdisk -l
```