Systemd 是 Linux 系统工具，用来启动[守护进程](https://www.ruanyifeng.com/blog/2016/02/linux-daemon.html)

# 创建流程
## 1. 创建 `.service` 配置文件
比如创建 `test.service`
```
[Unit]
Description=monitor deamon of python version app 
After=rc-local.service

[Service]
Type=simple
User=root
Group=root
WorkingDirectory=/root/monitor
ExecStart=/root/monitor/monitor
Restart=always

[Install]
WantedBy=multi-user.target
```
## 2. 移动配置文件到`/etc/systemd/system`
配置文件主要放在 `/usr/lib/systemd/system` 目录，也可能在 `/etc/systemd/system` 目录。目前在 centos 上是 `/etc/systemd/system`

```shell
cp /home/test.service /etc/systemd/system/
```
## 3. 启动service

```bash
systemctl start test.service
systemctl start httpd
```
带不带后缀似乎无差异

# 常用命令
## 开机启动
```shell
$ systemctl enable test.service

# 以上命令相当于执行以下命令，把test.service添加到开机启动中
$ sudo ln -s  '/etc/systemd/system/test.service'  '/etc/systemd/system/multi-user.target.wants/test.service' 

```
## 查看服务状态

```bash
sudo systemctl status test
```
执行后：
```shell
[root@bogon ~]# systemctl status monitor_py_ver
● monitor_py_ver.service - monitor deamon of python version app
   Loaded: loaded (/etc/systemd/system/monitor_py_ver.service; disabled; vendor preset: disabled)
   Active: active (running) since Wed 2022-12-28 11:04:43 CST; 7min ago
 Main PID: 284962 (monitor)
   CGroup: /system.slice/monitor_py_ver.service
           ├─284962 /root/monitor/monitor
           └─284972 /root/monitor/monitor

Dec 28 11:04:43 bogon systemd[1]: Started monitor deamon of python version app.
Dec 28 11:04:43 bogon monitor[284962]: [2022-12-28 11:04:43,602] [main] [INFO] [main.p...3.4
Dec 28 11:07:20 bogon monitor[284962]: [2022-12-28 11:07:20,346] [main] [INFO] [main.p...ew'
Dec 28 11:07:20 bogon monitor[284962]: [2022-12-28 11:07:20,348] [main] [INFO] [main.p...1}}
Dec 28 11:07:21 bogon monitor[284962]: [2022-12-28 11:07:21,351] [main] [INFO] [main.p...ew'
Dec 28 11:07:21 bogon monitor[284962]: [2022-12-28 11:07:21,353] [main] [INFO] [main.p...1}}
Dec 28 11:07:22 bogon monitor[284962]: [2022-12-28 11:07:22,356] [main] [INFO] [main.p...ew'
Dec 28 11:07:22 bogon monitor[284962]: [2022-12-28 11:07:22,358] [main] [INFO] [main.p...1}}
Dec 28 11:07:23 bogon monitor[284962]: [2022-12-28 11:07:23,361] [main] [INFO] [main.p...ew'
Dec 28 11:07:23 bogon monitor[284962]: [2022-12-28 11:07:23,363] [main] [INFO] [main.p...1}}
Hint: Some lines were ellipsized, use -l to show in full.
```
输出结果含义如下：

> - `Loaded`行：配置文件的位置，是否设为开机启动
> - `Active` 行：表示正在运行
> - `Main PID` 行：主进程 ID
> - `Status`行：由应用本身（这里是 httpd ）提供的软件当前状态
> - `CGroup`块：应用的所有子进程
> - 日志块：应用的日志

## 停止服务

终止正在运行的服务
**停掉该systemd时，服务也会随之停掉**


> ```bash
> systemctl stop test.service
> ```

有时候，该命令可能没有响应，服务停不下来。这时候就不得不"杀进程"了，向正在运行的进程发出`kill`信号。

> ```bash
systemctl kill test.service
> ```

此外，重启服务要执行`systemctl restart`命令。

> ```bash
systemctl restart test. service
> ```

## 重新加载配置文件
当修改了 `.service` 文件后，需要重载才能生效
```bash
# 重新加载一个服务的配置文件
$ sudo systemctl reload apache.service

# 重载所有修改过的配置文件
$ sudo systemctl daemon-reload
```
# `.service` 文件解析解析

