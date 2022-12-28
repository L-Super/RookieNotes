Systemd 是 Linux 系统工具，用来启动[守护进程](https://www.ruanyifeng.com/blog/2016/02/linux-daemon.html)


## 1. 创建`.service`配置文件

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